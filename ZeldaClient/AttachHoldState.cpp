#include "AttachHoldState.h"

#include "../GraphicsTest/CoreSystem.h"

namespace Phyzzle
{
	AttachHoldState::~AttachHoldState()
	= default;

#pragma region StateEvent
	void AttachHoldState::StateEnter()
	{
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);
		}

		TrySelect();
	}

	void AttachHoldState::StateExit()
	{
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);
		}

		VariableReset();
	}

	void AttachHoldState::StateStay()
	{

		SpringCalculate();
		ApplyObjectVelocity();
		ResetObjectVelocity();
	}
#pragma endregion StateEvent

#pragma region Input
	// �÷��̾� �̵�
	void AttachHoldState::Stick_L()
	{
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
		// Ű �Է��� ������ �� ����
		if (player->currInput.Rstick.Size)
		{
			ObjectXTranslate(5.f * player->currInput.Rstick.X * player->currInput.Rstick.Size);
			ObjectYTranslate(5.f * player->currInput.Rstick.Y * player->currInput.Rstick.Size);
		}
	}

	void AttachHoldState::Trigger_L()
	{
		adjustmentMode = false;

		if (player->currInput.LTrigger > 0)
			adjustmentMode = true;
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
		TryAttach();
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
		const bool justRotate = !adjustmentMode && roateMode;
		const bool justTranslate = !adjustmentMode && !roateMode;

		if (justTranslate)
		{
			SpringZTranslate(2.f);
		}
		else if (justRotate)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� +ȸ�� ��Ŵ
			SpringXRotate(rotateAngle);
		}
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DDown()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		const bool justTranslate = !adjustmentMode && !roateMode;

		if (justTranslate)
		{
			SpringZTranslate(-2.f);
		}
		else if (justRotate)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
			SpringXRotate(-rotateAngle);
		}
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DLeft()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		if (justRotate)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ
			SpringYRotate(rotateAngle);
		}
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DRight()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		if (justRotate)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ

			SpringYRotate(-rotateAngle);
		}
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Pressing_DUp()
	{
		const bool rotateAdjustment = adjustmentMode && roateMode;
		const bool translateAdjustment = adjustmentMode && !roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (translateAdjustment)
		{
			// �÷��̾� - ������Ʈ �������� �ְ� ����.
			const float velocity = pushingVelocity * player->currInput.LTrigger * dt;

			SpringZTranslate(velocity);
		}
		else if (rotateAdjustment)
		{
			const float velocity = rotateAngle * player->currInput.LTrigger * dt;

			SpringXRotate(velocity);
		}
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Pressing_DDown()
	{
		const bool translateAdjustment = adjustmentMode && !roateMode;
		const bool rotateAdjustment = adjustmentMode && roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		// �̵�
		if (translateAdjustment)
		{
			const float velocity = pushingVelocity * player->currInput.LTrigger * dt;

			SpringZTranslate(-velocity);
		}
		// ȸ��
		else if (rotateAdjustment)
		{
			const float velocity = rotateAngle * player->currInput.LTrigger * dt;
			
			SpringXRotate(-velocity);
		}
	}

	// ������Ʈ ȸ��
	void AttachHoldState::Pressing_DLeft()
	{
		const bool rotateAdjustment = adjustmentMode && roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (rotateAdjustment)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� +ȸ�� ��Ŵ

			const float velocity = rotateAngle * player->currInput.LTrigger * dt;

			SpringYRotate(velocity);
		}
	}

	// ������Ʈ ȸ��
	void AttachHoldState::Pressing_DRight()
	{
		const bool rotateAdjustment = adjustmentMode && roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (rotateAdjustment)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� Y �� �������� -ȸ�� ��Ŵ

			const float velocity = rotateAngle * player->currInput.LTrigger * dt;

			SpringYRotate(-velocity);
		}
	}

	// Rotate ���
	void AttachHoldState::Pressing_RB()
	{
		roateMode = true;
	}

	void AttachHoldState::Up_RB()
	{
		roateMode = false;
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

	void AttachHoldState::TrySelect()
	{
		const bool hit = player->RaycastFromCamera(selectRange, &selectBody, nullptr, nullptr);

		if (!hit || !selectBody)
		{
			player->ChangeState(Player::ATTACH_SELECT);
		}
		else
		{
			if (selectBody->GetGameObject()->tag.IsContain(L"Phyzzle Player"))
				player->ChangeState(Player::ATTACH_SELECT);
			else
				VariableSet();
		}
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
		selectBody->SetLinearVelocity(targetVelocity + springL);
		selectBody->SetAngularVelocity(targetAngularVelocity + springR);
	}

	void AttachHoldState::ResetObjectVelocity()
	{
		targetVelocity = Eigen::Vector3f::Zero();
		targetAngularVelocity = Eigen::Vector3f::Zero();
	}

	void AttachHoldState::SpringCalculate()
	{
		float zeta = 0.7f;												// Damping ratio
		float zeta0 = 0.1f;												// Damping ratio
		float omega = 2.0f * std::numbers::pi_v<float> * 5.0f;			// Angular frequency (5 Hz)
		float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();	// Time step

		Eigen::Vector3f currPos = selectBody->GetPosition();
		currPos.y() = 0.f;				// stick R �Է����� ������Ʈ�� ������ ���� �������� �ƴϹǷ�..

		Eigen::Vector3f playerPos = player->data.modelCore->GetWorldPosition();
		playerPos.y() = 0.f;
		Eigen::Vector3f zPos = player->data.modelCore->GetFront() * targetPosition.z();
		zPos.y() = 0.f;
		const Eigen::Vector3f targetP = playerPos + zPos;

		posSpring.Update(
			currPos,
			springL,
			targetP,
			zeta,
			omega,
			timeStep
		);

		Eigen::Quaternionf currRot = selectBody->GetRotation();
		const Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();
		const Eigen::Quaternionf targetR = playerRot * targetRotation;

		quatSpring.Update(
			currRot,
			springR,
			targetR,
			zeta0,
			timeStep);

		selectBody->SetRotation(currRot);
	}

	void AttachHoldState::ObjectPostionSpring(
		const Eigen::Vector3f& _s, Eigen::Vector3f& _v, const Eigen::Vector3f& _e,
		float _zeta, float _omega, float _dt)
	{
		const float f = 1.f + 2.f * _dt * _zeta * _omega;
		const float oo = _omega * _omega;
		const float hoo = _dt * oo;
		const float hhoo = _dt * hoo;
		const float detInv = 1.f / (f + hhoo);
		const Eigen::Vector3f detV = _v + hoo * (_e - _s);
		
		_v = detV * detInv;
	}

	void AttachHoldState::Spring(
		const float& _s, float& _v, const float& _e,
		float _zeta, float _omega, float _dt)
	{
		const float f = 1.0f + 2.0f * _dt * _zeta * _omega;
		const float oo = _omega * _omega;
		const float hoo = _dt * oo;
		const float hhoo = _dt * hoo;
		const float detInv = 1.0f / (f + hhoo);
		const float detV = _v + hoo * (_e - _s);

		// Update velocity vector
		_v = detV * detInv;
	}

	void AttachHoldState::SpringYTranslate(float _distance)
	{
		targetPosition.y() += _distance;
	}

	void AttachHoldState::SpringZTranslate(float _distance)
	{
		targetPosition.z() += _distance;
	}

	void AttachHoldState::ObjectTranslate(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachHoldState::ObjectXTranslate(float _distance)
	{
		const Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const Eigen::Vector3f objPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objPos - playerPos;
		forward.y() = 0.f;
		forward.normalize();
		const Eigen::Vector3f right = Eigen::Vector3f::UnitY().cross(forward);

		ObjectTranslate(right.normalized(), _distance);
	}

	void AttachHoldState::ObjectYTranslate(float _distance)
	{
		// �Ÿ� ���ؾ���.

		ObjectTranslate(Eigen::Vector3f::UnitY(), _distance);
	}

	void AttachHoldState::ObjectZTranslate(float _distance)
	{
		// �Ÿ� �� �ؾ���.

		const auto playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const auto objPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objPos - playerPos;
		forward.y() = 0.f;
		forward.normalize();

		ObjectTranslate(forward, _distance);
	}

	void AttachHoldState::SpringRotate(const Eigen::Vector3f& _axis, float _angle)
	{
		targetRotation = Eigen::Quaternionf(Eigen::AngleAxisf{ _angle, _axis }) * targetRotation;
	}

	void AttachHoldState::SpringXRotate(float _angle)
	{
		Eigen::Vector3f dir = Eigen::Vector3f::UnitX();

		SpringRotate(dir, _angle);
	}

	void AttachHoldState::SpringYRotate(float _angle)
	{
		const Eigen::Vector3f worldUp = Eigen::Vector3f::UnitY();

		SpringRotate(worldUp, _angle);
	}

	void AttachHoldState::TryAttach() const
	{
		Attach();
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

		Eigen::Vector3f lookTo = objectPosition - playerPosition;
		targetPosition.y() = lookTo.y();
		lookTo.y() = 0.f;
		LookToWorldDirection(lookTo);
		targetPosition.z() = lookTo.norm();

		const Eigen::Quaternionf objectRotation = selectBody->GetRotation();
		const Eigen::Quaternionf playerRotation = player->GetGameObject()->GetTransform()->GetWorldRotation();
		targetRotation = playerRotation.inverse() * objectRotation;
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
		targetAngularVelocity = Eigen::Vector3f::Zero();

		springL = Eigen::Vector3f::Zero();
		springR = Eigen::Vector3f::Zero();

		targetPosition = Eigen::Vector3f::Zero();
		targetRotation = Eigen::Quaternionf::Identity();

		hasGravity = false;
		mass = -0.1f;
		diffWidth = -0.1f;
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
