#include "PlayerController.h"


namespace Phyzzle
{
	void PlayerController::Move(const Eigen::Vector3f& _dir, float _speed)
	{
		const Eigen::Vector3f movement = _dir * _speed;
	}

	void PlayerController::LookTo(const Eigen::Vector3f& _worldDirection)
	{
		assert(!_worldDirection.isZero());

		// �÷��̾��� ���� ȸ���� ����
		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();

		// Model�� ���� ������ ����.
		const Eigen::Vector3f localDirection = parentWorld.conjugate() * _worldDirection;

		// ���� Z�� Local Direction ������ ���ʹϾ��� ����.
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localDirection);

		// Model�� ȸ����Ŵ
		model->SetLocalRotation(targetRotation);
	}

	void PlayerController::LookAt(const Eigen::Vector3f& _worldPosition)
	{
		const Eigen::Vector3f position = gameObject->GetTransform()->GetWorldPosition();

		Eigen::Vector3f to = _worldPosition - position;
		to.normalize();

		LookTo(to);
	}

	void PlayerController::Jump()
	{
		if (!CanJump())
			return;


	}

	void PlayerController::SetSlope(float _angle)
	{
		slope = _angle;
	}

	bool PlayerController::CanJump()
	{
		return !jumping;
	}

	bool PlayerController::IsGround()
	{
		return true;
	}

	bool PlayerController::IsWall()
	{

		return true;
	}

	bool PlayerController::IsObstacle()
	{
		return true;
	}
}
