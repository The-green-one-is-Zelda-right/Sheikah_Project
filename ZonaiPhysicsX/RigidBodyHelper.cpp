#include <PxRigidDynamic.h>
#include <extensions/PxRigidBodyExt.h>

#include "ZnUtil.h"
#include "RigidBody.h"

#include "RigidBodyHelper.h"

namespace ZonaiPhysics
{
	void RigidBodyHelper::Detach(void* _pxBody, void* _pxShape)
	{
		assert(_pxBody != nullptr);
		assert(_pxShape != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		const auto pxShape = static_cast<physx::PxShape*>(_pxShape);

		pxBody->detachShape(*pxShape);
	}

	void RigidBodyHelper::Attach(void* _pxBody, void* _pxShape)
	{
		assert(_pxBody != nullptr);
		assert(_pxShape != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		const auto pxShape = static_cast<physx::PxShape*>(_pxShape);

		pxBody->attachShape(*pxShape);
	}

	void RigidBodyHelper::Release(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->release();
	}

	void RigidBodyHelper::WakeUp(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->wakeUp();
	}

	bool RigidBodyHelper::IsSleeping(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->isSleeping();
	}

	void RigidBodyHelper::UseGravity(void* _pxBody, bool _value)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !_value);
	}

	void RigidBodyHelper::SetKinematic(void* _pxBody, bool _value)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _value);
	}

	void RigidBodyHelper::UpdateInertiaTensor(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		physx::PxRigidBodyExt::updateMassAndInertia(*static_cast<physx::PxRigidDynamic*>(_pxBody), 1.f);
	}

	void RigidBodyHelper::Disable(void* _pxBody, bool _value)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !_value);
	}

	uint8_t RigidBodyHelper::GetDynamicLockFlags(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->getRigidDynamicLockFlags();
	}

	void RigidBodyHelper::SetDynamicLockFlag(void* _pxBody, uint8_t _flag, bool _value)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setRigidDynamicLockFlag(
			static_cast<physx::PxRigidDynamicLockFlag::Enum>(_flag),
			_value
		);
	}

	void RigidBodyHelper::SetDynamicLockFlags(void* _pxBody, uint8_t _flags)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setRigidDynamicLockFlags(
			static_cast<physx::PxRigidDynamicLockFlags>(_flags)
		);
	}

	void* RigidBodyHelper::GetUserData(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->userData;
	}

	void RigidBodyHelper::SetUserData(void* _pxBody, void* _userData)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->userData = _userData;
	}

	float RigidBodyHelper::GetMass(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->getMass();
	}

	void RigidBodyHelper::SetMass(void* _pxBody, float _mass)
	{
		assert(_pxBody != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		pxBody->setMass(_mass);
		physx::PxRigidBodyExt::setMassAndUpdateInertia(*pxBody, _mass);
	}

	float RigidBodyHelper::GetInvMass(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->getInvMass();
	}

	Eigen::Vector3f RigidBodyHelper::GetInertiaTensor(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_pxBody)->getMassSpaceInertiaTensor());
	}

	void RigidBodyHelper::SetInertiaTensor(void* _pxBody, const Eigen::Vector3f& _tensor)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setMassSpaceInertiaTensor(EigenToPhysx(_tensor));
	}

	float RigidBodyHelper::GetLinearDamping(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->getLinearDamping();
	}

	void RigidBodyHelper::SetLinearDamping(void* _pxBody, float _damping)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setLinearDamping(_damping);
	}

	float RigidBodyHelper::GetAngularDamping(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->getAngularDamping();
	}

	void RigidBodyHelper::SetAngularDamping(void* _pxBody, float _damping)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setAngularDamping(_damping);
	}

	Eigen::Vector3f RigidBodyHelper::GetLinearVelocity(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_pxBody)->getLinearVelocity());
	}

	void RigidBodyHelper::SetLinearVelocity(void* _pxBody, const Eigen::Vector3f& _velo)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setLinearVelocity(EigenToPhysx(_velo));
	}

	Eigen::Vector3f RigidBodyHelper::GetAngularVelocity(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_pxBody)->getAngularVelocity());
	}

	void RigidBodyHelper::SetAngularVelocity(void* _pxBody, const Eigen::Vector3f& _velo)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setAngularVelocity(EigenToPhysx(_velo));
	}

	float RigidBodyHelper::GetMaxLinearVelocity(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->getMaxLinearVelocity();
	}

	void RigidBodyHelper::SetMaxLinearVelocity(void* _pxBody, const float& _velo)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setMaxLinearVelocity(_velo);
	}

	float RigidBodyHelper::GetMaxAngularVelocity(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return static_cast<physx::PxRigidDynamic*>(_pxBody)->getMaxAngularVelocity();
	}

	void RigidBodyHelper::SetMaxAngularVelocity(void* _pxBody, const float& _velo)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setMaxAngularVelocity(_velo);
	}

	Eigen::Vector3f RigidBodyHelper::GetPosition(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_pxBody)->getGlobalPose().p);
	}

	void RigidBodyHelper::SetPosition(void* _pxBody, const Eigen::Vector3f& _pos)
	{
		assert(_pxBody != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		physx::PxTransform t = pxBody->getGlobalPose();
		t.p = EigenToPhysx(_pos);
		pxBody->setGlobalPose(t);
	}

	Eigen::Quaternionf RigidBodyHelper::GetQuaternion(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_pxBody)->getGlobalPose().q);
	}

	void RigidBodyHelper::SetQuaternion(void* _pxBody, const Eigen::Quaternionf& _quat)
	{
		assert(_pxBody != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		physx::PxTransform t = pxBody->getGlobalPose();
		t.q = EigenToPhysx(_quat);
		pxBody->setGlobalPose(t);
	}

	void RigidBodyHelper::SetForceAndTorque(void* _pxBody, const Eigen::Vector3f& _force,
		const Eigen::Vector3f& _torque, ForceType _type)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setForceAndTorque(
			EigenToPhysx(_force),
			EigenToPhysx(_torque),
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBodyHelper::AddForce(void* _pxBody, const Eigen::Vector3f& _force, ForceType _type)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->addForce(
			EigenToPhysx(_force),
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBodyHelper::ClearForce(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->clearForce();
	}

	void RigidBodyHelper::AddTorque(void* _pxBody, const Eigen::Vector3f& _torque, ForceType _type)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->addTorque(
			EigenToPhysx(_torque),
			static_cast<physx::PxForceMode::Enum>(_type)
		);
	}

	void RigidBodyHelper::ClearTorque(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->clearTorque();
	}
}
