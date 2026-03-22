#include "PxPhysicsAPI.h"

#include "ZnUtil.h"
#include "CharacterController.h"


namespace ZonaiPhysics
{
	CharacterController::CharacterController(physx::PxController* _controller)
		: controller(_controller), isJumping(false), jumpVelocity(0.0f), pendingMovement(Eigen::Vector3f::Zero())
	{
	}

	void CharacterController::SetCollisonData()
	{
		controller->getActor();
	}

	void CharacterController::SetPosition(const Eigen::Vector3f& _position)
	{
		controller->setPosition({ _position.x(), _position.y(), _position.z() });
	}

	void CharacterController::SetFootPosition(const Eigen::Vector3f& _position)
	{
		controller->setFootPosition({ _position.x(), _position.y(), _position.z() });
	}

	void CharacterController::SetStepOffset(const float _offset)
	{
		controller->setStepOffset(_offset);
	}

	void CharacterController::SetContactOffset(const float _offset)
	{
		controller->setContactOffset(_offset);
	}

	void CharacterController::SetUpDirection(const Eigen::Vector3f& _direction)
	{
		controller->setUpDirection({ _direction.x(), _direction.y(), _direction.z() });
	}

	void CharacterController::SetSlopeLimit(const float _slope)
	{
		controller->setSlopeLimit(_slope);
	}

	Eigen::Vector3f CharacterController::GetPosition()
	{
		return PhysxToEigen(physx::toVec3(controller->getPosition()));
	}

	Eigen::Vector3f CharacterController::GetFootPosition()
	{
		return PhysxToEigen(physx::toVec3(controller->getFootPosition()));
	}

	float CharacterController::GetStepOffset()
	{
		return controller->getStepOffset();
	}

	float CharacterController::GetContactOffset()
	{
		return controller->getContactOffset();
	}

	Eigen::Vector3f CharacterController::GetUpDirection()
	{
		return PhysxToEigen(controller->getUpDirection());
	}

	float CharacterController::GetSlopeLimit()
	{
		return controller->getSlopeLimit();
	}

	void CharacterController::Move(Eigen::Vector3f _vec)
	{
		pendingMovement += _vec;
	}

	void CharacterController::Jump(float _velocity)
	{
		if (!isJumping)
		{
			isJumping = true;
			jumpVelocity = _velocity;
		}
	}

	void CharacterController::ApplyImpulse(const Eigen::Vector3f& _impulse)
	{
		pendingMovement += _impulse;
	}

	void CharacterController::Apply(float _dt)
	{
		if (isJumping)
		{
			// 중력 적용
			jumpVelocity -= 9.8f * _dt;
			pendingMovement.y() += jumpVelocity * _dt;
		}

		// 이동 벡터 적용
		physx::PxControllerFilters filters;
		collisionFlags = controller->move(EigenToPhysx(pendingMovement), 0.001f, _dt, filters);

		// 바닥에 닿았는지 확인
		if (collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			isJumping = false;
			jumpVelocity = 0.0f;
		}

		// 대기 중인 이동 벡터 초기화
		pendingMovement = Eigen::Vector3f::Zero();
	}

	bool CharacterController::IsInAir() const
	{
		return (collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN) == false;
	}
}