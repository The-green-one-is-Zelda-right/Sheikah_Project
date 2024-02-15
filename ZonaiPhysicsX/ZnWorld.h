#pragma once
#include <map>
#include <set>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#pragma warning (pop)

namespace ZonaiPhysics
{
	struct ZnRaycastInfo;
	class EventCallback;
	class ZnTransform;
	class RigidBody;
}

namespace physx
{
	class PxPhysics;
	class PxScene;
}

namespace ZonaiPhysics
{
	class ZnWorld
	{
	public:
		static void					Run(float _dt);
		static void					Release();

	public:
		static void					AddScene(void* _userScene, physx::PxScene* _pxScene);
		static void					LoadScene(void* _userScene);
		static void					UnloadScene(void* _userScene);

		static void					SetGravity(const Eigen::Vector3f&, void* _userScene);
		static bool					Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance,
											ZnRaycastInfo& _out);

		static bool					Boxcast(float _x, float _y, float _z, const ZnTransform& trans);

	public:
		static void					AddBody(void* _znBody, void* _userScene = nullptr);
		static void					RemoveBody(void* _znBody, void* _userScene = nullptr);
		static RigidBody*			GetBody(void* _znBody, void* _userScene = nullptr);

		static void					AddMaterial(uint32_t, physx::PxMaterial*);
		static physx::PxMaterial*	GetMaterial(uint32_t);
		

		// static bool Gometrycast();

	private:
		static physx::PxScene* currScene;
		static std::unordered_map<void*, physx::PxScene*> sceneList;	// [userScene, pxScene]

		using BodyList = std::map<void*, RigidBody*>;					// [userData, znBody]
		static std::map<void*, BodyList> bodies;						// [pxScene, bodyList]
		static std::unordered_map<uint32_t, physx::PxMaterial*> materials;	// [id, pxMaterial]
	};
}


