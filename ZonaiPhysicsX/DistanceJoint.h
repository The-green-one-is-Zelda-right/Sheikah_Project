#pragma once
#include "ZnDistanceJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
	class PxFixedJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class DistanceJoint : public ZnDistanceJoint
	{
	public:
		DistanceJoint() noexcept = delete;
		DistanceJoint(
			physx::PxPhysics*& _factory,
			RigidBody* _object0, const ZnTransform& _transform0,
			RigidBody* _object1, const ZnTransform& _transform1) noexcept;
		~DistanceJoint() noexcept override;

	public:
		void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept override;

		void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept override;
		Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept override;

		Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept override;

		void		SetBreakForce(float _force, float _torque) noexcept override;
		void		GetBreakForce(float& _force, float& _torque) const noexcept override;

		virtual float	GetDistance() const override;

		virtual void	SetMinDistance(float _distance) override;
		virtual float	GetMinDistance() const override;

		virtual void	SetMaxDistance(float _distance) override;
		virtual float	GetMaxDistance() const override;

		virtual void	SetTolerance(float _tolerance) override;
		virtual float	GetTolerance() const override;

		virtual void	SetStiffness(float _stiffness) override;
		virtual float	GetStiffness() const override;

		virtual void	SetDamping(float _damping) override;
		virtual float	GetDamping() const override;

		virtual void	SetMinDistanceEnable(bool _value) override;
		virtual void	SetMaxDistanceEnable(bool _value) override;
		virtual void	SetSpringEnable(bool _value) override;

	private:
		RigidBody* object[2];
		physx::PxDistanceJoint* joint;
	};
}

