#pragma once
#include <map>
#include <set>
#include <vector>
#include <queue>
#include "ZnMaterial.h"
#include "ZnResourceID.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>

#include "QueryFilterCallback.h"
#pragma warning (pop)

namespace ZonaiPhysics
{
	struct ZnQueryInfo;
	class EventCallback;
	struct ZnTransform;
	struct ZnQueryDesc;
	class RigidBody;
	class Collider;

	class ZnJoint;
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
		static physx::PxScene*		GetScene(void* _userScene);
		static physx::PxScene*		GetCurrentScene();
		static void					AddScene(void* _userScene, physx::PxScene* _pxScene);
		static void					LoadScene(void* _userScene);
		static void					UnloadScene(void* _userScene);

		static void					SetManager(void* _userScene, physx::PxControllerManager*);
		static physx::PxControllerManager* GetManager(void* _userScene);

		static Eigen::Vector3f		GetGravity(void* _userScene);
		static void					SetGravity(const Eigen::Vector3f&, void* _userScene);

		static bool					Raycast(const ZnQueryDesc& _desc, ZnQueryInfo& _out);

		static bool					GeometrySweep(const physx::PxGeometry& _geom, const ZnQueryDesc& _desc, ZnQueryInfo& _out);
		static bool					Boxcast(const Eigen::Vector3f& _extend,		const ZnQueryDesc& _desc, ZnQueryInfo& _out);
		static bool					Spherecast(float _radius,					const ZnQueryDesc& _desc, ZnQueryInfo& _out);
		static bool					Capsulecast(float _radius, float _height,	const ZnQueryDesc& _desc, ZnQueryInfo& _out);

		static bool					GeometryOverLap(const physx::PxGeometry& _geom,	const ZnQueryDesc& _desc, ZnQueryInfo& _out);
		static bool					BoxOverLap(const Eigen::Vector3f& _extend, const ZnQueryDesc& _desc, ZnQueryInfo& _out);
		static bool					SphereOverLap(float _radius, const ZnQueryDesc& _desc, ZnQueryInfo& _out);
		static bool					CapsuleOverLap(float _radius, float _height, const ZnQueryDesc& _desc, ZnQueryInfo& _out);
		
	public:
		static void					CreateCharactor();

		static void					AddBody(RigidBody* _znBody, void* _userData, void* _userScene = nullptr);
		static void					RemoveBody(RigidBody* _znBody, void* _userData, void* _userScene = nullptr);

		static void					AddCollider(Collider* _znShape, void* _userData, void* _userScene = nullptr);
		static void					RemoveCollider(Collider* _znShape, void* _userData, void* _userScene = nullptr);


		static RigidBody*			GetBody(void* _znBody, void* _userScene = nullptr);
		static void					SetHasBody(void* _userData, bool _hasBody, void* _userScene = nullptr);

		static void					AddJoint(ZnJoint* znJoint);
		static void					RemoveJoint(ZnJoint* _znJoint, void* _userData, void* _userScene = nullptr);

	private:
		static void					ReleaseBody(RigidBody**, void* _data, physx::PxScene*, bool);


	private:
		static std::unordered_map<void*, physx::PxControllerManager*> controllerManager;// [userScene, pxScene]

		static physx::PxScene* currScene;
		static std::unordered_map<void*, physx::PxScene*> sceneList;					// [userScene, pxScene]

		using Bodies = std::map<void*, std::pair<RigidBody*, bool>>;					// [userData, RigidBody]
		static std::map<void*, Bodies>		bodyList;									// [pxScene, [userData, RigidBody]]

		using Colliders = std::map<void*, std::vector<Collider*>>;						// [userData, Colliders]
		static std::map<void*, Colliders>	colliderList;								// [pxScene, [userData, Colliders]]

		static std::vector<ZnJoint*> jointList;

		static QueryFilter queryFilter;
	};
}


