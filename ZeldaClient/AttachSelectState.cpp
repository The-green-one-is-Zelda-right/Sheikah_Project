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

			auto p = player->camData.coreDefaultPosition;
			p.x() += 0.5f;
			p.y() += 0.5f;

			player->SetCameraCoreLocalTargetPosition(p);
		}

		CrossHeadRender(true);
	}

	// ���� ������
	void AttachSelectState::StateExit()
	{
		{
			player->SetCameraCoreLocalTargetPosition(player->camData.coreDefaultPosition);
		}

		select = false;
		selectObject = nullptr;
		seleteBody = nullptr;

		CrossHeadRender(false);
	}

	// Update
	void AttachSelectState::StateStay()
	{
		EnableOutline(false);
		seleteBody = nullptr;
		selectObject = nullptr;

		select = Search();

		if (select)
		{
			EnableOutline(true);
		}
		
		if (player->data.debugMode)
		{
			SearchDebugDraw(select);
		}
	}

	void AttachSelectState::PostStateStay()
	{
		CameraUpdate();
	}

	void AttachSelectState::StateCancel()
	{
		Cancel();
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

	}

	// ���
	void AttachSelectState::Click_A()
	{
		Cancel();
	}

	// ����
	void AttachSelectState::Click_B()
	{
		// ����ĳ����
		if (select)
		{ 
			// ���� �ٲ�
			player->data.holdObject = selectObject;
			player->data.holdObjectBody = seleteBody;
			player->ChangeAbilityState(Player::ATTACH_HOLD);
		}
	}

	// ���
	void AttachSelectState::Click_X()
	{
		Cancel();
	}

	// ���
	void AttachSelectState::Click_Y()
	{
		Cancel();
	}

	// ���
	void AttachSelectState::Click_LB()
	{
		Cancel();
	}
#pragma endregion Input

#pragma region Content
	void AttachSelectState::PlayerMove(float _speed) const
	{
		if (player->TryPlayerMove(_speed))
		{
			player->ChangePlayerAnimationState(Player::WALK);
		}
		else
		{
			player->ChangePlayerAnimationState(Player::IDLE);
		}
	}

	void AttachSelectState::CameraUpdate() const
	{
		player->UpdateSelectCamera();
	}

	void AttachSelectState::Cancel()
	{
		EnableOutline(false);
		CrossHeadRender(false);
		player->ChangeAbilityState(Player::AbilityState::DEFAULT);
	}

	void AttachSelectState::Jump() const
	{
		if (player->TryJump())
		{
			player->ChangePlayerAnimationState(Player::JUMP);
		}
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
		const bool hit = player->RaycastFromCamera(distance, &seleteBody, &selectObject, nullptr);

		if (!hit || !selectObject || !seleteBody)
			return false;

		if (seleteBody->IsKinematic())
			return false;

		return true;
	}

	void AttachSelectState::EnableOutline(bool _value) const
	{
		if (!seleteBody || !selectObject)
			return;

		if (_value)
		{
			AttachSystem::Instance()->EnableOutline(selectObject, &player->color0, &player->color1);
		}
		else
		{
			AttachSystem::Instance()->DisableOutline(selectObject);
		}
	}
	
	void AttachSelectState::CrossHeadRender(bool _value)
	{
		player->data.crossHead->SetEnable(_value);
	}
#pragma endregion Content

#pragma region Debug
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
#pragma endregion Debug
}
