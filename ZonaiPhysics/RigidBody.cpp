#include "RigidBody.h"

namespace ZonaiPhysics
{
	Eigen::Vector3f RigidBody::GetPosition() const
	{
		return position;
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _position)
	{
		position = _position;
	}

	Eigen::Quaternionf RigidBody::GetQuaternion() const
	{
		return rotation;
	}

	void RigidBody::SetQuaternion(const Eigen::Quaternionf& _quaternion)
	{
		rotation = _quaternion;
	}

	void* RigidBody::GetUserData() const
	{
		return userData;
	}

	void RigidBody::SetUserData(void* _userData)
	{
		userData = _userData;
	}

	void RigidBody::WakeUp()
	{
		active = true;
	}

	bool RigidBody::IsSleeping() const
	{
		return !active;
	}

	DynamicLocks RigidBody::GetDynamicLockFlags() const
	{
		return lockFlags;
	}

	void RigidBody::SetDynamicLockFlag(DynamicLock _flag, bool _value)
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

	void RigidBody::SetDynamicLockFlags(DynamicLocks _flags)
	{
		lockFlags = _flags;
	}

	float RigidBody::GetMass() const
	{
		return massData.mass;
	}

	void RigidBody::SetMass(float _mass)
	{
		massData.mass = _mass;
	}

	float RigidBody::GetInvMass() const
	{
		return massData.invMass;
	}

	Eigen::Vector3f RigidBody::GetInertiaTensor() const
	{
		return massData.inerita;
	}

	void RigidBody::SetInertiaTensor(const Eigen::Vector3f& _inertia)
	{
		massData.inerita = _inertia;
	}

	float RigidBody::GetLinearDamping() const
	{
		return material.linearDamping;
	}

	void RigidBody::SetLinearDamping(float _damping)
	{
		material.linearDamping = _damping;
	}

	float RigidBody::GetAngularDamping() const
	{
		return material.angularDamping;
	}

	void RigidBody::SetAngularDamping(float _damping)
	{
		material.angularDamping = _damping;
	}

	Vector3f RigidBody::GetLinearVelocity() const
	{
		return linearVelocity;
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity)
	{
		linearVelocity = _velocity;
	}

	Vector3f RigidBody::GetAngularVelocity() const
	{
		return angularVelocity;
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity)
	{
		angularVelocity = _velocity;
	}

	float RigidBody::GetMaxLinearVelocity() const
	{
		return limitLinearVelocity;
	}

	void RigidBody::SetMaxLinearVelocity(const float& _limit)
	{
		limitLinearVelocity = _limit;
	}

	float RigidBody::GetMaxAngularVelocity() const
	{
		return limitAngularVelocity;
	}

	void RigidBody::SetMaxAngularVelocity(const float& _limit)
	{
		limitAngularVelocity = _limit;
	}

	void RigidBody::SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque, ForceType _type)
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

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ForceType _type)
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

	void RigidBody::ClearForce()
	{
		linearAcceleration = Eigen::Vector3f::Zero();
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ForceType _type)
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

	void RigidBody::ClearTorque()
	{
		angularAcceleration = Eigen::Vector3f::Zero();
	}
}
