#include "RigidBody.h"

#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Transform.h"

namespace PurahEngine
{
	RigidBody::RigidBody()
	{

	}

	RigidBody::~RigidBody()
	{

	}

	void RigidBody::Awake()
	{
		auto name = GetGameObject()->GetName();
		body = PhysicsSystem::GetInstance().CreateRigidBody(name);
		PhysicsSystem::GetInstance().bodies.push_back(this);
		awake = false;
		auto trans = GetGameObject()->GetComponent<Transform>();

		body->SetPosition(trans->GetWorldPosition());
		body->SetQuaternion(trans->GetWorldRotation());
		SetKinematic(isKinematic);
	}

	/// \brief 스크립트에서 호출하지 말아라
	void RigidBody::WakeUp() noexcept
	{
		body->WakeUp();
	}

	/// 스크립트에서 호출하지 말아라
	bool RigidBody::IsSleeping() const noexcept
	{
		return body->IsSleeping();
	}

	/// 
	ZonaiPhysics::DynamicLocks RigidBody::GetDynamicLockFlags() const noexcept
	{
		return body->GetDynamicLockFlags();
	}

	void RigidBody::SetDynamicLockFlag(ZonaiPhysics::DynamicLock flag, bool value) noexcept
	{
		body->SetDynamicLockFlag(flag, value);
	}

	void RigidBody::SetDynamicLockFlags(ZonaiPhysics::DynamicLocks flags) noexcept
	{
		body->SetDynamicLockFlags(flags);
	}

	float RigidBody::GetMass() const noexcept
	{
		if (awake)
		{
			return mass;
		}
		else
		{
			return  body->GetMass();
		}
	}

	void RigidBody::SetMass(float _mass) noexcept
	{
		if (awake)
		{
			mass = _mass;
		}
		else
		{
			body->SetMass(_mass);
		}
	}

	float RigidBody::GetLinearDamping() const noexcept
	{
		if (awake)
		{
			return linearDamping;
		}
		else
		{
			return body->GetLinearDamping();
		}
	}

	void RigidBody::SetLinearDamping(float _damping) noexcept
	{
		if (awake)
		{
			linearDamping = _damping;
		}
		else
		{
			body->SetLinearDamping(_damping);
		}
	}

	float RigidBody::GetAngularDamping() const noexcept
	{
		if (awake)
		{
			return angularDamping;
		}
		else
		{
			return body->GetAngularDamping();
		}
	}

	void RigidBody::SetAngularDamping(float _damping) noexcept
	{
		if (awake)
		{
			angularDamping = _damping;
		}
		else
		{
			body->SetAngularDamping(_damping);
		}
	}

	Eigen::Vector3f RigidBody::GetLinearVelocity() const noexcept
	{
		return body->GetLinearVelocity();
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		body->SetLinearVelocity(_velocity);
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		return body->GetAngularVelocity();
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		body->SetAngularVelocity(_velocity);
	}

// 	float RigidBody::GetMaxLinearVelocity() const noexcept
// 	{
// 		return body->GetMaxLinearVelocity();
// 	}
// 
// 	void RigidBody::SetMaxLinearVelocity(const float& _velo) noexcept
// 	{
// 		body->SetMaxLinearVelocity(_velo);
// 	}
// 
// 	float RigidBody::GetMaxAngularVelocity() const noexcept
// 	{
// 		return body->GetMaxAngularVelocity();
// 	}
// 
// 	void RigidBody::SetMaxAngularVelocity(const float& _velo) noexcept
// 	{
// 		body->SetMaxAngularVelocity(_velo);
// 	}

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ZonaiPhysics::ForceType _type) noexcept
	{
		if (awake)
		{
			force = _force;
		}
		else
		{
			body->AddForce(_force, _type);
		}
	}

	void RigidBody::ClearForce() noexcept
	{
		body->ClearForce();
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ZonaiPhysics::ForceType _type) noexcept
	{
		if (awake)
		{
			torque = _torque;
		}
		else
		{
			body->AddTorque(_torque, _type);
		}
	}

	void RigidBody::ClearTorque() noexcept
	{
		body->ClearTorque();
	}

	void RigidBody::SetKinematic(bool value) noexcept
	{
		if (awake)
		{
			isKinematic = value;
		}
		else
		{
			body->SetKinematic(value);
		}
	}

	void RigidBody::SimulateResult()
	{
		auto pos = body->GetPosition();
		auto rot = body->GetQuaternion();

		auto transform = GetGameObject()->GetComponent<Transform>();
		transform->SetLocalPosition(pos);
		transform->SetLocalRotation(rot);
	}
}
