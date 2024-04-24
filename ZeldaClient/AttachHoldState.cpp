#include "AttachHoldState.h"

namespace Phyzzle
{
	AttachHoldState::~AttachHoldState()
	= default;

#pragma region StateEvent
	void AttachHoldState::StateEnter()
	{
		if (!TrySelect())
		{
			player->ChangeState(Player::ATTACH_SELECT);
		}
		else
		{
			VariableSet();
		}
	}

	void AttachHoldState::StateExit()
	{
		VariableReset();
	}

	void AttachHoldState::StateStay()
	{
		ApplyObjectVelocity();
		ResetObjectVelocity();
	}
#pragma endregion StateEvent

#pragma region Input
	// �÷��̾� �̵�
	void AttachHoldState::Stick_L()
	{
		//case HOLD:
		//case ROTATE:
		PlayerMove(player->data.moveSpeed * 0.7f);
		// ������Ʈ �������� ĳ���͸� ȸ�� ��Ŵ
		Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
		direction.y() = 0.f;
		LookToLocalDirection(direction);

		// �÷��̾� �̵� ����� ���� �������� ���� ��
		ObjectTranslate(player->data.playerRigidbody->GetLinearVelocity(), 1.f);
	}

	// ������Ʈ �̵�
	void AttachHoldState::Stick_R()
	{
		// Stick L�� �÷��̾��� ������
		// Stick R�� D Pad�� ���� ������Ʈ�� ������
		//
		// ������Ʈ ���� ������Ʈ�� �÷��̾��� ����� �Ÿ��� ȸ���� �ٽ� ����� ��.
		// ������Ʈ ���� �� ��ġ�� �������� üũ�ϰ� �ƴϸ� �ɷ��� �����.
		//
		// Stick L�� �÷��̾ ������ ��, ��ü���� ���� ���� ��� ��.
		//
		// ��ü�� ������ ���¶��
		// �¿� �Է��� '�÷��̾ ���� �ִ� ����'�� �¿�� ���� �ְ�
		// ���� �Է��� ���� Up, Down �������� ���� �ִ� ������ ����.
		// �÷��̾ �׻� ��ü�� �ٶ󺸰� �ִ� ������ �ϸ� ���� ����ϰ� ���� ���̴�.

		// Ű �Է��� ������ �� ����
		if (player->currInput.Rstick.Size)
		{
			const Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
			const Eigen::Vector3f objectPos = selectBody->GetPosition();
			Eigen::Vector3f forward = objectPos - playerPos;
			const float distance = forward.y();
			forward.y() = 0.f;
			forward.normalize();
			const Eigen::Vector3f right = Eigen::Vector3f::UnitY().cross(forward);
			const Eigen::Vector3f rightVelocity = right * player->currInput.Rstick.X;
			Eigen::Vector3f upVelocity = Eigen::Vector3f::Zero();

			if (player->currInput.Rstick.Y > 0.f && distance < 10.f)
			{
				upVelocity = Eigen::Vector3f::UnitY() * player->currInput.Rstick.Y;
			}
			else if (player->currInput.Rstick.Y < 0.f && distance > -5.f)
			{
				upVelocity = Eigen::Vector3f::UnitY() * player->currInput.Rstick.Y;
			}

			Eigen::Vector3f direction = upVelocity + rightVelocity;
			direction.normalize();
			direction *= player->currInput.Rstick.Size;

			ObjectTranslate(direction, 5.f);
		}
	}

	// ���
	void AttachHoldState::Click_A()
	{
		StateCancel();
	}

	// ����
	void AttachHoldState::Click_B()
	{
		// ������Ʈ�� �̵���ų�� �ִ� ���¶��
		// ���� �� �־����.
	}

	// ���
	void AttachHoldState::Click_X()
	{
		StateCancel();
	}

	// ���
	void AttachHoldState::Click_Y()
	{
		StateCancel();
	}

	// ���
	void AttachHoldState::Click_LB()
	{
		StateCancel();
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DUp()
	{
		//case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DDown()
	{
		//case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DLeft()
	{
		//case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DRight()
	{
		//case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Pressing_DUp()
	{
		// �÷��̾� - ������Ʈ �������� �ְ� ����.
		const Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const Eigen::Vector3f objectPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objectPos - playerPos;
		forward.y() = 0.f;
		float distance = forward.norm();

		if (distance > 20.f)
			return;

		forward.normalize();

		ObjectTranslate(forward, 10.f);
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Pressing_DDown()
	{
		Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		Eigen::Vector3f objectPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objectPos - playerPos;
		forward.y() = 0.f;
		float distance = forward.norm();

		if (distance < 3.f)
			return;

		forward.normalize();

		ObjectTranslate(forward, -10.f);
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Pressing_DLeft()
	{
		//case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� -ȸ�� ��Ŵ
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Pressing_DRight()
	{
		//case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
	}

	// Rotate ���
	void AttachHoldState::Pressing_RB()
	{

	}
#pragma endregion Input

#pragma region Content
	void AttachHoldState::PlayerMove(float _speed) const
	{
		player->PlayerMove(_speed);
	}

	void AttachHoldState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttachHoldState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	bool AttachHoldState::TrySelect()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttachHoldState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
		debugVector1 = _to.normalized();

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachHoldState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		debugVector2 = _to.normalized();

		player->LookInLocalDirection(_to.normalized());
	}

	void AttachHoldState::ApplyObjectVelocity() const
	{
		selectBody->SetLinearVelocity(targetVelocity);
	}

	void AttachHoldState::ResetObjectVelocity()
	{
		targetVelocity = Eigen::Vector3f::Zero();
	}

	void AttachHoldState::ObjectTranslate(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachHoldState::XRotate() const
	{

	}

	void AttachHoldState::YRotate() const
	{

	}

	void AttachHoldState::Attach() const
	{

	}

	void AttachHoldState::VariableSet()
	{
		using namespace Eigen;

		if (selectBody == nullptr)
			return;

		hasGravity = selectBody->HasGravity();
		mass = selectBody->GetMass();

		selectBody->UseGravity(false);
		selectBody->SetMass(0.001f);

		const Eigen::Vector3f objectPosition = selectBody->GetPosition();
		const Eigen::Vector3f playerPosition = player->GetGameObject()->GetTransform()->GetWorldPosition();

		const Eigen::Vector3f lookTo = objectPosition - playerPosition;
		LookToWorldDirection(lookTo);

		distance = lookTo.size();
	}

	void AttachHoldState::VariableReset()
	{
		using namespace Eigen;

		if (selectBody)
		{
			selectBody->UseGravity(hasGravity);
			selectBody->SetMass(mass);
		}

		playerVelocity = Eigen::Vector3f::Zero();
		targetVelocity = Eigen::Vector3f::Zero();
		hasGravity = false;
		mass = -0.1f;
		distance = -0.1f;
		selectBody = nullptr;
	}
#pragma endregion Content

#if _DEBUG
	void AttachHoldState::SearchDebugDraw()
	{
		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"player to object : " +
			std::to_wstring(debugVector0.x()) + L" " +
			std::to_wstring(debugVector0.y()) + L" " +
			std::to_wstring(debugVector0.z()) + L" ",
			1200, 100,
			200, 600, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"world direction : " +
			std::to_wstring(debugVector1.x()) + L" " +
			std::to_wstring(debugVector1.y()) + L" " +
			std::to_wstring(debugVector1.z()) + L" ",
			1200, 200,
			200, 600, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"local direction : " +
			std::to_wstring(debugVector2.x()) + L" " +
			std::to_wstring(debugVector2.y()) + L" " +
			std::to_wstring(debugVector2.z()) + L" ",
			1200, 300,
			200, 600, 15,
			255, 255, 255, 255);
	}
#endif _DEBUG
}
