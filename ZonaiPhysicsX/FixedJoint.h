#pragma once
#include "Joint.h"

namespace physx
{
	class PxFixedJoint;
}

namespace ZonaiPhysics
{
	class FixedJoint : public Joint
	{
	public:
					FixedJoint() noexcept = default;
		virtual		~FixedJoint() noexcept = default;

	public:
		virtual void		SetObject(ZnObject*, ZnObject*) noexcept override;
		virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept override;

		virtual void		SetLocalPosition(ObjectIndex, const Vector3D&) noexcept override;
		virtual Vector3D	GetLocalPosition(ObjectIndex) const noexcept override;

		virtual void		SetLocalQuaternion(ObjectIndex, const Quaternion&) noexcept override;
		virtual Quaternion	GetLocalQuaternion(ObjectIndex) const noexcept override;

		virtual Vector3D	GetRelativeLinearVelocity() const noexcept override;
		virtual Vector3D	GetRelativeAngularVelocity() const noexcept override;

		virtual void		SetBreakForce(float _force, float _torque) noexcept override;
		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept override;

		void SetLocalPosition(const Vector3D&) noexcept override;
		Vector3D GetLocalPosition() const noexcept override;

		void SetLocalQuaternion(const Quaternion&) noexcept override;
		Quaternion GetLocalQuaternion() const noexcept override;

	private:
		physx::PxFixedJoint* joint;
	};
} // namespace ZonaiPhysics

