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
		auto& instance = PhysicsSystem::GetInstance();

		GetGameObject()->GetTransform()->SetRigidBody(nullptr);
		instance.FreeObject(body, GetGameObject());
		instance.bodies.erase(std::ranges::find(instance.bodies, this));
	}

	void RigidBody::Initialize()
	{
		body = PhysicsSystem::GetInstance().CreateRigidBody(GetGameObject());
		PhysicsSystem::GetInstance().bodies.push_back(this);
		// awake = false;
	}

	void RigidBody::OnDataLoadComplete()
	{
		const auto trans = GetGameObject()->GetTransform();
		body->SetPosition(trans->GetWorldPosition());
		body->SetQuaternion(trans->GetWorldRotation());
		trans->SetRigidBody(this);

		this->SetDynamicLockFlags(freeze);
		this->SetKinematic(isKinematic);
		this->UseGravity(useGravity);
		// SetLinearVelocity(LinearVelocity);
		// SetAngularVelocity(angularVelocity);
		this->SetMass(mass);
		this->SetLinearDamping(linearDamping);
		this->SetAngularDamping(angularDamping);
		this->WakeUp();
		// AddForce(force);
		// AddTorque(torque);
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _pos) noexcept
	{
		// if (awake)
		// {
		// 	auto trans = GetGameObject()->GetTransform();
		// 	trans->SetWorldPosition(_pos);
		// }
		// else
		// {
			body->SetPosition(_pos);
		// }
	}

	const Eigen::Vector3f& RigidBody::GetPosition() noexcept
	{
		// if (awake)
		// {
		// 	auto trans = GetGameObject()->GetTransform();
		// 	return trans->GetWorldPosition();
		// }
		// else
		// {
			return body->GetPosition();
		// }
	}

	void RigidBody::SetRotation(const Eigen::Quaternionf& _rot) noexcept
	{
		// if (awake)
		// {
		// 	auto trans = GetGameObject()->GetTransform();
		// 	trans->SetWorldRotation(_rot);
		// }
		// else
		// {
			body->SetQuaternion(_rot);
		// }
	}

	const Eigen::Quaternionf& RigidBody::GetRotation() noexcept
	{
		// if (awake)
		// {
		// 	auto trans = GetGameObject()->GetTransform();
		// 	return trans->GetWorldRotation();
		// }
		// else
		// {
			return body->GetQuaternion();
		// }
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
		// if (awake)
		// {
		// 	return freeze;
		// }
		// else
		// {
			return body->GetDynamicLockFlags();
		// }
	}

	void RigidBody::SetDynamicLockFlag(ZonaiPhysics::FreezeFlag flag, bool value) noexcept
	{
		// if (awake)
		// {
		// 	value ? freeze |= flag : freeze &= flag;
		// }
		// else
		// {
			body->SetDynamicLockFlag(flag, value);
		// }
	}

	void RigidBody::SetDynamicLockFlags(uint8_t flags) noexcept
	{
		// if (awake)
		// {
		// 	freeze = flags;
		// }
		// else
		// {
			body->SetDynamicLockFlags(flags);
		// }
	}

	float RigidBody::GetMass() const noexcept
	{
		// if (awake)
		// {
		// 	return mass;
		// }
		// else
		// {
			return  body->GetMass();
		// }
	}

	void RigidBody::SetMass(float _mass) noexcept
	{
		// if (awake)
		// {
		// 	mass = _mass;
		// }
		// else
		// {
			body->SetMass(_mass);
		// }
	}

	float RigidBody::GetLinearDamping() const noexcept
	{
		// if (awake)
		// {
		// 	return linearDamping;
		// }
		// else
		// {
			return body->GetLinearDamping();
		// }
	}

	void RigidBody::SetLinearDamping(float _damping) noexcept
	{
		// if (awake)
		// {
		// 	linearDamping = _damping;
		// }
		// else
		// {
			body->SetLinearDamping(_damping);
		// }
	}

	float RigidBody::GetAngularDamping() const noexcept
	{
		// if (awake)
		// {
		// 	return angularDamping;
		// }
		// else
		// {
			return body->GetAngularDamping();
		// }
	}

	void RigidBody::SetAngularDamping(float _damping) noexcept
	{
		// if (awake)
		// {
		// 	angularDamping = _damping;
		// }
		// else
		// {
			body->SetAngularDamping(_damping);
		// }
	}

	Eigen::Vector3f RigidBody::GetLinearVelocity() const noexcept
	{
		// if (awake)
		// {
		// 	return linearVelocity;
		// }
		// else
		// {
			return body->GetLinearVelocity();
		// }
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		// if (awake)
		// {
		// 	linearVelocity = _velocity;
		// }
		// else
		// {
			body->SetLinearVelocity(_velocity);
		// }
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		// if (awake)
		// {
		// 	return angularVelocity;
		// }
		// else
		// {
			return body->GetAngularVelocity();
		// }
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		// if (awake)
		// {
		// 	angularVelocity = _velocity;
		// }
		// else
		// {
			body->SetAngularVelocity(_velocity);
		// }
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
		// if (awake)
		// {
		// 	force = _force;
		// }
		// else
		// {
			body->AddForce(_force, _type);
		// }
	}

	void RigidBody::ClearForce() noexcept
	{
		body->ClearForce();
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ZonaiPhysics::ForceType _type) noexcept
	{
		// if (awake)
		// {
		// 	torque = _torque;
		// }
		// else
		// {
			body->AddTorque(_torque, _type);
		// }
	}

	void RigidBody::ClearTorque() noexcept
	{
		body->ClearTorque();
	}

	void RigidBody::SetKinematic(bool value) noexcept
	{
		// if (awake)
		// {
		// 	isKinematic = value;
		// }
		// else
		// {
			body->SetKinematic(value);
		// }
	}

	void RigidBody::UseGravity(bool _value)
	{
		// if (awake)
		// {
		// 	useGravity = _value;
		// }
		// else
		// {
			body->UseGravity(_value);
		// }
	}

	void RigidBody::SimulateResult()
	{
		auto pos = body->GetPosition();
		auto rot = body->GetQuaternion();

		const auto transform = GetGameObject()->GetTransform();
		transform->SetWorldRotation(rot);
		transform->SetWorldPosition(pos);
	}

	void RigidBody::PreSerialize(json& jsonData) const
	{
		
	}

	void RigidBody::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(isKinematic);
		PREDESERIALIZE_VALUE(useGravity);

		int dynamicLock = 0;
		PREDESERIALIZE_VALUE(dynamicLock);
		this->freeze = (uint8_t)dynamicLock;

		PREDESERIALIZE_VALUE(mass);
		PREDESERIALIZE_VALUE(linearDamping);
		PREDESERIALIZE_VALUE(angularDamping);
	}

	void RigidBody::PostSerialize(json& jsonData) const
	{

	}

	void RigidBody::PostDeserialize(const json& jsonData)
	{

	}

}
