#pragma once
#include "ZnPhysicsBase.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

#include "ZnLayer.h"
#include <queue>

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxMaterial;
	class PxPvd;
	class PxRigidDynamic;
}

namespace ZonaiPhysics
{
	class ZnRigidBody;
	class RigidBody;

	class ZnCollider;
	class Collider;

	class ZnJoint;
	class Joint;
	class FixedJoint;

	class ZnTransform;

	struct ZnRaycastInfo;
	struct ZnMaterial;


	class ZnPhysicsX : public ZnPhysicsBase
	{
	public:
		ZnPhysicsX() = default;
		~ZnPhysicsX() override = default;

		static ZnPhysicsX*&		Instance();
		static void				Release();

	public:
		void Initialize(ZnSimulationCallback*) override;
		void Simulation(float _dt) override;
		void Finalize() override;

	public:
		void AddMaterial(
			uint32_t _id, 
			float staticFriction, 
			float dynamicFriction, 
			float _restitution) override;

		void CreateScene(
			void* _userScene, 
			const Eigen::Vector3f& _gravity) override;

		void LoadScene(void* _userScene) override;

		void UnloadScene(void* _userScene) override;

		void SetGravity(
			const Eigen::Vector3f& _gravity,
			void* _userScene) override;

		void SetCollisionLayerData(
			uint32_t _layer, 
			const std::initializer_list<uint32_t>& _data) override;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		ZnRigidBody* CreateRigidBody(
			void* _userData, 
			void* _userScene) override;

		/// <summary>
		/// Create Collider
		/// </summary>
		ZnCollider* CreateBoxCollider(
			void* _userData, const Eigen::Vector3f& extend, 
			uint32_t _material, 
			void* userScene = nullptr) override;

		ZnCollider* CreateSphereCollider(
			void* _userData, float radius, 
			uint32_t _material,
			void* userScene = nullptr) override;

		ZnCollider* CreateCapsuleCollider(
			void* _userData, float radius, float height, 
			uint32_t _material,
			void* userScene = nullptr) override;

		ZnCollider* CreateMeshCollider(void* _userData, const std::wstring& _path, uint32_t _material, void* userScene = nullptr) override;
		ZnCollider* CreateConvexCollider(void* _userData, const std::wstring& _path, uint32_t _material, void* userScene = nullptr) override;

		/// <summary>
		/// Create Joint
		/// </summary>
		// 고정 조인트
		ZnFixedJoint* CreateFixedJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// 거리 조인트
		ZnDistanceJoint* CreateDistanceJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// 구형 조인트
		ZnSphericalJoint* CreateSphericalJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// 회전 조인트
		ZnHingeJoint* CreateHingeJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// 프리즘 조인트
		ZnPrismaticJoint* CreatePrismaticJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;

	public:
		bool Raycast(
			const Eigen::Vector3f& _from,
			const Eigen::Vector3f& _to,
			float _distance,
			ZnRaycastInfo& _out) override;

		void ReleaseRigidBody(ZnRigidBody*, void* _userData, void* _userScene = nullptr) override;
		void ReleaseCollider(ZnCollider*, void* _userData, void* _userScene = nullptr) override;
		void ReleaseJoint(ZnJoint*, void* _userData, void* _userScene = nullptr) override;

		// bool Boxcast(const Eigen::Vector3f&, const Eigen::Vector3f&, float, ZnRaycastInfo&) override;

	private:
		physx::PxMaterial* defaultMaterial;

	private:
		static ZnPhysicsX* instance;
	};


	extern "C"
	{
		__declspec(dllexport) ZnPhysicsBase* CreatePhysics();
		__declspec(dllexport) void ReleasePhysics();
	}
} // namespace ZonaiPhysics
