#include "RigidBody.h"

namespace ZonaiPhysics
{
	Eigen::Vector3f RigidBody::GetPosition() const noexcept
	{
		return position;
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		position = _position;
	}

	Eigen::Quaternionf RigidBody::GetQuaternion() const noexcept
	{
		return rotation;
	}

	void RigidBody::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		rotation = _quaternion;
	}

	void* RigidBody::GetUserData() const noexcept
	{
		return userData;
	}

	void RigidBody::SetUserData(void* _userData) noexcept
	{
		userData = _userData;
	}

	void RigidBody::WakeUp() noexcept
	{
		active = true;
	}

	bool RigidBody::IsSleeping() const noexcept
	{
		return !active;
	}

	DynamicLocks RigidBody::GetDynamicLockFlags() const noexcept
	{
		return lockFlags;
	}

	void RigidBody::SetDynamicLockFlag(DynamicLock _flag, bool _value) noexcept
	{
		const auto flag = static_cast<uint32_t>(_flag);

		if (_value)
		{
			lockFlags |= flag;
		}
		else
		{
			lockFlags &= ~flag;
		}
	}

	void RigidBody::SetDynamicLockFlags(DynamicLocks _flags) noexcept
	{
		lockFlags = _flags;
	}

	float RigidBody::GetMass() const noexcept
	{
		return massData.mass;
	}

	void RigidBody::SetMass(float _mass) noexcept
	{
		massData.mass = _mass;
	}

	float RigidBody::GetInvMass() const noexcept
	{
		return massData.invMass;
	}

	Eigen::Vector3f RigidBody::GetInertiaTensor() const noexcept
	{
		return massData.inerita;
	}

	void RigidBody::SetInertiaTensor(const Eigen::Vector3f& _inertia) noexcept
	{
		massData.inerita = _inertia;
	}

	float RigidBody::GetLinearDamping() const noexcept
	{
		return material.linearDamping;
	}

	void RigidBody::SetLinearDamping(float _damping) noexcept
	{
		material.linearDamping = _damping;
	}

	float RigidBody::GetAngularDamping() const noexcept
	{
		return material.angularDamping;
	}

	void RigidBody::SetAngularDamping(float _damping) noexcept
	{
		material.angularDamping = _damping;
	}

	Vector3f RigidBody::GetLinearVelocity() const noexcept
	{
		return linearVelocity;
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		linearVelocity = _velocity;
	}

	Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		return angularVelocity;
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		angularVelocity = _velocity;
	}

	float RigidBody::GetMaxLinearVelocity() const noexcept
	{
		return limitLinearVelocity;
	}

	void RigidBody::SetMaxLinearVelocity(const float& _limit) noexcept
	{
		limitLinearVelocity = _limit;
	}

	float RigidBody::GetMaxAngularVelocity() const noexcept
	{
		return limitAngularVelocity;
	}

	void RigidBody::SetMaxAngularVelocity(const float& _limit) noexcept
	{
		limitAngularVelocity = _limit;
	}

	void RigidBody::SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque, ForceType _type) noexcept
	{
		switch (_type)
		{
			case Force:
			{
				linearAcceleration = _force * massData.invMass;
				angularAcceleration = massData.invInerita * _torque;
			}
			break;

			case Impulse:
			{
				linearVelocity = _force * massData.invMass;
				angularVelocity = massData.invInerita * _torque;
			}
			break;

			case Accelration:
			{
				linearAcceleration = _force;
				angularAcceleration = _torque;
			}
			break;

			case Velocity_Change:
			{
				linearVelocity = _force;
				angularVelocity = _torque;
			}
			break;

			default:
			{
				assert(0);
			}
			break;
		}
	}

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ForceType _type) noexcept
	{
		switch (_type)
		{
			case Force:
			{
				linearAcceleration += _force * massData.invMass;
			}
			break;

			case Impulse:
			{
				linearVelocity += _force * massData.invMass;
			}
			break;

			case Accelration:
			{
				linearAcceleration = _force;
			}
			break;

			case Velocity_Change:
			{
				linearVelocity = _force;
			}
			break;

			default:
			{
				assert(0);
			}
			break;
		}
	}

	void RigidBody::ClearForce() noexcept
	{
		linearAcceleration = Eigen::Vector3f::Zero();
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ForceType _type) noexcept
	{
		switch (_type)
		{
			case Force:
			{
				angularAcceleration += massData.invInerita * _torque;
			}
			break;

			case Impulse:
			{
				angularVelocity += massData.invInerita * _torque;
			}
			break;

			case Accelration:
			{
				angularAcceleration += _torque;
			}
			break;

			case Velocity_Change:
			{
				angularVelocity += _torque;
			}
			break;

			default:
			{
				assert(0);
			}
			break;
		}
	}

	void RigidBody::ClearTorque() noexcept
	{
		angularAcceleration = Eigen::Vector3f::Zero();
	}
}
