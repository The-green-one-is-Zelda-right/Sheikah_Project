#include "AttachSelectState.h"

#include "AttachSystem.h"

namespace Phyzzle
{
	AttachSelectState::~AttachSelectState()
		= default;

#pragma region StateEvent
	// ���� ������
	void AttachSelectState::StateEnter()
	{
		{
			using namespace Eigen;

			player->data.cameraCore->SetLocalPosition(player->data.coreDefaultPosition);
			auto p = player->data.cameraCore->GetLocalPosition();
			p.x() = 0.5f;
			p.y() = 0.5f;

			player->data.cameraCore->SetLocalPosition(p);
			player->data.cameraCore->SetLocalRotation(Quaternionf::Identity());
		}

		CrossHeadRender(true);
	}

	// ���� ������
	void AttachSelectState::StateExit()
	{
		// �÷��̾� ī�޶� ������Ʈ�� ����ٸ�
		// �̺κ��� ������ �ݵ�� �ʿ���
		{
			using namespace Eigen;

			player->data.cameraCore->SetLocalPosition(player->data.coreDefaultPosition);
			player->data.cameraCore->SetLocalRotation(player->data.coreDefaultRotation);
		}

		CrossHeadRender(false);
	}

	// Update
	void AttachSelectState::StateStay()
	{
		EnableOutline(false);
		result = nullptr;
		attachable = nullptr;

		select = Search();

		if (select)
		{
			EnableOutline(true);
		}

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

	void AttachSelectState::CameraPositionUpdate()
	{

	}

	void AttachSelectState::StateCancel() const
	{
		EnableOutline(false);
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
		const bool hit = player->RaycastFromCamera(distance, &result, &attachable, nullptr);

		if (!hit || !attachable || !result)
			return false;

		if (result->IsKinematic())
			return false;

		return true;
	}

	void AttachSelectState::EnableOutline(bool _value) const
	{
		if (!result || !attachable)
			return;

		AttachSystem::Instance()->EnableOutline(attachable, _value);
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

	void AttachSelectState::CrossHeadRender(bool _value)
	{
		player->data.crossHead->SetEnable(_value);
	}
}
