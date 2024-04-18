#include "AttatchState.h"

namespace Phyzzle
{
	AttatchState::~AttatchState()
		= default;

	// ���� ������
	void AttatchState::StateEnter()
	{
		auto p = player->data.cameraCore->GetLocalPosition();
		p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

		player->data.cameraCore->SetLocalPosition(p);
	}

	// ���� ������
	void AttatchState::StateExit()
	{
		auto p = player->data.cameraCore->GetLocalPosition();
		p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

		player->data.cameraCore->SetLocalPosition(p);

		Reset();
	}

	// Update
	void AttatchState::StateStay()
	{
		if (selected)
		{
			// ������Ʈ�� ��ġ�� ������Ʈ ��.
			ObjectToTargetPosition();
		}
		else
		{
			SearchDebugDraw(Search());
		}
	}

#pragma region Ű�Է�
	// �̵�
	void AttatchState::Stick_L()
	{
		PlayerMove();

		if (selected)
		{
			// ī�޶� �������� ĳ���͸� ȸ�� ��Ŵ
			auto direction = player->data.cameraArm->GetLocalRotation() * Eigen::Vector3f::UnitZ();
			direction.y() = 0.f;

			LookToLocalDirection(direction);
		}
		else
		{
			// �̵� �������� ĳ���͸� ȸ�� ��Ŵ
			auto velocity = player->data.playerRigidbody->GetLinearVelocity();
			velocity.y() = 0.f;

			LookToWorldDirection(velocity);
		}
	}

	// ī�޶� ȸ��
	void AttatchState::Stick_R()
	{
		// ��ü�� ������ ���¶��
		// �¿� �Է��� '�÷��̾ ���� �ִ� ����'�� �¿�� ���� �ְ�
		// ���� �Է��� ���� Up, Down �������� ���� �ִ� ������ ����.
		// �÷��̾ �׻� ��ü�� �ٶ󺸰� �ִ� ������ �ϸ� ���� ����ϰ� ���� ���̴�.
		CameraAround();
	}

	// ���
	void AttatchState::Click_A()
	{
		StateCancel();
	}

	// ����
	void AttatchState::Click_B()
	{
		if (Select())
		{
			Set();
		}
	}

	// ���
	void AttatchState::Click_X()
	{
		StateCancel();
	}

	// ���
	void AttatchState::Click_Y()
	{
		StateCancel();
	}

	// ���
	void AttatchState::Click_LB()
	{
		StateCancel();
	}

	// ���
	void AttatchState::Click_RB()
	{
		StateCancel();
	}
#pragma endregion Ű�Է�

	void AttatchState::PlayerMove() const
	{
		player->Move(player->data.moveSpeed);
	}

	void AttatchState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttatchState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void AttatchState::Jump() const
	{
		player->Jump();
	}

	void AttatchState::LookToWorldDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInWorldDirection(_to.normalized());
	}

	void AttatchState::LookToLocalDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInLocalDirection(_to.normalized());
	}

	bool AttatchState::Search() const
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, nullptr, nullptr, nullptr);

		if (!hit)
			return false;

		return true;
	}

	bool AttatchState::Select()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttatchState::ObjectToTargetPosition() const
	{
		using namespace Eigen;

		const Vector3f currPosition = selectBody->GetPosition();
		const Quaternionf currRotation = selectBody->GetRotation();

		const Vector3f targetP = MulMatrixVector(player->data.modelCore->GetWorldMatrix(), targetLocalPosition);
		const Quaternionf targetR = player->data.modelCore->GetWorldRotation() * targetLocalRotation;

		const Vector3f diffP = targetP - currPosition;
		const Quaternionf diffR = currRotation.inverse() * targetR;

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"���� ȸ�� �� : " +
			std::to_wstring(currRotation.vec().x()) + L" " +
			std::to_wstring(currRotation.vec().y()) + L" " +
			std::to_wstring(currRotation.vec().z()) + L" ",
			1200, 100,
			200, 300, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"��ǥ ȸ�� �� : " +
			std::to_wstring(targetR.vec().x()) + L" " + 
			std::to_wstring(targetR.vec().y()) + L" " + 
			std::to_wstring(targetR.vec().z()) + L" ",
			1200, 150,
			200, 300, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"ȸ�� ���� : " + 
			std::to_wstring(diffR.vec().x()) + L" " +
			std::to_wstring(diffR.vec().y()) + L" " +
			std::to_wstring(diffR.vec().z()) + L" ",
			1200, 200,
			200, 300, 15,
			255, 255, 255, 255);

		selectBody->SetLinearVelocity(diffP / lerpTime);
		selectBody->SetRotation(targetR);
	}

	void AttatchState::Set()
	{
		if (selectBody == nullptr)
			return;

		using namespace Eigen;
		selected = true;

		hasGravity = selectBody->HasGravity();
		mass = selectBody->GetMass();

		selectBody->UseGravity(false);
		selectBody->SetMass(0.001f);

		Eigen::Vector3f lookTo = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
		lookTo.normalize();
		// ���� ��ü�� �ٶ󺸵��� ��.
		LookToWorldDirection(lookTo);

		targetLocalPosition = MulMatrixVector(player->data.modelCore->GetWorldMatrix().inverse(), selectBody->GetPosition());
		targetLocalRotation = player->data.modelCore->GetWorldRotation().inverse() * selectBody->GetRotation();
	}

	void AttatchState::Reset()
	{
		using namespace Eigen;

		if (selectBody && selected)
		{
			selectBody->UseGravity(hasGravity);
			selectBody->SetMass(mass);
		}

		targetLocalPosition = Vector3f::Zero();
		targetLocalRotation = Quaternionf::Identity();
		hasGravity = false;
		mass = -0.1f;
		selectBody = nullptr;
		selected = false;
	}

	void AttatchState::SearchDebugDraw(bool _value)
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
}
