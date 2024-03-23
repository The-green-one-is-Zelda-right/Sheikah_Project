#pragma once
#include <map>
#include <set>
#include <vector>
#include <queue>

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
	class Collider;
	class Joint;
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
		static void					AddBody(RigidBody* _znBody, void* _userData, void* _userScene = nullptr);
		static void					RemoveBody(RigidBody* _znBody, void* _userData, void* _userScene = nullptr);

		static void					AddCollider(Collider* _znShape, void* _userData, void* _userScene = nullptr);
		static void					RemoveCollider(Collider* _znShape, void* _userData, void* _userScene = nullptr);
		static RigidBody*			GetBody(void* _znBody, void* _userScene = nullptr);
		static void					SetHasBody(void* _userData, bool _hasBody, void* _userScene = nullptr);

		static void					AddMaterial(uint32_t, physx::PxMaterial*);
		static physx::PxMaterial*	GetMaterial(uint32_t);
		

		// static bool Gometrycast();

	private:
		static physx::PxScene* currScene;
		static std::unordered_map<void*, physx::PxScene*> sceneList;					// [userScene, pxScene]

		using Bodies = std::map<void*, std::pair<RigidBody*, bool>>;										// [userData, RigidBody]
		static std::map<void*, Bodies>		bodyList;									// [pxScene, [userData, RigidBody]]

		using Colliders = std::map<void*, std::vector<Collider*>>;						// [userData, Colliders]
		static std::map<void*, Colliders>	colliderList;								// [pxScene, [userData, Colliders]]

		static std::unordered_map<uint32_t, physx::PxMaterial*> materials;	// [id, pxMaterial]

		// static std::queue<RigidBody*>	rigidbodyRemoveList;
		// static std::queue<Collider*>		colliderRemoveList;
		// static std::queue<Joint*>		jointRemoveList;
	};
}


