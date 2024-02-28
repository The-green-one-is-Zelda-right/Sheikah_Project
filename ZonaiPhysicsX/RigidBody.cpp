#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

#include "ZnUtil.h"
#include "RigidBodyHelper.h"
#include "RigidBody.h"


namespace ZonaiPhysics
{
	RigidBody::RigidBody(physx::PxRigidDynamic * _pxBody, void* _userData) : pxBody(_pxBody), userData(_userData)
	{
		_pxBody->userData = this;
	}

	RigidBody::~RigidBody()
	{
		RigidBodyHelper::Release(pxBody);
		pxBody = nullptr;
	}

	void RigidBody::WakeUp()
	{
		RigidBodyHelper::WakeUp(pxBody);
	}

	bool RigidBody::IsSleeping() const
	{
		return RigidBodyHelper::IsSleeping(pxBody);
	}

	void RigidBody::UseGravity(bool value)
	{
		RigidBodyHelper::UseGravity(pxBody, value);
	}

	void RigidBody::SetKinematic(bool value)
	{
		RigidBodyHelper::SetKinematic(pxBody, value);
	}

	void RigidBody::Disable(bool value) const
	{
		RigidBodyHelper::Disable(pxBody, value);
	}

	uint8_t RigidBody::GetDynamicLockFlags() const
	{
		return RigidBodyHelper::GetDynamicLockFlags(pxBody);
	}

	void RigidBody::SetDynamicLockFlag(FreezeFlag _flag, bool _value)
	{
		RigidBodyHelper::SetDynamicLockFlag(pxBody, _flag, _value);
	}

	void RigidBody::SetDynamicLockFlags(uint8_t _flags)
	{
		RigidBodyHelper::SetDynamicLockFlags(pxBody, _flags);
	}

	void* RigidBody::GetUserData() const
	{
		return RigidBodyHelper::GetUserData(pxBody);
	}

	void RigidBody::SetUserData(void* _userData)
	{
		RigidBodyHelper::SetUserData(pxBody, _userData);
	}

	float RigidBody::GetMass() const
	{
		return RigidBodyHelper::GetMass(pxBody);
	}

	void RigidBody::SetMass(float _mass)
	{
		assert(pxBody != nullptr);

		RigidBodyHelper::SetMass(pxBody, _mass);
	}

	float RigidBody::GetInvMass() const
	{
		return RigidBodyHelper::GetInvMass(pxBody);
	}

	Eigen::Vector3f RigidBody::GetInertiaTensor() const
	{
		return RigidBodyHelper::GetInertiaTensor(pxBody);
	}

	float RigidBody::GetLinearDamping() const
	{
		return RigidBodyHelper::GetLinearDamping(pxBody);
	}

	void RigidBody::SetLinearDamping(float _damping)
	{
		RigidBodyHelper::SetLinearDamping(pxBody, _damping);
	}

	float RigidBody::GetAngularDamping() const
	{
		return RigidBodyHelper::GetAngularDamping(pxBody);
	}

	void RigidBody::SetAngularDamping(float _damping)
	{
		RigidBodyHelper::SetAngularDamping(pxBody, _damping);
	}

	Eigen::Vector3f RigidBody::GetLinearVelocity() const
	{
		return RigidBodyHelper::GetLinearVelocity(pxBody);
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity)
	{
		RigidBodyHelper::SetLinearVelocity(pxBody, _velocity);
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const
	{
		return RigidBodyHelper::GetAngularVelocity(pxBody);
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity)
	{
		RigidBodyHelper::SetAngularVelocity(pxBody, _velocity);
	}

	float RigidBody::GetMaxLinearVelocity() const
	{
		return RigidBodyHelper::GetMaxLinearVelocity(pxBody);
	}

	void RigidBody::SetMaxLinearVelocity(const float& _maxVelocity)
	{
		RigidBodyHelper::SetMaxLinearVelocity(pxBody, _maxVelocity);
	}

	float RigidBody::GetMaxAngularVelocity() const
	{
		return RigidBodyHelper::GetMaxAngularVelocity(pxBody);
	}

	void RigidBody::SetMaxAngularVelocity(const float& _maxVelocity)
	{
		RigidBodyHelper::SetMaxAngularVelocity(pxBody, _maxVelocity);
	}

	Eigen::Vector3f RigidBody::GetPosition() const
	{
		return RigidBodyHelper::GetPosition(pxBody);
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _position)
	{
		RigidBodyHelper::SetPosition(pxBody, _position);
	}

	Eigen::Quaternionf RigidBody::GetQuaternion() const
	{
		return RigidBodyHelper::GetQuaternion(pxBody);
	}

	void RigidBody::SetQuaternion(const Eigen::Quaternionf& _quaternion)
	{
		RigidBodyHelper::SetQuaternion(pxBody, _quaternion);
	}

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ForceType _type)
	{
		RigidBodyHelper::AddForce(pxBody, _force, _type);
	}

	void RigidBody::ClearForce()
	{
		RigidBodyHelper::ClearForce(pxBody);
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ForceType _type)
	{
		RigidBodyHelper::AddTorque(pxBody, _torque, _type);
	}

	void RigidBody::ClearTorque()
	{
		RigidBodyHelper::ClearTorque(pxBody);
	}
} // namespace ZonaiPhysics