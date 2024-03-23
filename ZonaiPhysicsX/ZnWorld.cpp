#include <PxPhysicsAPI.h>
#include <cassert>

#include "ZnRigidBody.h"
#include "RigidBody.h"
#include "ZnCollider.h"
#include "ZnUtil.h"
#include "ZnRaycastInfo.h"

#include "ZnWorld.h"

#include <ranges>

#include "ZnTransform.h"

namespace ZonaiPhysics
{
	physx::PxScene*										ZnWorld::currScene = nullptr;
	std::unordered_map<void*, physx::PxScene*>			ZnWorld::sceneList{};
	std::map<void*, ZnWorld::Bodies>					ZnWorld::bodyList{};
	std::map<void*, ZnWorld::Colliders>					ZnWorld::colliderList{};
	std::unordered_map<uint32_t, physx::PxMaterial*>	ZnWorld::materials{};

	void ZnWorld::Run(float _dt)
	{
		assert(currScene != nullptr);

		currScene->simulate(_dt);
		currScene->fetchResults(true);
	}

	void ZnWorld::Release()
	{
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

		for (auto& material : materials | std::views::values)
		{
			material->release();
			material = nullptr;
		}

		materials.clear();
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

	void ZnWorld::SetGravity(const Eigen::Vector3f& _gravity, void* _userScene)
	{
		assert(currScene != nullptr);

		_userScene ?
			sceneList[_userScene]->setGravity(EigenToPhysx(_gravity)) :
			currScene->setGravity(EigenToPhysx(_gravity));
	}

	bool ZnWorld::Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out)
	{
		physx::PxRaycastBuffer temp;

		if (bool hit = currScene->raycast(EigenToPhysx(_from), EigenToPhysx(_to), _distance, temp))
		{
			_out.bodyData = static_cast<ZnRigidBody*>(temp.block.actor->userData)->GetUserData();
			_out.colliderData = static_cast<ZnCollider*>(temp.block.shape->userData)->GetUserData();
			_out.position = PhysxToEigen(temp.block.position);
			_out.distance = temp.block.distance;

			return true;
		}

		return false;
	}

	/// 구현 필요
	bool ZnWorld::Boxcast(float _x, float _y, float _z, const ZnTransform& trans)
	{
		physx::PxOverlapBuffer temp;

		const physx::PxTransform transform(EigenToPhysx(trans.position), EigenToPhysx(trans.quaternion));

		if (bool hit = currScene->overlap(physx::PxBoxGeometry(_x, _y, _z), transform, temp))
		{
			// temp.

			return true;
		}

		return false;
	}

	void ZnWorld::AddBody(RigidBody* _znBody, void* _userData, void* _userScene)
	{
		assert(currScene != nullptr);
		assert(_znBody != nullptr);

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

		auto& colliders = colliderList[scene];
		auto& shapes = colliders[_userData];

		auto& bodies = bodyList[scene];
		auto& [znBody, hasBody] = bodies[_userData];

		// 콜라이더가 비어있으면 리지드바디도 아예 삭제시켜야함
		if (shapes.empty())
		{
			const auto pxBody = static_cast<physx::PxRigidDynamic*>(_znBody->pxBody);
			scene->removeActor(*pxBody);

			bodies.erase(_userData);

			delete znBody;
		}
		// 안 비어있으면 삭제시키면 안됨
		else
		{
			hasBody = false;

			_znBody->SetKinematic(true);
			_znBody->UseGravity(false);
		}
	}

	void ZnWorld::RemoveCollider(Collider* _znShape, void* _userData, void* _userScene /*= nullptr*/)
	{
		assert(currScene != nullptr);
		assert(_znShape != nullptr);

		physx::PxScene* scene = _userScene ? sceneList[_userScene] : currScene;

		auto& colliders = colliderList[scene];
		auto& shapes = colliders[_userData];

		shapes.erase(std::ranges::find(shapes, _znShape));

		delete _znShape;

		// 콜라이더가 비어있네?
		if (shapes.empty())
		{
			auto& bodies = bodyList[scene];
			auto& [znBody, hasBody] = bodies[_userData];

			// 근데 바디도 없네?
			if (znBody && !hasBody)
			{
				// 바디 삭제
				const auto pxBody = static_cast<physx::PxRigidDynamic*>(znBody->pxBody);
				scene->removeActor(*pxBody);

				bodies.erase(_userData);

				delete znBody;
			}
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

	void ZnWorld::AddMaterial(uint32_t _id, physx::PxMaterial* _material)
	{
		assert(_material != nullptr);

		materials.insert(std::make_pair(_id, _material));
	}

	physx::PxMaterial* ZnWorld::GetMaterial(uint32_t _id)
	{
		if (materials.contains(_id))
			return materials[_id];

		return nullptr;
	}
}
