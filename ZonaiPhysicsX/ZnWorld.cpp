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
	std::map<void*, ZnWorld::BodyList>					ZnWorld::bodies{};
	std::unordered_map<uint32_t, physx::PxMaterial*>	ZnWorld::materials{};

	void ZnWorld::Run(float _dt)
	{
		assert(currScene != nullptr);

		currScene->simulate(_dt);
		currScene->fetchResults(true);
	}

	void ZnWorld::Release()
	{
		for (auto& [scene, bodyList] : bodies)
		{
			for(auto& znBody : bodyList | std::views::values)
			{
				if (znBody)
				{
					delete znBody;
					znBody = nullptr;
				}
			}

			bodyList.clear();
			sceneList[scene]->release();
		}

		bodies.clear();

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

		auto& bodylist = bodies[_userScene];

		for (auto& znBody : bodylist | std::views::values)
		{
			if (znBody)
			{
				delete znBody;
				znBody = nullptr;
			}
		}

		bodylist.clear();
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

	void ZnWorld::AddBody(void* _znBody, void* _userScene)
	{
		assert(currScene != nullptr);
		assert(_znBody != nullptr);

		const auto znBody = static_cast<RigidBody*>(_znBody);
		const auto pxBody = static_cast<physx::PxRigidDynamic*>(znBody->pxBody);
		physx::PxScene* scene = _userScene ? sceneList[_userScene] : currScene;
		scene->addActor(*pxBody);

		bodies[scene].insert(std::make_pair(znBody->userData, znBody));
	}

	void ZnWorld::RemoveBody(void* _znBody, void* _userScene)
	{
		assert(currScene != nullptr);
		assert(_znBody != nullptr);

		const auto znBody = static_cast<RigidBody*>(_znBody);
		const auto pxBody = static_cast<physx::PxRigidDynamic*>(znBody->pxBody);
		physx::PxScene* scene = _userScene ? sceneList[_userScene] : currScene;
		scene->removeActor(*pxBody);

		bodies[scene].erase(znBody->userData);
		delete znBody;
	}

	RigidBody* ZnWorld::GetBody(void* _userData, void* _userScene)
	{
		assert(currScene != nullptr);
		assert(_userData != nullptr);

		physx::PxScene* scene = nullptr;
		scene = _userScene ? sceneList[_userScene] : currScene;

		auto& bodyList = bodies[scene];
		if (bodyList.contains(_userData))
			return bodyList[_userData];
		
		return nullptr;
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
