#include <PxPhysicsAPI.h>
#include <cassert>

#include "ZnRigidBody.h"
#include "RigidBody.h"
#include "Collider.h"
#include "ZnCollider.h"
#include "ZnJoint.h"
#include "ZnUtil.h"
#include "ZnQueryDesc.h"
#include "ZnQueryInfo.h"

#include "ZnMaterial.h"
#include "ZnResourceID.h"

#include <ranges>
#include <bitset>

#include "ZnTransform.h"
#include "ZnWorld.h"


namespace ZonaiPhysics
{
	std::unordered_map<void*, physx::PxControllerManager*> ZnWorld::controllerManager{};

	physx::PxScene*										ZnWorld::currScene = nullptr;
	std::unordered_map<void*, physx::PxScene*>			ZnWorld::sceneList{};
	std::map<void*, ZnWorld::Bodies>					ZnWorld::bodyList{};
	std::map<void*, ZnWorld::Colliders>					ZnWorld::colliderList{};
	std::vector<ZnJoint*>								ZnWorld::jointList{};
	QueryFilter											ZnWorld::queryFilter;

	void ZnWorld::Run(float _dt)
	{
		assert(currScene != nullptr);

		currScene->simulate(_dt);
		currScene->fetchResults(true);
	}

	void ZnWorld::Release()
	{
		for (const auto& joint : jointList)
		{
			delete joint;
		}

		jointList.clear();

		for (auto& [scene, colliders] : colliderList)
		{
			for (auto& [userData, collider] : colliders)
			{
				if (userData)
				{
					for (auto& shape : collider)
					{
						delete shape;
					}
				}
			}

			colliders.clear();
		}

		colliderList.clear();

		for (auto& [scene, bodies] : bodyList)
		{
			for (auto& [userData, rigidbody] : bodies)
			{
				if (userData)
				{
					auto& [body, hasShape] = rigidbody;
					delete body;
				}
			}

			bodies.clear();
		}

		bodyList.clear();

		for (auto& Scene : sceneList | std::views::values)
		{
			Scene->release();
			Scene = nullptr;
		}

		sceneList.clear();
	}

	physx::PxScene* ZnWorld::GetScene(void* _userScene)
	{
		if (sceneList.contains(_userScene))
		{
			return sceneList[_userScene];
		}

		return nullptr;
	}

	physx::PxScene* ZnWorld::GetCurrentScene()
	{
		assert(currScene != nullptr);

		return currScene;
	}

	void ZnWorld::AddScene(void* _userScene, physx::PxScene* _pxScene)
	{
		assert(_userScene != nullptr && _pxScene != nullptr);

		sceneList.insert({ _userScene, _pxScene });
	}

	void ZnWorld::LoadScene(void* _userScene)
	{
		assert(_userScene != nullptr);

		currScene = sceneList[_userScene];
	}

	void ZnWorld::UnloadScene(void* _userScene)
	{
		assert(_userScene != nullptr);
		auto& scene = sceneList[_userScene];
		assert(scene != nullptr);

		auto& bodies = bodyList[_userScene];
		auto& colliders = colliderList[_userScene];

		for (auto& [userData, rigidbody] : bodies)
		{
			if (userData)
			{
				auto& [body, hasShape] = rigidbody;
				delete body;
			}
		}

		for (auto& [userData, collider] : colliders)
		{
			if (userData)
			{
				for (auto& znCollider : collider)
				{
					delete znCollider;
				}
			}
		}

		bodies.clear();
		bodyList.erase(_userScene);
		colliders.clear();
		colliderList.erase(_userScene);

		PX_RELEASE(scene);
		sceneList.erase(_userScene);
	}

	void ZnWorld::SetManager(void* _userScene, physx::PxControllerManager* _manager)
	{
		if (controllerManager.contains(_userScene))
		{
			_manager->release();
		}
		else
		{
			controllerManager.insert({_userScene, _manager});
		}
	}

	physx::PxControllerManager* ZnWorld::GetManager(void* _userScene)
	{
		if (controllerManager.contains(_userScene))
		{
			return controllerManager[_userScene];
		}

		return nullptr;
	}

	Eigen::Vector3f ZnWorld::GetGravity(void* _userScene)
	{
		assert(currScene != nullptr);

		return PhysxToEigen(_userScene ?
			sceneList[_userScene]->getGravity() :
			currScene->getGravity());
	}

	void ZnWorld::SetGravity(const Eigen::Vector3f& _gravity, void* _userScene)
	{
		assert(currScene != nullptr);

		_userScene ?
			sceneList[_userScene]->setGravity(EigenToPhysx(_gravity)) :
			currScene->setGravity(EigenToPhysx(_gravity));
	}

	bool ZnWorld::Raycast(const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		const physx::PxVec3 from(EigenToPhysx(_desc.position));
		const physx::PxVec3 to(EigenToPhysx(_desc.direction.normalized()));
		const physx::PxReal distance(_desc.distance);
		physx::PxRaycastBuffer result;
		const physx::PxHitFlags flag = physx::PxHitFlag::eDEFAULT;

		physx::PxQueryFilterData filter = physx::PxQueryFilterData();
		filter.flags |= physx::PxQueryFlag::ePREFILTER;
		filter.flags |= physx::PxQueryFlag::eDYNAMIC;
		filter.flags |= physx::PxQueryFlag::eSTATIC;
		filter.data.setToDefault();
		filter.data.word1 = static_cast<physx::PxU32>(_desc.queryLayer);

		physx::PxQueryFilterCallback* callback = &queryFilter;
		const physx::PxQueryCache* cache = nullptr;

		if (bool hit = currScene->raycast(
			from,
			to,
			distance,
			result,
			flag,
			filter,
			callback,
			cache
		))
		{
			_out.bodyData = static_cast<RigidBody*>(result.block.actor->userData)->GetUserData();
			_out.colliderData = static_cast<Collider*>(result.block.shape->userData)->GetUserData();
			_out.position = PhysxToEigen(result.block.position);
			_out.distance = result.block.distance;
			_out.normal = PhysxToEigen(result.block.normal);

			return true;
		}

		return false;
	}

	bool ZnWorld::GeometrySweep(const physx::PxGeometry& _geom, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		const physx::PxTransform transform(EigenToPhysx(_desc.position), EigenToPhysx(_desc.rotation));
		const physx::PxVec3 dir(EigenToPhysx(_desc.direction.normalized()));
		const physx::PxReal distance(_desc.distance);
		physx::PxSweepBuffer result;
		const physx::PxHitFlags flag = physx::PxHitFlag::eDEFAULT;

		physx::PxQueryFilterData filter = physx::PxQueryFilterData();
		filter.flags |= physx::PxQueryFlag::ePREFILTER;
		filter.flags |= physx::PxQueryFlag::eDYNAMIC;
		filter.flags |= physx::PxQueryFlag::eSTATIC;
		filter.data.setToDefault();
		filter.data.word1 = static_cast<physx::PxU32>(_desc.queryLayer);

		physx::PxQueryFilterCallback* callback = &queryFilter;
		const physx::PxQueryCache* cache = nullptr;
		physx::PxReal offset = _desc.offset;

		if (bool block = currScene->sweep(
			_geom, 
			transform, dir, distance, 
			result, flag, 
			filter, callback, 
			cache, offset))
		{
			const physx::PxSweepHit* hit = &result.block;

			_out.bodyData = static_cast<RigidBody*>(hit->actor->userData)->GetUserData();
			_out.colliderData = static_cast<Collider*>(hit->shape->userData)->GetUserData();
			_out.position = PhysxToEigen(hit->position);
			_out.distance = hit->distance;
			_out.normal = PhysxToEigen(hit->normal);

			return true;
		}

		return false;
	}

	bool ZnWorld::Boxcast(const Eigen::Vector3f& _extend, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		physx::PxBoxGeometry geom(_extend.x(), _extend.y(), _extend.z());

		return GeometrySweep(geom, _desc, _out);
	}

	bool ZnWorld::Spherecast(float _radius, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		physx::PxSphereGeometry geom(_radius);

		return GeometrySweep(geom, _desc, _out);
	}

	bool ZnWorld::Capsulecast(float _radius, float _height, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		physx::PxCapsuleGeometry geom(_radius, _height);

		return GeometrySweep(geom, _desc, _out);
	}

	bool ZnWorld::GeometryOverLap(const physx::PxGeometry& _geom, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		using namespace physx;

		const PxTransform transform(EigenToPhysx(_desc.position), EigenToPhysx(_desc.rotation));
		PxU32 bufferSize = _out.shapes.size();
		PxOverlapHit* buffer = new PxOverlapHit[bufferSize];

		PxQueryFilterData filter = PxQueryFilterData();
		filter.flags |= PxQueryFlag::ePREFILTER;
		filter.flags |= PxQueryFlag::eDYNAMIC;
		filter.data.setToDefault();
		filter.data.word1 = static_cast<physx::PxU32>(_desc.queryLayer);

		if (PxSceneQueryExt::overlapMultiple(*currScene, _geom, transform, buffer, bufferSize, filter))
		{
			PxU32 touches = bufferSize;

			for (PxU32 i = 0; i < touches; i++)
			{
				const PxOverlapHit hit = buffer[i];

				if (hit.actor)
					_out.actors[i] = static_cast<RigidBody*>(hit.actor->userData)->GetUserData();

				if (hit.shape)
					_out.shapes[i] = static_cast<Collider*>(hit.shape->userData)->GetUserData();
			}

			delete[] buffer;
			return true;
		}

		delete[] buffer;
		return false;
	}

	bool ZnWorld::BoxOverLap(const Eigen::Vector3f& _extend, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		physx::PxBoxGeometry geom(_extend.x(), _extend.y(), _extend.z());

		return GeometryOverLap(geom, _desc, _out);
	}

	bool ZnWorld::SphereOverLap(float _radius, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		physx::PxSphereGeometry geom(_radius);

		return GeometryOverLap(geom, _desc, _out);
	}

	bool ZnWorld::CapsuleOverLap(float _radius, float _height, const ZnQueryDesc& _desc, ZnQueryInfo& _out)
	{
		physx::PxCapsuleGeometry geom(_radius, _height);

		return GeometryOverLap(geom, _desc, _out);
	}

	void ZnWorld::CreateCharactor()
	{
		physx::PxControllerManager* manager = PxCreateControllerManager(*currScene);

		physx::PxCapsuleControllerDesc desc;
		// desc.height;
		// desc.radius;
		// desc.position;

		manager->createController(desc);
	}

	void ZnWorld::AddBody(RigidBody* _znBody, void* _userData, void* _userScene)
	{
		if (currScene == nullptr || _znBody == nullptr)
		{
			ZONAI_CAUTUON(Add Rigidbody Failed,  Zonai Physics ZnWolrd)

			assert(currScene != nullptr);
			assert(_znBody != nullptr);
		}

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_znBody->pxBody);

		physx::PxScene* scene = _userScene ? sceneList[_userScene] : currScene;
		scene->addActor(*pxBody);

		auto& bodies = bodyList[scene];
		bodies.insert(std::make_pair(_userData, std::make_pair(_znBody, false)));
	}

	void ZnWorld::AddCollider(Collider* _znShape, void* _userData, void* _userScene /*= nullptr*/)
	{
		assert(currScene != nullptr);
		assert(_znShape != nullptr);

		physx::PxScene* scene = _userScene ? sceneList[_userScene] : currScene;

		auto& colliders = colliderList[scene];
		auto& shapes = colliders[_userData];
		shapes.push_back(_znShape);
	}

	void ZnWorld::RemoveBody(RigidBody* _znBody, void* _userData, void* _userScene)
	{
		assert(currScene != nullptr);
		assert(_znBody != nullptr);

		physx::PxScene* scene = _userScene ? sceneList[_userScene] : currScene;

		// 씬의 강체 리스트를 가져옴
		const auto bodiesItr = bodyList.find(scene);
		if (bodiesItr == bodyList.end())
		{
			assert(0);
			return;
		}

		auto& bodies = bodiesItr->second;

		// 강체 리스트에서 유저 데이터에 해당하는 강체를 가져옴
		const auto bodyItr = bodies.find(_userData);
		if (bodyItr == bodies.end())
		{
			assert(0);
			return;
		}

		auto& [znBody, hasBody] = bodyItr->second;

		// 씬의 콜라이더 리스트를 가져옴
		const auto colliderItr = colliderList.find(scene);
		if (colliderItr == colliderList.end())
		{
			assert(0);
			return;
		}

		auto& colliders = colliderItr->second;

		// 콜라이더 리스트에서 유저 데이터에 해당하는 콜라이더를 가져옴
		const auto shapeItr = colliders.find(_userData);
		if (shapeItr == colliders.end())
		{
			// shape가 비어있으면 삭제시킴
			hasBody = false;
			ReleaseBody(&znBody, _userData, scene, hasBody);
			return;
		}

		const auto& shapes = shapeItr->second;
		if (shapes.empty())
		{
			// shape가 비어있음.
			// body도 지움
			hasBody = false;
			ReleaseBody(&znBody, _userData, scene, hasBody);
		}
		else
		{
			// shape가 비어있지 않음.
			// body는 지워짐
			hasBody = false;

			znBody->SetKinematic(true);
			znBody->UseGravity(false);
		}
	}

	void ZnWorld::RemoveCollider(Collider* _znShape, void* _userData, void* _userScene /*= nullptr*/)
	{
		assert(currScene != nullptr);
		assert(_znShape != nullptr);

		physx::PxScene* scene = _userScene ? sceneList[_userScene] : currScene;

		const auto collidersItr = colliderList.find(scene);
		if (collidersItr == colliderList.end())
		{
			// 씬도 없음.
			assert(0);
			return;
		}
		auto& colliders = collidersItr->second;

		const auto shapesItr = colliders.find(_userData);
		if (shapesItr == colliders.end())
		{
			// 유저 데이터에 해당하는 콜라이더 리스트가 없다?
			// 지울 콜라이더도 없는 거임.
			assert(0);
			return;
		}
		auto& shapes = shapesItr->second;

		const auto shapeItr = std::ranges::find(shapes, _znShape);
		if (shapeItr != shapes.end())
		{
			// 콜라이더를 지움
			shapes.erase(shapeItr);
			delete _znShape;
		}

		// 콜라이더가 비어있는가?
		if (shapes.empty())
		{
			colliders.erase(_userData);

			const auto bodiesItr = bodyList.find(scene);
			if (bodiesItr == bodyList.end())
			{
				assert(0);
				return;
			}
			auto& bodies = bodiesItr->second;

			const auto bodyItr = bodies.find(_userData);
			if (bodyItr == bodies.end())
			{
				assert(0);
				return;
			}

			RigidBody* znBody = bodyItr->second.first;
			const bool hasBody = bodyItr->second.second;

			ReleaseBody(&znBody, _userData, scene, hasBody);
		}
	}

	RigidBody* ZnWorld::GetBody(void* _userData, void* _userScene)
	{
		assert(currScene != nullptr);
		assert(_userData != nullptr);

		physx::PxScene* scene = nullptr;
		scene = _userScene ? sceneList[_userScene] : currScene;

		auto& bodies = bodyList[scene];

		if (bodies.contains(_userData))
			return bodies[_userData].first;
		
		return nullptr;
	}

	void ZnWorld::SetHasBody(void* _userData, bool _hasBody, void* _userScene /*= nullptr*/)
	{
		assert(currScene != nullptr);
		assert(_userData != nullptr);

		physx::PxScene* scene = nullptr;
		scene = _userScene ? sceneList[_userScene] : currScene;

		auto& bodies = bodyList[scene];

		if (!bodies.contains(_userData))
			return;
		
		bodies[_userData].second = _hasBody;
	}

	void ZnWorld::AddJoint(ZnJoint* _znJoint)
	{
		assert(_znJoint != nullptr);

		jointList.push_back(_znJoint);
	}

	void ZnWorld::RemoveJoint(ZnJoint* _znJoint, void* _userData, void* _userScene)
	{
		assert(_znJoint != nullptr);

		delete _znJoint;
		jointList.erase(std::ranges::find(jointList, _znJoint));

		_znJoint = nullptr;
	}

	void ZnWorld::ReleaseBody(RigidBody** _znBody, void* _data, physx::PxScene* _scene, bool _hasBody)
	{
		const auto bodiesItr = bodyList.find(_scene);
		if (bodiesItr == bodyList.end())
		{
			assert(0);
		}

		auto& bodies = bodiesItr->second;

		if (!_hasBody)
		{
			const auto pxBody = static_cast<physx::PxRigidDynamic*>((*_znBody)->pxBody);
			_scene->removeActor(*pxBody);

			delete *_znBody;
			bodies.erase(_data);
			*_znBody = nullptr;
		}
	}
}
