#pragma once
#include <string>
#include <Eigen/Dense>

#include "MaterialEnum.h"
#include "ZnResourceID.h"
#include "ZnMaterial.h"
#include "ZnBase.h"

namespace ZonaiPhysics
{
	struct ZnQueryDesc;
	struct ZnControllerDecs;
	class ZnSimulationCallback;
	class ZnPrismaticJoint;
	class ZnHingeJoint;
	class ZnDistanceJoint;
	class ZnSphericalJoint;
	class ZnFixedJoint;
}

namespace ZonaiPhysics
{
	class FixedJoint;
	class ZnCapsuleController;
	class ZnBoxController;

	class ZnBase;
	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;

	class ZnTransform;
	struct ZnQueryInfo;

	struct MaterialDesc;

	class ZnPhysicsBase : public ZnBase
	{
	public:
		virtual						~ZnPhysicsBase() = default;
	
	public:
		virtual void				Initialize(ZnSimulationCallback* = nullptr) = 0;
		virtual void				Simulation(float _dt) = 0;
		virtual void				Finalize() = 0;

		virtual ZnMaterialID		CreateMaterial(const MaterialDesc& _desc) = 0;
		virtual ZnConvexID			ConvexMeshLoadFromPath(const std::wstring& _path) = 0;
		virtual ZnMeshID			TriangleMeshLoadFromPath(const std::wstring& _path) = 0;

		virtual bool				ReleaseMaterial(const ZnMaterialID& _id) = 0;
		virtual bool				ReleaseConvexMesh(const ZnConvexID& _id) = 0;
		virtual bool				ReleaseTriangleMesh(const ZnMeshID& _id) = 0;


		virtual void				CreateScene(void* _userScene, const Eigen::Vector3f& _gravity) = 0;
		virtual void				LoadScene(void* _userScene) = 0;
		virtual void				UnloadScene(void* _userScene) = 0;

	public:
		virtual Eigen::Vector3f		GetGravity(void* _userScene = nullptr) = 0;
		virtual void				SetGravity(const Eigen::Vector3f& _gravity, void* _userScene = nullptr) = 0;
		virtual void				SetCollisionLayer(uint32_t _layer, uint32_t _collision, bool _value) = 0;
		virtual void				SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data) = 0;

	public:
		virtual void				ReleaseRigidBody(ZnRigidBody*, void* _userData, void* _userScene = nullptr) = 0;
		virtual void				ReleaseCollider(ZnCollider*, void* _userData, void* _userScene = nullptr) = 0;
		virtual void				ReleaseJoint(ZnJoint*, void* _userData, void* _userScene = nullptr) = 0;

	public:
		virtual ZnCapsuleController* CreateCapsuleController(
			void* _userScene,
			float _radius, float _height,
			const ZnControllerDecs& _desc,
			float _density, const ZnMaterialID& _material) = 0;

		virtual ZnBoxController* CreateBoxController(
			void* _userScene,
			const Eigen::Vector3f& _extend,
			const ZnControllerDecs& _desc,
			float _density, const ZnMaterialID& _material) = 0;

		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*		CreateRigidBody(void* _userData, void* _userScene = nullptr) = 0;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*			CreateBoxCollider(
			void* _userData, 
			const Eigen::Vector3f& extend, 
			const ZnMaterialID& _material, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateSphereCollider(
			void* _userData, 
			float _radius, 
			const ZnMaterialID& _material, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateCapsuleCollider(
			void* _userData, 
			float _radius, 
			float _height, 
			const ZnMaterialID& _material, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateMeshCollider(
			void* _userData, 
			const ZnMeshID&,
			const Eigen::Quaternionf& _rot,
			const Eigen::Vector3f&,
			const ZnMaterialID&, 
			void* userScene = nullptr) = 0;

		virtual ZnCollider*			CreateConvexCollider(
			void* _userData, 
			const ZnConvexID&,
			const Eigen::Quaternionf&,
			const Eigen::Vector3f&,
			const ZnMaterialID&, 
			void* userScene = nullptr) = 0;

		/// <summary>
		/// Create Joint
		/// </summary>
		// ���� ����Ʈ
		virtual ZnFixedJoint*		CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// �Ÿ� ����Ʈ
		virtual ZnDistanceJoint*	CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// ���� ����Ʈ
		virtual ZnSphericalJoint*	CreateSphericalJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// Ư�� �࿡ ȸ���� ����ϴ� ����Ʈ (�̵� X)
		virtual ZnHingeJoint*		CreateHingeJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;
		// Ư�� �࿡ �̵��� ����ϴ� ����Ʈ (ȸ�� X)
		virtual ZnPrismaticJoint*	CreatePrismaticJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) = 0;

	public:
		virtual bool				Raycast(const ZnQueryDesc& _desc, ZnQueryInfo& _out) = 0;

		virtual bool				Boxcast(const Eigen::Vector3f& _extend, const ZnQueryDesc& _desc, ZnQueryInfo& _out) = 0;
		virtual bool				Spherecast(float _radius, const ZnQueryDesc& _desc, ZnQueryInfo& _out) = 0;
		virtual bool				Capsulecast(float _radius, float _height, const ZnQueryDesc& _desc, ZnQueryInfo& _out) = 0;
	};
}
