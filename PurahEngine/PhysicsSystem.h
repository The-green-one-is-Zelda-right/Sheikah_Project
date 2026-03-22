#pragma once
#include "PurahEngineAPI.h"
#include <functional>
#include <queue>
#include <vector>
#include <windows.h>
#include "ZnQueryInfo.h"
#include "ZnMaterial.h"
#include "MaterialEnum.h"

#define PHYSCIS_CAUTUON(header, string)	\
		MessageBox(0,					\
			L#header,					\
			L#string,					\
			MB_OK | MB_ICONEXCLAMATION	\
		);								\

#define NULL_POINTER_REFERENCE(pointer, header, error_path)\
if(pointer == nullptr)					\
	PHYSCIS_CAUTUON(header, error_path)	\
// 	assert(pointer != nullptr);			\


namespace ZonaiPhysics
{
	struct ZnQueryDesc;
	class ZnPhysicsBase;

	class ZnRigidBody;
	class ZnBoxController;
	class ZnCapsuleController;

	class ZnCollider;

	class ZnJoint;
	class ZnFixedJoint;
	class ZnPrismaticJoint;
	class ZnHingeJoint;
	class ZnSphericalJoint;
	class ZnDistanceJoint;

	struct ZnTransform;
}

namespace PurahEngine
{
	class EventCallbackSystem;
	class Collider;
	class RigidBody;
	class Joint;

	class PURAHENGINE_API PhysicsSystem
	{
	public:
		PhysicsSystem() = default;
		~PhysicsSystem() = default;

		PhysicsSystem(const PhysicsSystem&) = delete;
		PhysicsSystem(PhysicsSystem&&) noexcept = delete;

		PhysicsSystem& operator=(const PhysicsSystem&) = delete;
		PhysicsSystem& operator=(PhysicsSystem&&) noexcept = delete;

	private:
		ZonaiPhysics::ZnPhysicsBase* physics;
		HMODULE ZonaiPhysicsXDLL;
		std::function<void()> releaseFuntion;

	public:
		void Initialize();
		void PreStep() const;
		void Simulation(float _dt) const;
		void SimulateResult() const;
		void Finalize();

	private:
		friend class Physics;
		Eigen::Vector3f GetGravity() const;

		void SetGravity(const Eigen::Vector3f&) const;

	public:
		void FreeObject(ZonaiPhysics::ZnRigidBody*, void* _gameObject) const;
		void FreeObject(ZonaiPhysics::ZnCollider*, void* _gameObject) const;
		void FreeObject(ZonaiPhysics::ZnJoint*, void* _gameObject) const;

	public:
		std::unordered_map<std::wstring, ZonaiPhysics::ZnMaterialID> materialNameTable;
		std::unordered_map<std::wstring, ZonaiPhysics::ZnConvexID> convexNameTable;
		std::unordered_map<std::wstring, ZonaiPhysics::ZnMeshID> meshNameTable;

		std::vector<PurahEngine::RigidBody*> bodies;
		std::vector<PurahEngine::Collider*> dynamicColliders;
		std::vector<PurahEngine::Collider*> staticColliders;
		std::vector<PurahEngine::Joint*> joints;
		std::queue<PurahEngine::Joint*> removejointBuffer;
		EventCallbackSystem* callbackSystem;

	public:
		ZonaiPhysics::ZnConvexID		CreateConvexMeshFromPath(const std::wstring&, bool _make = false);
		ZonaiPhysics::ZnMeshID			CreateTriangleMeshFromPath(const std::wstring&, bool _make = false);

		bool							ReleaseConvexMesh(const ZonaiPhysics::ZnConvexID&);
		bool							ReleaseTriangleMesh(const ZonaiPhysics::ZnMeshID&);

		bool							BindMaterial(const std::wstring&, const ZonaiPhysics::ZnMaterialID& _id, bool _make = false);
		bool							BindConvex(const std::wstring&, const ZonaiPhysics::ZnConvexID& _id, bool _make = false);
		bool							BindMesh(const std::wstring&, const ZonaiPhysics::ZnMeshID& _id, bool _make = false);

		ZonaiPhysics::ZnMaterialID		GetMaterialID(const std::wstring& _name);
		ZonaiPhysics::ZnConvexID		GetConvexID(const std::wstring& _name);
		ZonaiPhysics::ZnMeshID			GetMeshID(const std::wstring& _name); 

		void							SetColliderMaterial(ZonaiPhysics::ZnCollider* _collider, std::wstring* _materials, int _size);

	public:
		ZonaiPhysics::ZnBoxController*			CreateBoxController(void* _gameObject) const;
		ZonaiPhysics::ZnCapsuleController*		CreateCapsuleController(void* _gameObject) const;

		ZonaiPhysics::ZnRigidBody*				CreateRigidBody(void* _gameObject) const;

	public:
		ZonaiPhysics::ZnCollider*		CreateBoxCollider(
			void* _gameObject,
			float x, float y, float z,
			const std::wstring&);

		ZonaiPhysics::ZnCollider*		CreateSphereCollider(
			void* _gameObject,
			float radius,
			const std::wstring&);

		ZonaiPhysics::ZnCollider*		CreateCapsuleCollider(
			void* _gameObject,
			float radius, float height,
			const std::wstring&);

		ZonaiPhysics::ZnCollider*		CreateConvexCollider(
			void* _gameObject,
			const std::wstring& _path,
			const Eigen::Vector3f& _scale,
			const std::wstring&);

		ZonaiPhysics::ZnCollider*		CreateMeshCollider(
			void* _gameObject,
			const std::wstring& _path,
			const Eigen::Vector3f& _scale,
			const std::wstring&);

	public:
		ZonaiPhysics::ZnFixedJoint*		CreateFixedJoint(
			ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0,
			ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const;

		ZonaiPhysics::ZnPrismaticJoint*	CreateSlideJoint(
			ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0, 
			ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const;

		ZonaiPhysics::ZnHingeJoint*		CreateHingeJoint(
			ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0, 
			ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const;

		ZonaiPhysics::ZnSphericalJoint*	CreateBallJoint(
			ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0, 
			ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const;

		ZonaiPhysics::ZnDistanceJoint*	CreateDistanceJoint(
			ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0, 
			ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const;

		ZonaiPhysics::ZnDistanceJoint*	CreateSpringJoint(
			ZonaiPhysics::ZnRigidBody* _body0, const ZonaiPhysics::ZnTransform& _localTm0, 
			ZonaiPhysics::ZnRigidBody* _body1, const ZonaiPhysics::ZnTransform& _localTm1) const;

	private:
		bool Raycast(const ZonaiPhysics::ZnQueryDesc& _desc, ZonaiPhysics::ZnQueryInfo& _out);

		bool Boxcast(const Eigen::Vector3f _extend, const ZonaiPhysics::ZnQueryDesc& _desc, ZonaiPhysics::ZnQueryInfo& _out);
		bool Spherecast(float _radius, const ZonaiPhysics::ZnQueryDesc& _desc, ZonaiPhysics::ZnQueryInfo& _out);
		bool Capsulecast(float _radius, float _height, const ZonaiPhysics::ZnQueryDesc& _desc, ZonaiPhysics::ZnQueryInfo& _out);
		
		bool SphereOverlap(float _radius, const ZonaiPhysics::ZnQueryDesc& _desc, ZonaiPhysics::ZnQueryInfo& _out);

	public:
		static PhysicsSystem& GetInstance();
	};
}
