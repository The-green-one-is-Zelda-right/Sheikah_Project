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
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetRigidBody(nullptr);
	}

	void RigidBody::Awake()
	{
		body = PhysicsSystem::GetInstance().CreateRigidBody(GetGameObject());
		PhysicsSystem::GetInstance().bodies.push_back(this);
		awake = false;
		const auto trans = GetGameObject()->GetComponent<Transform>();

		body->SetPosition(trans->GetWorldPosition());
		body->SetQuaternion(trans->GetWorldRotation());
		SetKinematic(isKinematic);
		UseGravity(useGravity);
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetRigidBody(this);
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _pos) noexcept
	{
		if (awake)
		{
			auto trans = GetGameObject()->GetComponent<Transform>();
			trans->SetLocalPosition(_pos);
		}
		else
		{
			body->SetPosition(_pos);
		}
	}

	const Eigen::Vector3f& RigidBody::GetPosition() noexcept
	{
		if (awake)
		{
			auto trans = GetGameObject()->GetComponent<Transform>();
			return trans->GetLocalPosition();
		}
		else
		{
			return body->GetPosition();
		}
	}

	void RigidBody::SetRotation(const Eigen::Quaternionf& _rot) noexcept
	{
		if (awake)
		{
			auto trans = GetGameObject()->GetComponent<Transform>();
			trans->SetLocalRotation(_rot);
		}
		else
		{
			body->SetQuaternion(_rot);
		}
	}

	const Eigen::Quaternionf& RigidBody::GetRotation() noexcept
	{
		if (awake)
		{
			auto trans = GetGameObject()->GetComponent<Transform>();
			return trans->GetLocalRotation();
		}
		else
		{
			return body->GetQuaternion();
		}
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
	uint8_t RigidBody::GetDynamicLockFlags() const noexcept
	{
		return body->GetDynamicLockFlags();
	}

	void RigidBody::SetDynamicLockFlag(ZonaiPhysics::FreezeFlag flag, bool value) noexcept
	{
		body->SetDynamicLockFlag(flag, value);
	}

	void RigidBody::SetDynamicLockFlags(uint8_t flags) noexcept
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
		if (awake)
		{
			return LinearVelocity;
		}
		else
		{
			return body->GetLinearVelocity();
		}
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		if (awake)
		{
			LinearVelocity = _velocity;
		}
		else
		{
			body->SetLinearVelocity(_velocity);
		}
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		if (awake)
		{
			return angularVelocity;
		}
		else
		{
			return body->GetAngularVelocity();
		}
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		if (awake)
		{
			angularVelocity = _velocity;
		}
		else
		{
			body->SetAngularVelocity(_velocity);
		}
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

	void RigidBody::UseGravity(bool _value)
	{
		if (awake)
		{
			useGravity = _value;
		}
		else
		{
			body->UseGravity(_value);
		}
	}

	void RigidBody::SimulateResult()
	{
		auto pos = body->GetPosition();
		auto rot = body->GetQuaternion();

		auto transform = GetGameObject()->GetComponent<Transform>();
		// matrix4f로 만들어서 보내는걸로 하자

		//transform->GetLocalScale();
		//transform->GetWorldScale();
		//Eigen::Matrix4f inverseW = transform->GetWorldMatrix().inverse();



		transform->SetWorldPosition(pos);
		transform->SetWorldRotation(rot);
	}
}
