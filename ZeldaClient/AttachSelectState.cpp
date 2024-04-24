#include "AttachSelectState.h"

namespace Phyzzle
{
	AttachSelectState::~AttachSelectState()
		= default;

	// ���� ������
	void AttachSelectState::StateEnter()
	{
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);

			mode = SELECT;
		}
	}

	// ���� ������
	void AttachSelectState::StateExit()
	{
		// �÷��̾� ī�޶� ������Ʈ�� ����ٸ�
		// �̺κ��� ������ �ݵ�� �ʿ���
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);

			mode = SELECT;
		}

		Reset();
	}

	// Update
	void AttachSelectState::StateStay()
	{
		switch (mode)
		{
		case SELECT:
		{
			SearchDebugDraw(Search());
		}
		break;

		case HOLD:
		case ROTATE:
		{
			ApplyObjectVelocity();
			ResetObjectVelocity();
		}
		break;

		default: ;
		}
	}

#pragma region Ű�Է�
	// �̵�
	void AttachSelectState::Stick_L()
	{
		switch (mode)
		{
		case SELECT:
		{
			// �̵� �������� ĳ���͸� ȸ�� ��Ŵ
			PlayerMove(player->data.moveSpeed);
			auto velocity = player->data.playerRigidbody->GetLinearVelocity();
			velocity.y() = 0.f;
			LookToWorldDirection(velocity);
		}
		break;

		case HOLD:
		case ROTATE:
		{

			PlayerMove(player->data.moveSpeed * 0.7f);
			// ������Ʈ �������� ĳ���͸� ȸ�� ��Ŵ
			Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
			direction.y() = 0.f;
			LookToLocalDirection(direction);

			// �÷��̾� �̵� ����� ���� �������� ���� ��
			ObjectTranslate(player->data.playerRigidbody->GetLinearVelocity(), 1.f);
		}
		break;

		default:;
		}
	}

	// ī�޶� ȸ��
	void AttachSelectState::Stick_R()
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

		switch (mode)
		{
		case SELECT:
		{
			CameraAround();
		}
		break;

		case HOLD:
		case ROTATE:
		{
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
			break;

		default:;
		}
	}

	// ���
	void AttachSelectState::Click_A()
	{
		StateCancel();
	}

	// ����
	void AttachSelectState::Click_B()
	{
		switch (mode)
		{
		case SELECT:
			// ����ĳ����
			if (Select())
			{
				// ������ ������.
				Set();
			}
			break;

		// ������Ʈ�� �̵���ų�� �ִ� ���¶��
		case HOLD:
		case ROTATE:
			// ���� �� �־����.
			break;

		default:;
		}
	}

	// ���
	void AttachSelectState::Click_X()
	{
		StateCancel();
	}

	// ���
	void AttachSelectState::Click_Y()
	{
		StateCancel();
	}

	void AttachSelectState::Pressing_DUp()
	{
		switch (mode)
		{
		case HOLD:
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
		break;

		case ROTATE:
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_DDown()
	{
		switch (mode)
		{
		case HOLD:
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
		break;

		case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� -ȸ�� ��Ŵ
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_DLeft()
	{
		switch (mode)
		{

		case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� -ȸ�� ��Ŵ
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_DRight()
	{
		switch (mode)
		{

		case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_RB()
	{
		switch (mode)
		{

		case HOLD:

			break;

		default:;
		}
	}

	void AttachSelectState::Click_DUp()
	{
		switch (mode)
		{

		case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
			break;

		default:;
		}
	}

	void AttachSelectState::Click_DDown()
	{
		switch (mode)
		{

		case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
			break;

		default:;
		}
	}

	void AttachSelectState::Click_DLeft()
	{
		switch (mode)
		{

		case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
			break;

		default:;
		}
	}

	void AttachSelectState::Click_DRight()
	{
		switch (mode)
		{

		case ROTATE:
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
			break;

		default:;
		}
	}

	// ���
	void AttachSelectState::Click_LB()
	{
		StateCancel();
	}

#pragma endregion Ű�Է�

	void AttachSelectState::PlayerMove(float _speed) const
	{
		player->PlayerMove(_speed);
	}

	void AttachSelectState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttachSelectState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void AttachSelectState::Jump() const
	{
		player->Jump();
	}

	void AttachSelectState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
		debugVector1 = _to.normalized();

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachSelectState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		debugVector2 = _to.normalized();

		player->LookInLocalDirection(_to.normalized());
	}

	bool AttachSelectState::Search() const
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, nullptr, nullptr, nullptr);

		if (!hit)
			return false;

		return true;
	}

	bool AttachSelectState::Select()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttachSelectState::ApplyObjectVelocity() const
	{
		selectBody->SetLinearVelocity(targetVelocity);
	}

	void AttachSelectState::ResetObjectVelocity()
	{
		targetVelocity = Eigen::Vector3f::Zero();
	}

	void AttachSelectState::ObjectTranslate(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachSelectState::ObjectRotate()
	{
	}

	void AttachSelectState::Attach() const
	{
	}

	void AttachSelectState::Set()
	{
		using namespace Eigen;

		if (selectBody == nullptr)
			return;

		mode = HOLD;

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

	void AttachSelectState::Reset()
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
		mode = SELECT;
	}

	void AttachSelectState::SearchDebugDraw(bool _value)
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

		if (_value)
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"O",
				945, 510,
				200, 100, 50,
				255, 255, 255, 255);
		}
		else
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"X",
				945, 510,
				200, 100, 50,
				255, 255, 255, 255);
		}
	}
}
