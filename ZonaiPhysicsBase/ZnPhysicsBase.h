#pragma once
#include <string>
#include <Eigen/Dense>
#include "ZnBase.h"

namespace ZonaiPhysics
{
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

	class ZnBase;
	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;

	class ZnTransform;
	struct ZnRaycastInfo;

	class ZnPhysicsBase : public ZnBase
	{
	public:
		virtual ~ZnPhysicsBase() = default;
	
	public:
		/// <summary>
		/// 
		/// </summary>
		virtual void				Initialize(ZnSimulationCallback* = nullptr) = 0;
		virtual void				Simulation(float _dt) = 0;
		virtual void				Finalize() = 0;

		virtual void				AddMaterial(uint32_t _id, float staticFriction,	float dynamicFriction, float _restitution) = 0;

		virtual void				CreateScene(void* _userScene, const Eigen::Vector3f& _gravity) = 0;
		virtual void				LoadScene(void* _userScene) = 0;
		virtual void				UnloadScene(void* _userScene) = 0;

	public:
		virtual void				SetGravity(const Eigen::Vector3f& _gravity, void* _userScene = nullptr) = 0;
		virtual void				SetCollisionLayerData(uint32_t _layer,const std::initializer_list<uint32_t>& _data) = 0;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*		CreateRigidBody(void* _userData, void* _userScene = nullptr) = 0;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*			CreateBoxCollider(void* _userData, const Eigen::Vector3f& extend, uint32_t _material, void* userScene = nullptr) = 0;
		virtual ZnCollider*			CreateSphereCollider(void* _userData, float _radius, uint32_t _material, void* userScene = nullptr) = 0;
		virtual ZnCollider*			CreateCapsuleCollider(void* _userData, float _radius, float _height, uint32_t _material, void* userScene = nullptr) = 0;
		virtual ZnCollider*			CreateMeshCollider(void* _userData, const std::wstring&, uint32_t _material, void* userScene = nullptr) = 0;
		virtual ZnCollider*			CreateConvexCollider(void* _userData, const std::wstring&, uint32_t _material, void* userScene = nullptr) = 0;
		// virtual ZnCollider*		CreateCustomCollider(const std::wstring&) = 0;

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
		virtual bool				Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out) = 0;
	};
}
