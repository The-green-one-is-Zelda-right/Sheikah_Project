#pragma once
#include <string>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class FixedJoint;

	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;

	class ZnTransform;
	struct ZnRaycastInfo;

	enum class Shape
	{
		SPHERE,
		BOX,
		CAPSULE,
		PLANE,
		CONVEX,
	};

	class ZnPhysicsBase
	{
	public:
		ZnPhysicsBase() noexcept = default;
		virtual ~ZnPhysicsBase() noexcept = default;
	
	public:
		/// <summary>
		/// 
		/// </summary>
		virtual void			Initialize() noexcept = 0;
		virtual void			Simulation(float _dt) noexcept = 0;
		virtual void			Finalize() noexcept = 0;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody*	CreateRigidBody(const std::wstring&) noexcept = 0;

		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*		CreateBoxCollider(const std::wstring&, float x, float y, float z) noexcept = 0;
		// virtual ZnCollider*		CreatPlaneCollider(const std::wstring&, float x, float y) noexcept = 0;
		virtual ZnCollider*		CreateSphereCollider(const std::wstring&, float radius) noexcept = 0;
		// virtual ZnCollider*		CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept = 0;
		// virtual ZnCollider*		CreateCustomCollider(const std::wstring&) noexcept = 0;

		/// <summary>
		/// Create Joint
		/// </summary>
		// virtual ZnJoint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept = 0;			// D6 조인트		*사실 뭔지 모름
		virtual ZnJoint*		CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;		// 고정 조인트
		virtual ZnJoint*		CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept = 0;		// 거리 조인트
		// virtual ZnJoint*		CreatSphericalJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept = 0;	// 구형 조인트
		// virtual ZnJoint*		CreatRevoluteJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept = 0;		// 회전 조인트
		// virtual ZnJoint*		CreatPrismaticJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept = 0;	// 프리즘 조인트


		virtual bool Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out) noexcept = 0;
	};
}