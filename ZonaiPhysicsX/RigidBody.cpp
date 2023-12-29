#include <Eigen/Dense>

#include "PxPhysicsAPI.h"
#include "RigidBody.h"

namespace ZonaiPhysics
{
	RigidBody::RigidBody() noexcept
	{

	}

	RigidBody::RigidBody(physx::PxPhysics*& _factory) noexcept
	{
		using namespace physx;
		rigidbody_ = _factory->createRigidDynamic(PxTransform(PxVec3(0,0,0)));
		rigidbody_->userData = this;
	}

	RigidBody::~RigidBody() noexcept
	{
		rigidbody_->release();
	}

	void RigidBody::WakeUp() noexcept
	{
		rigidbody_->wakeUp();
	}

	bool RigidBody::IsSleeping() const noexcept
	{
		return rigidbody_->isSleeping();
	}

	void RigidBody::UseGravity(bool value) noexcept
	{
		using namespace physx;
		rigidbody_->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !value);
	}

	void RigidBody::SetKinematic(bool value) noexcept
	{
		using namespace physx;
		rigidbody_->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, value);
	}

	void RigidBody::CanSimulate(bool value) const noexcept
	{
		using namespace physx;
		rigidbody_->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !value);
	}

	DynamicLocks RigidBody::GetDynamicLockFlags() const noexcept
	{
		return static_cast<uint8_t>(rigidbody_->getRigidDynamicLockFlags());
	}

	void RigidBody::SetDynamicLockFlag(DynamicLock _flag, bool _value) noexcept
	{
		using namespace physx;
		rigidbody_->setRigidDynamicLockFlag(static_cast<PxRigidDynamicLockFlag::Enum>(_flag), _value);
	}

	void RigidBody::SetDynamicLockFlags(DynamicLocks _flags) noexcept
	{
		using namespace physx;
		rigidbody_->setRigidDynamicLockFlags(static_cast<PxRigidDynamicLockFlags>(_flags));
	}

	void* RigidBody::GetUserData() const noexcept
	{
		return this->userData;
	}

	void RigidBody::SetUserData(void* _userData) noexcept
	{
		this->userData = _userData;
	}

	float RigidBody::GetMass() const noexcept
	{
		return rigidbody_->getMass();
	}

	void RigidBody::SetMass(float _mass) noexcept
	{
		rigidbody_->setMass(_mass);
	}

	float RigidBody::GetInvMass() const noexcept
	{
		return rigidbody_->getInvMass();
	}

	Eigen::Vector3f RigidBody::GetInertiaTensor() const noexcept
	{
		using namespace physx;
		PxVec3 I = rigidbody_->getMassSpaceInertiaTensor();
		return { I.x, I.y, I.z };
	}

	void RigidBody::SetInertiaTensor(const Eigen::Vector3f& _I) noexcept
	{
		using namespace physx;
		rigidbody_->setMassSpaceInertiaTensor(PxVec3(_I.x(), _I.y(), _I.z()));
	}

	float RigidBody::GetLinearDamping() const noexcept
	{
		return rigidbody_->getLinearDamping();
	}

	void RigidBody::SetLinearDamping(float _damping) noexcept
	{
		rigidbody_->setLinearDamping(_damping);
	}

	float RigidBody::GetAngularDamping() const noexcept
	{
		return rigidbody_->getAngularDamping();
	}

	void RigidBody::SetAngularDamping(float _damping) noexcept
	{
		rigidbody_->setAngularDamping(_damping);
	}

	Eigen::Vector3f RigidBody::GetLinearVelocity() const noexcept
	{
		using namespace physx;
		PxVec3 v = rigidbody_->getLinearVelocity();
		return { v.x, v.y, v.z };
	}

	void RigidBody::SetLinearVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		using namespace physx;
		rigidbody_->setLinearVelocity(PxVec3(_velocity.x(), _velocity.y(), _velocity.z()));
	}

	Eigen::Vector3f RigidBody::GetAngularVelocity() const noexcept
	{
		using namespace physx;
		PxVec3 v = rigidbody_->getAngularVelocity();
		return { v.x, v.y, v.z };
	}

	void RigidBody::SetAngularVelocity(const Eigen::Vector3f& _velocity) noexcept
	{
		using namespace physx;
		rigidbody_->setAngularVelocity(PxVec3(_velocity.x(), _velocity.y(), _velocity.z()));
	}

	float RigidBody::GetMaxLinearVelocity() const noexcept
	{
		return rigidbody_->getMaxLinearVelocity();
	}

	void RigidBody::SetMaxLinearVelocity(const float& _maxVelocity) noexcept
	{
		rigidbody_->setMaxLinearVelocity(_maxVelocity);
	}

	float RigidBody::GetMaxAngularVelocity() const noexcept
	{
		return rigidbody_->getMaxAngularVelocity();
	}

	void RigidBody::SetMaxAngularVelocity(const float& _maxVelocity) noexcept
	{
		rigidbody_->setMaxAngularVelocity(_maxVelocity);
	}

	Eigen::Vector3f RigidBody::GetPosition() const noexcept
	{
		auto transform = rigidbody_->getGlobalPose();
		return { transform.p.x, transform.p.y, transform.p.z };
	}

	void RigidBody::SetPosition(const Eigen::Vector3f& _position) noexcept
	{
		using namespace physx;
		PxTransform t = rigidbody_->getGlobalPose();
		t.p.x = _position.x();
		t.p.y = _position.y();
		t.p.z = _position.z();
		rigidbody_->setGlobalPose(t);
	}

	Eigen::Quaternionf RigidBody::GetQuaternion() const noexcept
	{
		using namespace physx;
		PxTransform t = rigidbody_->getGlobalPose();
		return { t.q.w, t.q.x, t.q.y, t.q.z };
	}

	void RigidBody::SetQuaternion(const Eigen::Quaternionf& _quaternion) noexcept
	{
		using namespace physx;
		PxTransform t = rigidbody_->getGlobalPose();
		t.q.w = _quaternion.w();
		t.q.x = _quaternion.x();
		t.q.y = _quaternion.y();
		t.q.z = _quaternion.z();
		rigidbody_->setGlobalPose(t);
	}

	void RigidBody::SetForceAndTorque(const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque,
		ForceType _type) noexcept
	{
		using namespace physx;
		rigidbody_->setForceAndTorque(
			{ _force.x(), _force.y(), _force.z()},
			{ _torque.x(), _torque.y(), _torque.z()},
			static_cast<PxForceMode::Enum>(_type)
		);
	}

	void RigidBody::AddForce(const Eigen::Vector3f& _force, ForceType _type) noexcept
	{
		using namespace physx;
		rigidbody_->addForce(PxVec3(_force.x(), _force.y(), _force.z()), static_cast<PxForceMode::Enum>(_type));
	}

	void RigidBody::ClearForce() noexcept
	{
		rigidbody_->clearForce();
	}

	void RigidBody::AddTorque(const Eigen::Vector3f& _torque, ForceType _type) noexcept
	{
		using namespace physx;
		rigidbody_->addForce(PxVec3(_torque.x(), _torque.y(), _torque.z()), static_cast<PxForceMode::Enum>(_type));
	}

	void RigidBody::ClearTorque() noexcept
	{
		rigidbody_->clearTorque();
	}

	physx::PxRigidDynamic* RigidBody::getRigidDynamic() const noexcept
	{
		return rigidbody_;
	}
} // namespace ZonaiPhysics