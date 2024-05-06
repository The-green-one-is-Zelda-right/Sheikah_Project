#include "AttachSelectState.h"

namespace Phyzzle
{
	AttachSelectState::~AttachSelectState()
		= default;

#pragma region StateEvent
	// ���� ������
	void AttachSelectState::StateEnter()
	{
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);
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
		}
	}

	// Update
	void AttachSelectState::StateStay()
	{
		select = Search();

#if _DEBUG
		SearchDebugDraw(select);
#endif _DEBUG
	}
#pragma endregion StateEvent

#pragma region Input
	// �̵�
	void AttachSelectState::Stick_L()
	{
		// �̵� �������� ĳ���͸� ȸ�� ��Ŵ
		PlayerMove(player->data.moveSpeed);
		auto velocity = player->data.playerRigidbody->GetLinearVelocity();
		velocity.y() = 0.f;
		LookToWorldDirection(velocity);
	}

	// ī�޶� ȸ��
	void AttachSelectState::Stick_R()
	{
		CameraAround();
	}

	// ���
	void AttachSelectState::Click_A()
	{
		StateCancel();
	}

	// ����
	void AttachSelectState::Click_B()
	{
		// ����ĳ����
		if (select)
		{ 
			// ���� �ٲ�
			player->ChangeState(Player::ATTACH_HOLD);
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

	// ���
	void AttachSelectState::Click_LB()
	{
		StateCancel();
	}

	void AttachSelectState::Pressing_RB()
	{
	}

#pragma endregion Input

#pragma region Content
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
		player->LookInWorldDirection(_to.normalized());
	}

	void AttachSelectState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		player->LookInLocalDirection(_to.normalized());
	}

	bool AttachSelectState::Search()
	{
		const float distance = 40.f;
		const bool hit = player->RaycastFromCamera(distance, &result, nullptr, nullptr);

		if (!hit)
			return false;

		if (result->IsKinematic())
			return false;

		if (result->GetGameObject()->tag.IsContain(L"Phyzzle Player"))
			return false;

		return true;
	}
#pragma endregion Content

#if _DEBUG
	void AttachSelectState::SearchDebugDraw(bool _value)
	{
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
#endif _DEBUG
}
