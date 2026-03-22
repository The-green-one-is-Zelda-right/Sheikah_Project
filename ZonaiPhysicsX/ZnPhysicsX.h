#pragma once
#include "ZnPhysicsBase.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#include <queue>
#pragma warning (pop)

#include "ZnLayer.h"

#include "MaterialEnum.h"
#include "ZnMaterial.h"

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

	struct ZnTransform;

	struct ZnQueryInfo;

	class ZnPhysicsX : public ZnPhysicsBase
	{
	public:
								ZnPhysicsX() = default;
								~ZnPhysicsX() override = default;

		static ZnPhysicsX*&		Instance();
		static void				Release();

	public:
		void					Initialize(ZnSimulationCallback*) override;
		void					Simulation(float _dt) override;
		void					Finalize() override;

	public:
		ZnMaterialID			CreateMaterial(const MaterialDesc& _desc) override;
		ZnConvexID				ConvexMeshLoadFromPath(const std::wstring& _path) override;
		ZnMeshID				TriangleMeshLoadFromPath(const std::wstring& _path) override;

		bool					ReleaseMaterial(const ZnMaterialID& _id) override;
		bool					ReleaseConvexMesh(const ZnConvexID& _id) override;
		bool					ReleaseTriangleMesh(const ZnMeshID& _id) override;

	public:
		void					CreateScene(void* _userScene, const Eigen::Vector3f& _gravity) override;
		void					LoadScene(void* _userScene) override;
		void					UnloadScene(void* _userScene) override;

		Eigen::Vector3f			GetGravity(void* _userScene) override;
		void					SetGravity(const Eigen::Vector3f& _gravity, void* _userScene) override;

		void					SetCollisionLayer(uint32_t _layer, uint32_t _collision, bool _value) override;
		void					SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data) override;

		void					SetColliderMaterial(ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnMaterialID& _id);
		void					SetColliderMaterials(ZonaiPhysics::ZnCollider*, ZonaiPhysics::ZnMaterialID*, int);

	public:
		ZnCapsuleController* CreateCapsuleController(
			void* _userScene,
			float _radius, float _height,
			const ZnControllerDecs& _desc,
			float _density, const ZnMaterialID& _material) override;

		ZnBoxController* CreateBoxController(
			void* _userScene,
			const Eigen::Vector3f& _extend,
			const ZnControllerDecs& _desc,
			float _density, const ZnMaterialID& _material) override;

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
			void* _userData, 
			const Eigen::Vector3f& extend, 
			const ZnMaterialID& _material,
			void* userScene = nullptr) override;

		ZnCollider* CreateSphereCollider(
			void* _userData, 
			float radius, 
			const ZnMaterialID& _material,
			void* userScene = nullptr) override;

		ZnCollider* CreateCapsuleCollider(
			void* _userData, 
			float radius, float height, 
			const ZnMaterialID& _material,
			void* userScene = nullptr) override;

		ZnCollider* CreateMeshCollider(
			void* _userData, 
			const ZnMeshID& _path,
			const Eigen::Vector3f& _scale,
			const ZnMaterialID& _material,
			void* userScene = nullptr) override;

		ZnCollider* CreateConvexCollider(
			void* _userData, 
			const ZnConvexID& _path,
			const Eigen::Vector3f& _scale,
			const ZnMaterialID& _material,
			void* userScene = nullptr) override;

		/// <summary>
		/// Create Joint
		/// </summary>
		// ���� ����Ʈ
		ZnFixedJoint* CreateFixedJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// �Ÿ� ����Ʈ
		ZnDistanceJoint* CreateDistanceJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// ���� ����Ʈ
		ZnSphericalJoint* CreateSphericalJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// ȸ�� ����Ʈ
		ZnHingeJoint* CreateHingeJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		// ������ ����Ʈ
		ZnPrismaticJoint* CreatePrismaticJoint(
			ZnRigidBody*, const ZnTransform&, 
			ZnRigidBody*, const ZnTransform&) override;
		ZnSpringFlexJoint* CreateSpringFlexJoint(
			ZnRigidBody*, const ZnTransform&,
			ZnRigidBody*, const ZnTransform&) override;

	public:
		bool				Raycast(
			const ZnQueryDesc& _desc, ZnQueryInfo& _out) override;

		bool				Boxcast(
			const Eigen::Vector3f& _extend, 
			const ZnQueryDesc& _desc, ZnQueryInfo& _out) override;
		bool				Spherecast(
			float _radius, 
			const ZnQueryDesc& _desc, ZnQueryInfo& _out) override;
		bool				Capsulecast(
			float _radius, float _height, 
			const ZnQueryDesc& _desc, ZnQueryInfo& _out) override;

		bool				SphereOverlap(
			float _radius,
			const ZnQueryDesc& _desc, ZnQueryInfo& _out) override;

		void ReleaseRigidBody(ZnRigidBody*, void* _userData, void* _userScene = nullptr) override;
		void ReleaseCollider(ZnCollider*, void* _userData, void* _userScene = nullptr) override;
		void ReleaseJoint(ZnJoint*, void* _userData, void* _userScene = nullptr) override;

		// bool Boxcast(const Eigen::Vector3f&, const Eigen::Vector3f&, float, ZnQueryInfo&) override;

	private:
		ZnMaterialID defaultMaterial;

	private:
		static ZnPhysicsX* instance;
	};


	extern "C"
	{
		__declspec(dllexport) ZnPhysicsBase* CreatePhysics();
		__declspec(dllexport) void ReleasePhysics();
	}
} // namespace ZonaiPhysics
