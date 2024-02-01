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
		ZnPhysicsBase() noexcept = default;
		virtual ~ZnPhysicsBase() noexcept = default;
	
	public:
		/// <summary>
		/// 
		/// </summary>
		virtual void				Initialize(ZnSimulationCallback* = nullptr) noexcept = 0;
		virtual void				Simulation(float _dt) noexcept = 0;
		virtual void				Finalize() noexcept = 0;

		virtual void				CreateScene(void* _userScene, const Eigen::Vector3f& _gravity) noexcept = 0;
		virtual void				LoadScene(void* _userScene) noexcept = 0;
		virtual void				UnloadScene(void* _userScene) noexcept = 0;

		virtual void				SetGravity(const Eigen::Vector3f&, void* _userScene = nullptr) noexcept = 0;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*		CreateRigidBody(void* _userData, void* _userScene = nullptr) noexcept = 0;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*			CreateBoxCollider(void* _userData, float x, float y, float z) noexcept = 0;
		virtual ZnCollider*			CreateSphereCollider(void* _userData, float radius) noexcept = 0;
		virtual ZnCollider*			CreateCapsuleCollider(void* _userData, float radius, float height) noexcept = 0;
		// virtual ZnCollider*		CreateCustomCollider(const std::wstring&) noexcept = 0;

		/// <summary>
		/// Create Joint
		/// </summary>
		// 
		// virtual ZnD6Joint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept = 0;			// D6 ����Ʈ		*��� ���� ��
		// ���� ����Ʈ
		virtual ZnFixedJoint*		CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// �Ÿ� ����Ʈ
		virtual ZnDistanceJoint*	CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// ���� ����Ʈ
		virtual ZnSphericalJoint*	CreateSphericalJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// Ư�� �࿡ ȸ���� ����ϴ� ����Ʈ (�̵� X)
		virtual ZnHingeJoint*		CreateHingeJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;
		// Ư�� �࿡ �̵��� ����ϴ� ����Ʈ (ȸ�� X)
		virtual ZnPrismaticJoint*	CreatePrismaticJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;

	public:
		virtual bool Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out) noexcept = 0;
	};
}
