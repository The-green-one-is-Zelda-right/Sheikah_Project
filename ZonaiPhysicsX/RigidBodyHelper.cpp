#include <PxPhysicsAPI.h>
#include <extensions/PxRigidBodyExt.h>

#include "ZnUtil.h"
#include "ZnBound3.h"
#include "Collider.h"

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

	bool RigidBodyHelper::Attach(void* _pxBody, void* _pxShape)
	{
		assert(_pxBody != nullptr);
		assert(_pxShape != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		const auto pxShape = static_cast<physx::PxShape*>(_pxShape);
		
		bool result = pxBody->attachShape(*pxShape);
		
		return result;
	}

	void RigidBodyHelper::Release(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic**>(_pxBody);
		(*pxBody)->release();
		*pxBody = nullptr;
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

	bool RigidBodyHelper::HasGravity(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		const auto body = static_cast<physx::PxRigidDynamic*>(_pxBody);

		const auto flag = body->getActorFlags();

		return !(flag & physx::PxActorFlag::eDISABLE_GRAVITY);
	}

	void RigidBodyHelper::SetKinematic(void* _pxBody, bool _value)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _value);
	}

	bool RigidBodyHelper::IsKinematic(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		const auto body = static_cast<physx::PxRigidDynamic*>(_pxBody);

		const auto flag = body->getRigidBodyFlags();

		return flag & physx::PxRigidBodyFlag::eKINEMATIC;
	}

	void RigidBodyHelper::UpdateInertiaTensor(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		const auto body = static_cast<physx::PxRigidDynamic*>(_pxBody);
		
		physx::PxRigidBodyExt::updateMassAndInertia(*body, 1.f);
	}

	void RigidBodyHelper::Disable(void* _pxBody, bool _value)
	{
		assert(_pxBody != nullptr);

		static_cast<physx::PxRigidDynamic*>(_pxBody)->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, _value);
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

	void RigidBodyHelper::SetDensity(void* _pxBody, float _density)
	{
		assert(_pxBody != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		physx::PxRigidBodyExt::updateMassAndInertia(*pxBody, _density);
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

	void RigidBodyHelper::SetPosition(void* _pxBody, const Eigen::Vector3f& _pos, bool _wakeup)
	{
		assert(_pxBody != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		physx::PxTransform t = pxBody->getGlobalPose();
		t.p = EigenToPhysx(_pos);
		pxBody->setGlobalPose(t, _wakeup);
	}

	Eigen::Quaternionf RigidBodyHelper::GetQuaternion(void* _pxBody)
	{
		assert(_pxBody != nullptr);

		return PhysxToEigen(static_cast<physx::PxRigidDynamic*>(_pxBody)->getGlobalPose().q);
	}

	void RigidBodyHelper::SetQuaternion(void* _pxBody, const Eigen::Quaternionf& _quat, bool _wakeup)
	{
		assert(_pxBody != nullptr);

		const auto pxBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		physx::PxTransform t = pxBody->getGlobalPose();
		t.q = EigenToPhysx(_quat);
		pxBody->setGlobalPose(t, _wakeup);
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

	ZnBound3 RigidBodyHelper::GetBoundingBox(void* _pxBody, const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot)
	{
		using namespace physx;
		using namespace Eigen;

		PxTransform baseTransform(EigenToPhysx(_pos), EigenToPhysx(_rot));

		PxRigidDynamic* const pxRigidBody = static_cast<physx::PxRigidDynamic*>(_pxBody);
		const PxTransform bodyGlobalPose = pxRigidBody->getGlobalPose();

		PxU32 shapeCount = pxRigidBody->getNbShapes();
		std::vector<PxShape*> shapes(shapeCount);
		pxRigidBody->getShapes(shapes.data(), shapeCount);

		Vector3f globalMinBounds(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3f globalMaxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (PxU32 i = 0; i < shapeCount; ++i)
		{
			PxShape* const shape = shapes[i];

			const PxTransform localShapePose = shape->getLocalPose();
			const PxTransform shapeGlobalPose = bodyGlobalPose * localShapePose;
			const PxTransform finalShapePose = baseTransform * shapeGlobalPose;

			Vector3f shapeWorldPos = PhysxToEigen(finalShapePose.p);
			Quaternionf shapeWorldRot = PhysxToEigen(finalShapePose.q);

			Collider* collider = static_cast<Collider*>(shape->userData);
			ZnBound3 shapeBound = collider->GetBoundingBox(shapeWorldPos, shapeWorldRot);

			globalMinBounds = globalMinBounds.cwiseMin(shapeBound.minimum);
			globalMaxBounds = globalMaxBounds.cwiseMax(shapeBound.maximum);
		}

		// 최종 경계 상자 반환
		return ZnBound3(globalMinBounds, globalMaxBounds);
	}

	ZnBound3 RigidBodyHelper::ComputeBoundingBoxAtTransform(void* _pxBody, const Eigen::Vector3f& _targetPos, const Eigen::Quaternionf& _targetRot)
	{
		using namespace physx;
		using namespace Eigen;

		PxTransform baseTransform(EigenToPhysx(_targetPos), EigenToPhysx(_targetRot));

		PxRigidDynamic* const pxRigidBody = static_cast<physx::PxRigidDynamic*>(_pxBody);

		PxU32 shapeCount = pxRigidBody->getNbShapes();
		std::vector<PxShape*> shapes(shapeCount);
		pxRigidBody->getShapes(shapes.data(), shapeCount);

		Vector3f globalMinBounds(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3f globalMaxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (PxU32 i = 0; i < shapeCount; ++i)
		{
			PxShape* const shape = shapes[i];

			const PxTransform localShapePose = shape->getLocalPose();
			const PxTransform finalShapePose = baseTransform * localShapePose;

			Vector3f shapeWorldPos = PhysxToEigen(finalShapePose.p);
			Quaternionf shapeWorldRot = PhysxToEigen(finalShapePose.q);

			Collider* collider = static_cast<Collider*>(shape->userData);
			ZnBound3 shapeBound = collider->GetBoundingBox(shapeWorldPos, shapeWorldRot);

			globalMinBounds = globalMinBounds.cwiseMin(shapeBound.minimum);
			globalMaxBounds = globalMaxBounds.cwiseMax(shapeBound.maximum);
		}

		return ZnBound3(globalMinBounds, globalMaxBounds);
	}
}
