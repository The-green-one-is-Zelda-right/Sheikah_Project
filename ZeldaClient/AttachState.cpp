#include "AttachState.h"

namespace Phyzzle
{
	AttachState::~AttachState()
		= default;

	// ���� ������
	void AttachState::StateEnter()
	{
		// �÷��̾� ī�޶� ������Ʈ�� ����ٸ�
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);
		}
	}

	// ���� ������
	void AttachState::StateExit()
	{
		// �÷��̾� ī�޶� ������Ʈ�� ����ٸ�
		// �̺κ��� ������ �ݵ�� �ʿ���
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);
		}

		Reset();
	}

	// Update
	void AttachState::StateStay()
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
	void AttachState::Stick_L()
	{
		PlayerMove();

		if (selected)
		{
			// ī�޶� �������� ĳ���͸� ȸ�� ��Ŵ
			// auto direction = player->data.cameraArm->GetLocalRotation() * Eigen::Vector3f::UnitZ();
			// ������Ʈ �������� ĳ���͸� ȸ�� ��Ŵ
			Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
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
	void AttachState::Stick_R()
	{
		// Stick L�� �÷��̾��� ������
		// Stick R�� D Pad�� ���� ������Ʈ�� ������

		// ������Ʈ ���� ������Ʈ�� �÷��̾��� ����� �Ÿ��� ȸ���� �ٽ� ����� ��.
		// ������Ʈ ���� �� ��ġ�� �������� üũ�ϰ� �ƴϸ� �ɷ��� �����.

		// Stick L�� �÷��̾ ������ ��, ��ü���� ���� ���� ��� ��.

		// ��ü�� ������ ���¶��
		// �¿� �Է��� '�÷��̾ ���� �ִ� ����'�� �¿�� ���� �ְ�
		// ���� �Է��� ���� Up, Down �������� ���� �ִ� ������ ����.
		// �÷��̾ �׻� ��ü�� �ٶ󺸰� �ִ� ������ �ϸ� ���� ����ϰ� ���� ���̴�.

		if (!selected)
		{
			CameraAround();
		}
		else
		{
			player->currInput.Rstick.X;
			player->currInput.Rstick.Y;
			player->currInput.Rstick.Size;
		}
	}

	// ���
	void AttachState::Click_A()
	{
		StateCancel();
	}

	// ����
	void AttachState::Click_B()
	{
		// ��Ʈ�� �ڵ� ���°� �ƴ϶��..
		if (!selected)
		{
			if (Select())
			{
				Set();
			}
		}
		// ������Ʈ�� �̵���ų�� �ִ� ���¶��
		else
		{
			// ���� �� �־����.
		}
	}

	// ���
	void AttachState::Click_X()
	{
		StateCancel();
	}

	// ���
	void AttachState::Click_Y()
	{
		StateCancel();
	}

	void AttachState::Click_DUp()
	{
		if (!rotateMode)
		{
			// ������Ʈ�� �ӷ��� �༭ ��
		}
		else
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� +ȸ�� ��Ŵ
		}
	}

	void AttachState::Click_DDown()
	{
		if (!rotateMode)
		{
			// ������Ʈ�� �ӷ��� �༭ ���
		}
		else
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� -ȸ�� ��Ŵ
		}
	}

	void AttachState::Click_DLeft()
	{
		if (rotateMode)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
		}
	}

	void AttachState::Click_DRight()
	{
		if (rotateMode)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
		}
	}

	// ���
	void AttachState::Click_LB()
	{
		StateCancel();
	}

	// ���
	void AttachState::Click_RB()
	{
		StateCancel();
	}
#pragma endregion Ű�Է�

	void AttachState::PlayerMove() const
	{
		player->PlayerMove(player->data.moveSpeed);
	}

	void AttachState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttachState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void AttachState::Jump() const
	{
		player->Jump();
	}

	void AttachState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
		debugVector1 = _to.normalized();

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		debugVector2 = _to.normalized();

		player->LookInLocalDirection(_to.normalized());
	}

	bool AttachState::Search() const
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, nullptr, nullptr, nullptr);

		if (!hit)
			return false;

		return true;
	}

	bool AttachState::Select()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttachState::ObjectToTargetPosition() const
	{
		using namespace Eigen;

		// const Vector3f currPosition = selectBody->GetPosition();
		// const Quaternionf currRotation = selectBody->GetRotation();
		// 
		// const Vector3f targetP = MulMatrixVector(player->data.modelCore->GetWorldMatrix(), targetLocalPosition);
		// const Quaternionf targetR = player->data.modelCore->GetWorldRotation() * targetLocalRotation;
		// 
		// const Vector3f diffP = targetP - currPosition;
		// const Quaternionf diffR = currRotation.inverse() * targetR;

		// PurahEngine::GraphicsManager::GetInstance().DrawString(
		// 	L"���� ȸ�� �� : " +
		// 	std::to_wstring(currRotation.vec().x()) + L" " +
		// 	std::to_wstring(currRotation.vec().y()) + L" " +
		// 	std::to_wstring(currRotation.vec().z()) + L" ",
		// 	1200, 100,
		// 	200, 300, 15,
		// 	255, 255, 255, 255);
		// 
		// PurahEngine::GraphicsManager::GetInstance().DrawString(
		// 	L"��ǥ ȸ�� �� : " +
		// 	std::to_wstring(targetR.vec().x()) + L" " + 
		// 	std::to_wstring(targetR.vec().y()) + L" " + 
		// 	std::to_wstring(targetR.vec().z()) + L" ",
		// 	1200, 150,
		// 	200, 300, 15,
		// 	255, 255, 255, 255);
		// 
		// PurahEngine::GraphicsManager::GetInstance().DrawString(
		// 	L"ȸ�� ���� : " + 
		// 	std::to_wstring(diffR.vec().x()) + L" " +
		// 	std::to_wstring(diffR.vec().y()) + L" " +
		// 	std::to_wstring(diffR.vec().z()) + L" ",
		// 	1200, 200,
		// 	200, 300, 15,
		// 	255, 255, 255, 255);

		// selectBody->SetLinearVelocity(diffP / lerpTime);
		// selectBody->SetRotation(targetR);
	}

	void AttachState::ObjectTranslate(const Eigen::Vector3f& _direction, float power)
	{
		selectBody->SetLinearVelocity(_direction * power);
	}

	void AttachState::Set()
	{
		if (selectBody == nullptr)
			return;

		using namespace Eigen;
		selected = true;

		// ���⼭ ����� ������Ʈ���� ��ȸ�ϸ鼭 ���� ������ �� �ʿ䰡 ����.
		// ������Ʈ ������ ���¿� �����ϰ� ������
		// Attatchable 

		hasGravity = selectBody->HasGravity();
		mass = selectBody->GetMass();

		selectBody->UseGravity(false);
		selectBody->SetMass(0.001f);                    

		const Eigen::Vector3f objectPosition = selectBody->GetPosition();
		const Eigen::Vector3f playerPosition = player->GetGameObject()->GetTransform()->GetWorldPosition();

		const Eigen::Vector3f lookTo = objectPosition - playerPosition;
		// ���� ��ü�� �ٶ󺸵��� ��.
		LookToWorldDirection(lookTo);

		debugVector0 = lookTo;

		// targetLocalPosition = MulMatrixVector(player->data.modelCore->GetWorldMatrix().inverse(), selectBody->GetPosition());
		// targetLocalRotation = player->data.modelCore->GetWorldRotation().inverse() * selectBody->GetRotation();
	}

	void AttachState::Reset()
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

	void AttachState::SearchDebugDraw(bool _value)
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
