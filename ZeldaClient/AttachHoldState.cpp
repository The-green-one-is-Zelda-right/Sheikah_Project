#include "AttachHoldState.h"

#include "PzObject.h"
#include "AttachSystem.h"

namespace Phyzzle
{
	AttachHoldState::AttachHoldState(Player* _player)
		: IState(_player), selectBody(), attachble()
	{
		using namespace Eigen;

		std::vector<Quaternionf> axis;
		InitializeAxis(axis, 90);

		// none
		std::vector<Quaternionf> none;
		{
			for (size_t i = 0; i < axis.size(); i++)
			{
				Quaternionf newAxis = axis[i];
				for (size_t z = 0; z < 360; z += 90)
				{
					float angleZ = (static_cast<float>(z) / 180.f) * std::numbers::pi_v<float>;
					Quaternionf axisZ = Quaternionf(AngleAxisf(angleZ, Vector3f::UnitZ()));

					Quaternionf temp = axisZ * newAxis;
					none.emplace_back(temp);
					axisies.emplace_back(temp, None);
				}
			}
		}

		// X
		std::vector<Quaternionf> rotateX;
		InitializeRotations(none, rotateX, 45, Vector3f::UnitX(), RotateX);

		// Y
		std::vector<Quaternionf> rotateY;
		InitializeRotations(none, rotateY, 45, Vector3f::UnitY(), RotateY);
		InitializeRotations(rotateX, 45, Vector3f::UnitY(), RotateXY);
		InitializeRotations(rotateX, -45, Vector3f::UnitY(), RotateX_Y);
		InitializeRotations(rotateY, 45, Vector3f::UnitX(), RotateYX);
		InitializeRotations(rotateY, -45, Vector3f::UnitX(), RotateY_X);
		InitializeRotations(none, 45, Vector3f::UnitZ(), RotateZ);
	}

	AttachHoldState::~AttachHoldState()
	{
		AttachSystem::Instance()->Clear();
	}

#pragma region Initialize
	void AttachHoldState::InitializeAxis(std::vector<Eigen::Quaternionf>& axis, float increment) 
	{
		using namespace Eigen;
		Quaternionf classic = Quaternionf::Identity();

		for (size_t x = 0; x < 360; x += increment)
		{
			float angleY = (static_cast<float>(x) / 180.f) * std::numbers::pi_v<float>;
			Quaternionf axisY = Quaternionf(AngleAxisf(angleY, Vector3f::UnitY()));
			Quaternionf temp = axisY * classic;
			axis.emplace_back(temp);
		}

		InitializeAxisHelper(axis, -90.f, Vector3f::UnitX());
		InitializeAxisHelper(axis, 90.f, Vector3f::UnitX());
	}

	void AttachHoldState::InitializeAxisHelper(std::vector<Eigen::Quaternionf>& axis, float angle, const Eigen::Vector3f& vector) 
	{
		using namespace Eigen;
		Quaternionf classic = Quaternionf::Identity();

		float angleRad = (angle / 180.f) * std::numbers::pi_v<float>;
		Quaternionf axisAngle = Quaternionf(AngleAxisf(angleRad, vector));
		Quaternionf temp = axisAngle * classic;
		axis.emplace_back(temp);
	}

	void AttachHoldState::InitializeRotations(
		const std::vector<Eigen::Quaternionf>& input, 
		std::vector<Eigen::Quaternionf>& output, float angle, 
		const Eigen::Vector3f& axis, RotateInfo type)
	{
		for (const auto& quat : input)
		{
			float angleRad = (angle / 180.f) * std::numbers::pi_v<float>;
			Eigen::Quaternionf axisQuat = Eigen::Quaternionf(Eigen::AngleAxisf(angleRad, axis));

			Eigen::Quaternionf temp = axisQuat * quat;
			output.emplace_back(temp);
			axisies.emplace_back(temp, type);
		}
	}

	void AttachHoldState::InitializeRotations(
		const std::vector<Eigen::Quaternionf>& input, 
		float angle, const Eigen::Vector3f& axis, RotateInfo type)
	{
		for (const auto& quat : input)
		{
			float angleRad = (angle / 180.f) * std::numbers::pi_v<float>;
			Eigen::Quaternionf axisQuat = Eigen::Quaternionf(Eigen::AngleAxisf(angleRad, axis));

			Eigen::Quaternionf temp = axisQuat * quat;
			axisies.emplace_back(temp, type);
		}
	}
#pragma endregion Initialize

#pragma region StateEvent
	void AttachHoldState::StateEnter()
	{
		using namespace Eigen;

		if (TrySelect())
		{
			Vector3f corePos = player->data.cameraCore->GetWorldPosition();
			Quaternionf coreRot = player->data.cameraCore->GetWorldRotation();

			player->data.cameraArm->SetLocalPosition(player->data.armDefaultPosition);
			player->data.armTargetPosition = player->data.armDefaultPosition;
			
			auto rot = player->data.modelCore->GetLocalRotation();
			player->data.cameraArm->SetLocalRotation(rot);
			player->data.armTargetRotation = rot;
			player->data.xAngle = 0.f;

			player->data.cameraCore->SetWorldRotation(coreRot);
			player->data.cameraCore->SetWorldPosition(corePos);
		}
	}

	void AttachHoldState::StateExit()
	{
		{
			using namespace Eigen;
			player->SetCameraCoreLocalTargetPosition(player->data.coreDefaultPosition);
			player->SetCameraCoreLocalTargetRotation(player->data.coreDefaultRotation);
		}

		EnableOutline(false);
		VariableReset();
	}

	void AttachHoldState::StateStay()
	{
		// ������ ���� ���
		CalculateSpringForces();

		// ������Ʈ�� �ӷ��� ����
		ApplyObjectVelocity();

		// �ӷ� ����
		ResetObjectVelocity();

		if (player->data.debugMode)
		{
			auto euler = targetRotation.toRotationMatrix().eulerAngles(0, 1, 2);
			auto degree = Eigen::Vector3f(
				euler.x() * 180.f / std::numbers::pi_v<float>,
				euler.y() * 180.f / std::numbers::pi_v<float>,
				euler.z() * 180.f / std::numbers::pi_v<float>
			);
			degree.x() = std::floor(degree.x() * 10.f) / 10.f;
			degree.y() = std::floor(degree.y() * 10.f) / 10.f;
			degree.z() = std::floor(degree.z() * 10.f) / 10.f;
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"������Ʈ ���� : \n" +
				std::to_wstring(degree.x()) + L" \n" +
				std::to_wstring(degree.y()) + L" \n" +
				std::to_wstring(degree.z()) + L" \n",
				1400, 100,
				200, 600, 15,
				255, 255, 255, 255);

			SearchDebugDraw();
		}
	}

	void AttachHoldState::PostStateStay()
	{
		// ī�޶� ������Ʈ
		UpdateCamera();
	}

	void AttachHoldState::StateCancel()
	{
		EnableOutline(false);
		Put();
		Cancel();
	}
#pragma endregion StateEvent

#pragma region Input
	// �÷��̾� �̵�
	void AttachHoldState::Stick_L()
	{
		PlayerMove(player->data.holdSpeed);

		// ������Ʈ �������� ĳ���͸� ȸ�� ��Ŵ
		Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
		direction.y() = 0.f;
		LookToLocalDirection(direction);

		// �÷��̾� �̵� ����� ���� �������� ���� ��
		TranslateObject(player->data.playerRigidbody->GetLinearVelocity(), 1.f);
	}

	// ������Ʈ �̵�
	void AttachHoldState::Stick_R()
	{
		// Ű �Է��� ������ �� ����
		if (player->currInput.Rstick.Size)
		{
			TranslateObjectAlongX(5.f * player->currInput.Rstick.X * player->currInput.Rstick.Size);
			TranslateObjectAlongY(5.f * player->currInput.Rstick.Y * player->currInput.Rstick.Size);
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
		Put();
		Cancel();
	}

	// ����
	void AttachHoldState::Click_B()
	{
		// ������Ʈ�� �̵���ų�� �ִ� ���¶��
		// ���� �� �־����.
		if (TryAttach())
		{
			Put();
			Cancel();
		}
	}

	// ���
	void AttachHoldState::Click_X()
	{
		Put();
		Cancel();
	}

	// ���
	void AttachHoldState::Click_Y()
	{
		TryDettach();
	}

	// ���
	void AttachHoldState::Click_LB()
	{
		Put();
		Cancel();
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DUp()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		const bool justTranslate = !adjustmentMode && !roateMode;

		if (justTranslate)
		{
			TranslateSpringAlongZ(2.f);
		}
		else if (justRotate)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� +ȸ�� ��Ŵ

			switch(info.info)
			{
			case None:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateX;
				break;

			case RotateX:
				RotateSpringAlongX(rotateAngle);
				info.info = None;
				break;

			case RotateY:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateYX;
				break;

			case RotateX_Y:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� ���ʿ� ���� ��
				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(-rotateAngle);	// ��
				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(rotateAngle);		// ��
				info.info = RotateXY;
				break;

			case RotateXY:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� �����ʿ� ���� ��
				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(rotateAngle);		// ��
				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(-rotateAngle);	// ��
				info.info = RotateX_Y;
				break;

			case RotateY_X:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateY;
				break;

			case RotateYX:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateZ;
				break;

			case RotateZ:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateY_X;
				break;

			default: ;
				assert(0);
			}
		}
	}

	// ������Ʈ �̵� �� ȸ��
	void AttachHoldState::Click_DDown()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		const bool justTranslate = !adjustmentMode && !roateMode;

		if (justTranslate)
		{
			TranslateSpringAlongZ(-2.f);
		}
		else if (justRotate)
		{
			// ������Ʈ�� ���ӵ��� �༭ ȸ����Ŵ
			// �÷��̾� �������� X �� �������� -ȸ�� ��Ŵ

			switch (info.info)
			{
			case None:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateX;
				break;

			case RotateX:
				RotateSpringAlongX(-rotateAngle);
				info.info = None;
				break;

			case RotateY:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateY_X;
				break;

			case RotateX_Y:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� ���ʿ� ���� ��
				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(-rotateAngle);	// ��
				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(rotateAngle);		// ��
				info.info = RotateXY;
				break;

			case RotateXY:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� �����ʿ� ���� ��
				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(rotateAngle);		// ��
				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(-rotateAngle);	// ��
				info.info = RotateX_Y;
				break;

			case RotateY_X:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateZ;
				break;

			case RotateYX:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateY;
				break;

			case RotateZ:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateYX;
				break;

			default:;
				assert(0);
			}
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

			switch (info.info)
			{
			case None:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateY;
				break;

			case RotateX:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateXY;
				break;

			case RotateY:
				RotateSpringAlongY(rotateAngle);
				info.info = None;
				break;

			case RotateX_Y:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateX;
				break;

			case RotateXY:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateZ;
				break;

			case RotateY_X:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� ���� ���� ��
				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(-rotateAngle);	// ��
				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(rotateAngle);		// ��

				info.info = RotateYX;
				break;

			case RotateYX:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� �Ʒ��� ���� ��
				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(rotateAngle);		// ��
				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(rotateAngle);		// ��

				RotateSpringAlongX(-rotateAngle);	// ��

				info.info = RotateY_X;
				break;

			case RotateZ:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateX_Y;
				break;

			default:;
				assert(0);
			}
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

			switch (info.info)
			{
			case None:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateY;
				break;

			case RotateX:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateX_Y;
				break;

			case RotateY:
				RotateSpringAlongY(-rotateAngle);
				info.info = None;
				break;

			case RotateX_Y:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateZ;
				break;

			case RotateXY:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateX;
				break;

			case RotateY_X:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� ���� ���� ��
				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(-rotateAngle);	// ��
				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(rotateAngle);		// ��

				info.info = RotateYX;
				break;

			case RotateYX:
				// ������ �� �����ε�
				// ���� ���� �Ǵ� �� �Ʒ��� ���� ��
				RotateSpringAlongX(-rotateAngle);	// ��

				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(rotateAngle);		// ��
				RotateSpringAlongX(rotateAngle);		// ��

				RotateSpringAlongY(-rotateAngle);	// ��

				RotateSpringAlongX(-rotateAngle);	// ��
				info.info = RotateY_X;
				break;

			case RotateZ:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateXY;
				break;

			default:;
				assert(0);
			}
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

			TranslateSpringAlongZ(velocity);
		}
		else if (rotateAdjustment)
		{
			const float velocity = rotateAngle * player->currInput.LTrigger * dt;

			RotateSpringAlongX(velocity);
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

			TranslateSpringAlongZ(-velocity);
		}
		// ȸ��
		else if (rotateAdjustment)
		{
			const float velocity = rotateAngle * player->currInput.LTrigger * dt;
			
			RotateSpringAlongX(-velocity);
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

			RotateSpringAlongY(velocity);
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

			RotateSpringAlongY(-velocity);
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
		if (player->TryPlayerMove(_speed))
		{
			float x = player->currInput.Lstick.X;
			float y = player->currInput.Lstick.Y;

			bool front = y > 0.f;
			bool right = x > 0.f;
			bool xBigger = std::fabs(x) > std::fabs(y);

			if (front && !xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_FRONT);
			}
			else if (!front && !xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_BACK);
			}
			else if (right && xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_RIGHT);
			}
			else if (!right && xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_LEFT);
			}
		}
		else
		{
			player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_IDLE);
		}
	}

	void AttachHoldState::UpdateCamera()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();

		UpdateHoldingCameraPosition(localPosition, worldPosition);
		UpdateHoldingCameraRotation();

		if (player->ResolveCameraCollision(localPosition, worldPosition))
		{
			player->SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			player->SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void AttachHoldState::UpdateHoldingCameraPosition(Eigen::Vector3f& _local, Eigen::Vector3f& _world) const
	{
		using namespace Eigen;

		float offset = 1.f;

		constexpr float highMax = 10.f;
		constexpr float lowMax = -7.f;

		constexpr float farMin = 5.f;
		constexpr float farMax = 20.f;

		Vector3f lowCamPos[2] = 
		{
			player->data.attachLowCamera0->GetLocalPosition(),
			player->data.attachLowCamera1->GetLocalPosition()
		};

		Vector3f defaultCamPos[2] =
		{
			player->data.attachDefaultCamera0->GetLocalPosition(),
			player->data.attachDefaultCamera1->GetLocalPosition()
		};

		Vector3f highCamPos[2] =
		{
			player->data.attachHighCamera0->GetLocalPosition(),
			player->data.attachHighCamera1->GetLocalPosition()
		};

		Vector3f objPos = selectBody->GetPosition();
		Vector3f playerPos = player->data.modelCore->GetWorldPosition();

		Vector3f direction = objPos - playerPos;
		const float dy = direction.y();
		direction.y() = 0.f;
		const float dz = targetPosition.z();

		Vector3f* activeCamPos = (dy > 0.f) ? highCamPos : lowCamPos;
		float totalY = (dy > 0.f) ? highMax : lowMax;

		float ratioY = std::clamp(std::fabs(dy) / std::fabs(totalY), 0.f, 1.f);
		float totalZ = farMax - farMin;
		float ratioZ = std::clamp(dz / totalZ, 0.f, 1.f);

		auto& math = PurahEngine::PurahMath::GetInstance();

		Vector3f interpolatedAF = player->lerp(activeCamPos[0], activeCamPos[1], ratioZ);
		Vector3f interpolatedDC = player->lerp(defaultCamPos[0], defaultCamPos[1], ratioZ);
		Vector3f finalInterpolatedCamera = player->lerp(interpolatedDC, interpolatedAF, ratioY);

		Affine3f world{ player->data.cameraArm->GetWorldMatrix() };
		world.translate(finalInterpolatedCamera);

		_local = finalInterpolatedCamera;
		_world = world.translation();
	}

	void AttachHoldState::UpdateHoldingCameraRotation() const
	{
		using namespace Eigen;

		Quaternionf modelRot = player->data.modelCore->GetLocalRotation();
		player->data.cameraArm->SetLocalRotation(modelRot);

		float offset = 1.f;

		constexpr float highMax = 10.f;
		constexpr float lowMax = -7.f;

		constexpr float farMin = 10.f;
		constexpr float farMax = 20.f;

		Quaternionf lowCamRot[2] =
		{
			player->data.attachLowCamera0->GetLocalRotation(),
			player->data.attachLowCamera1->GetLocalRotation()
		};

		Quaternionf defaultCamRot[2] =
		{
			player->data.attachDefaultCamera0->GetLocalRotation(),
			player->data.attachDefaultCamera1->GetLocalRotation()
		};

		Quaternionf highCamRot[2] =
		{
			player->data.attachHighCamera0->GetLocalRotation(),
			player->data.attachHighCamera1->GetLocalRotation()
		};

		Vector3f objPos = selectBody->GetPosition();
		Vector3f playerPos = player->data.modelCore->GetWorldPosition();

		Vector3f direction = objPos - playerPos;
		const float dy = direction.y();
		direction.y() = 0.f;
		const float dz = targetPosition.z();

		Quaternionf* activeCamRot = (dy > 0.f) ? highCamRot : lowCamRot;
		float totalY = (dy > 0.f) ? highMax : lowMax;

		float ratioY = std::clamp(std::fabs(dy) / std::fabs(totalY), 0.f, 1.f);
		float totalZ = farMax - farMin;
		float ratioZ = std::clamp(dz / totalZ, 0.f, 1.f);

		auto& math = PurahEngine::PurahMath::GetInstance();

		Quaternionf interpolatedAF = activeCamRot[0].slerp(ratioZ, activeCamRot[1]);
		Quaternionf interpolatedDC = defaultCamRot[0].slerp(ratioZ, defaultCamRot[1]);
		Quaternionf finalInterpolatedCamera = interpolatedDC.slerp(ratioY, interpolatedAF);

		player->SetCameraCoreLocalTargetRotation(finalInterpolatedCamera);
	}

	void AttachHoldState::CameraReset() const
	{
		player->ResetCameraCore();
	}

	void AttachHoldState::Cancel() const
	{
		player->ChangeAbilityState(Player::AbilityState::DEFAULT);
	}

	bool AttachHoldState::TrySelect()
	{
		attachble = player->data.holdObject;
		selectBody = player->data.holdObjectBody;

		if (!attachble || !selectBody)
		{
			player->ChangeAbilityState(Player::ATTACH_SELECT);
			return false;;
		}
		else
		{
			if (selectBody->GetGameObject()->tag.IsContain(L"Phyzzle Player"))
			{
				player->ChangeAbilityState(Player::ATTACH_SELECT);
				return false;;
			}
			else
			{
				VariableSet();
				return true;
			}
		}
	}

	void AttachHoldState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
#if _DEBUG
		debugVector1 = _to.normalized();
#endif _DEBUG

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachHoldState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
#if _DEBUG
		debugVector2 = _to.normalized();
#endif _DEBUG

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

	//void AttachHoldState::CalculateSpringForces()
	//{
	//	constexpr float zeta = 0.5f;												// Damping ratio
	//	constexpr float zeta0 = 0.1f;												// Damping ratio
	//	constexpr float omega = 2.0f * std::numbers::pi_v<float> * 5.0f;			// Angular frequency (5 Hz)
	//	const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();	// Time step
	//
	//	{
	//		if (attachble)
	//		{
	//			using namespace Eigen;
	//
	//			const Vector3f pos = player->data.modelCore->GetWorldPosition();
	//			const Quaternionf rot = player->data.modelCore->GetWorldRotation();
	//			Affine3f transform = Affine3f::Identity();
	//			transform.translate(pos);
	//			transform.rotate(rot); 
	//			Affine3f invTrans = transform.inverse();
	//
	//			ZonaiPhysics::ZnBound3 bound = AttachSystem::Instance()->CalculateBoundingBox(attachble, invTrans.matrix());
	//			
	//			if (bound.minimum.z() < 0.5f)
	//			{
	//				float distance = 0.5f - bound.minimum.z();
	//
	//				targetPosition.z() += distance;
	//			}
	//		}
	//	}
	//
	//	{
	//		Eigen::Vector3f currPos = selectBody->GetPosition();
	//		currPos.y() = 0.f;				// stick R �Է����� ������Ʈ�� ������ ���� �������� �ƴϹǷ�..
	//
	//		Eigen::Vector3f playerPos = player->data.modelCore->GetWorldPosition();
	//		playerPos.y() = 0.f;
	//		Eigen::Vector3f zPos = player->data.modelCore->GetFront() * targetPosition.z();
	//		zPos.y() = 0.f;
	//		const Eigen::Vector3f targetP = playerPos + zPos;
	//
	//		posSpring.Update(
	//			currPos,
	//			springL,
	//			targetP,
	//			zeta,
	//			omega,
	//			timeStep
	//		);
	//	}
	//
	//	{
	//		Eigen::Quaternionf currRot = selectBody->GetRotation();
	//		const Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();
	//		const Eigen::Quaternionf targetR = playerRot * targetRotation;
	//
	//		PurahEngine::GraphicsManager::GetInstance().DrawString(
	//			L"���� ȸ�� : " +
	//			std::to_wstring(currRot.w()) + L"\n" +
	//			std::to_wstring(currRot.x()) + L"\n" +
	//			std::to_wstring(currRot.y()) + L"\n" +
	//			std::to_wstring(currRot.z()) + L"\n",
	//			1200, 300,
	//			200, 600, 15,
	//			255, 255, 255, 255);
	//
	//		PurahEngine::GraphicsManager::GetInstance().DrawString(
	//			L"Ÿ�� ȸ�� : " +
	//			std::to_wstring(targetR.w()) + L"\n" +
	//			std::to_wstring(targetR.x()) + L"\n" +
	//			std::to_wstring(targetR.y()) + L"\n" +
	//			std::to_wstring(targetR.z()) + L"\n",
	//			1200, 500,
	//			200, 600, 15,
	//			255, 255, 255, 255);
	//
	//		using namespace Eigen;
	//		// Vector3f r = selectBody->GetAngularVelocity();
	//
	//		quatSpring.Update(
	//			currRot,
	//			springR,
	//			targetR.normalized(),
	//			zeta0,
	//			timeStep);
	//
	//		// selectBody->SetAngularVelocity(r);
	//		selectBody->SetRotation(currRot);
	//	}
	//
	//}

	void AttachHoldState::CalculateSpringForces()
	{
		CalculateSpringPosition();
		CalculateSpringRotation();
	}

	void AttachHoldState::CalculateSpringPosition()
	{
		constexpr float zeta = 0.5f;
		constexpr float omega = 2.0f * std::numbers::pi_v<float> *5.0f;
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (attachble)
		{
			using namespace Eigen;

			const Vector3f pos = player->data.modelCore->GetWorldPosition();
			const Quaternionf rot = player->data.modelCore->GetWorldRotation();
			Affine3f transform = Affine3f::Identity();
			transform.translate(pos);
			transform.rotate(rot);
			Affine3f invTrans = transform.inverse();

			ZonaiPhysics::ZnBound3 bound = AttachSystem::Instance()->CalculateBoundingBox(attachble, invTrans.matrix());

			if (bound.minimum.z() < 0.5f)
			{
				float distance = 0.5f - bound.minimum.z();
				targetPosition.z() += distance;
			}
		}

		Eigen::Vector3f currPos = selectBody->GetPosition();
		currPos.y() = 0.f;

		Eigen::Vector3f playerPos = player->data.modelCore->GetWorldPosition();
		playerPos.y() = 0.f;
		Eigen::Vector3f zPos = player->data.modelCore->GetFront() * targetPosition.z();
		zPos.y() = 0.f;
		const Eigen::Vector3f targetP = playerPos + zPos;

		posSpring.Update(currPos, springL, targetP, zeta, omega, timeStep);
	}

	void AttachHoldState::CalculateSpringRotation()
	{
		constexpr float zeta0 = 0.1f;
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		Eigen::Quaternionf currRot = selectBody->GetRotation();
		const Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();
		const Eigen::Quaternionf targetR = playerRot * targetRotation;

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"���� ȸ�� : " +
			std::to_wstring(currRot.w()) + L"\n" +
			std::to_wstring(currRot.x()) + L"\n" +
			std::to_wstring(currRot.y()) + L"\n" +
			std::to_wstring(currRot.z()) + L"\n",
			1200, 300, 200, 600, 15, 255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"Ÿ�� ȸ�� : " +
			std::to_wstring(targetR.w()) + L"\n" +
			std::to_wstring(targetR.x()) + L"\n" +
			std::to_wstring(targetR.y()) + L"\n" +
			std::to_wstring(targetR.z()) + L"\n",
			1200, 500, 200, 600, 15, 255, 255, 255, 255);

		quatSpring.Update(currRot, springR, targetR.normalized(), zeta0, timeStep);

		selectBody->SetRotation(currRot);
	}

	void AttachHoldState::TranslateSpringAlongY(float _distance)
	{
		targetPosition.y() += _distance;
	}

	void AttachHoldState::TranslateSpringAlongZ(float _distance)
	{
		targetPosition.z() += _distance;
	}

	void AttachHoldState::TranslateObject(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachHoldState::TranslateObjectAlongX(float _distance)
	{
		const Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const Eigen::Vector3f objPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objPos - playerPos;
		forward.y() = 0.f;
		forward.normalize();
		const Eigen::Vector3f right = Eigen::Vector3f::UnitY().cross(forward);

		TranslateObject(right.normalized(), _distance);
	}

	void AttachHoldState::TranslateObjectAlongY(float _distance)
	{
		TranslateObject(Eigen::Vector3f::UnitY(), _distance);
	}

	void AttachHoldState::TranslateObjectAlongZ(float _distance)
	{
		const auto playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const auto objPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objPos - playerPos;
		forward.y() = 0.f;
		forward.normalize();

		TranslateObject(forward, _distance);
	}

	void AttachHoldState::RotateWithSpring(const Eigen::Vector3f& _axis, float _angle)
	{
		targetRotation = Eigen::Quaternionf(Eigen::AngleAxisf{ _angle, _axis }) * targetRotation;
	}

	void AttachHoldState::RotateSpringAlongX(float _angle)
	{
		Eigen::Vector3f dir = Eigen::Vector3f::UnitX();

		RotateWithSpring(dir, _angle);
	}

	void AttachHoldState::RotateSpringAlongY(float _angle)
	{
		const Eigen::Vector3f worldUp = Eigen::Vector3f::UnitY();

		RotateWithSpring(worldUp, _angle);
	}

	bool AttachHoldState::TryAttach() const
	{
		if (!attachble)
			return false;

		return AttachSystem::Instance()->TryAttach(attachble);
	}

	bool AttachHoldState::TryDettach() const
	{
		if (!attachble)
			return false;

		return AttachSystem::Instance()->Dettach(attachble);
	}

	void AttachHoldState::Put() const
	{
		if (!attachble)
			return;

		AttachSystem::Instance()->DeselectBody(attachble);
	}

	void AttachHoldState::EnableOutline(bool _value) const
	{
		if (!selectBody || !attachble)
			return;

		if (_value)
		{
			AttachSystem::Instance()->EnableOutline(attachble, &player->color0, &player->color1);
		}
		else
		{
			AttachSystem::Instance()->DisableOutline(attachble);
		}
	}

	void AttachHoldState::Snap()
	{
		using namespace Eigen;

		const Quaternionf objectRotation = selectBody->GetRotation();
		const Quaternionf playerRotation = player->data.modelCore->GetWorldRotation();

		const Vector3f objectPosition = selectBody->GetPosition();
		const Vector3f playerPosition = player->data.modelCore->GetWorldPosition();

		Vector3f playerToObject = objectPosition - playerPosition;
		playerToObject.y() = 0.f;
		playerToObject.normalize();

		// ������Ʈ-�÷��̾� ����� ������ ���� ã��
		Quaternionf test = playerRotation.inverse() * objectRotation;
		Quaternionf front = FindAxis(test);
		targetRotation = front;

		targetRotation.normalize();
	}

	Eigen::Quaternionf AttachHoldState::FindAxis(const Eigen::Quaternionf& _direction)
	{
		using namespace Eigen;

		Quaternionf result;
		int size = axisies.size();

		float max = 0.f;
		for (int i = 0; i < size; i++)
		{
			float cosTheta = 0.f;
			const auto& [axis, data] = axisies[i];
			Quaternionf dir = axis.normalized();
			Quaternionf minus = Quaternionf(-dir.w(), -dir.x(), -dir.y(), -dir.z());

			cosTheta = _direction.dot(dir);									// �ึ�� ������ �����
			cosTheta = std::clamp(cosTheta, -1.f, 1.f);

			if (max <= cosTheta)								// ���� ����� ���� ã��
			{
				if (result != dir)
				{
					result = dir;
					info = data;
					max = cosTheta;
				}
			}

			cosTheta = _direction.dot(minus);									// �ึ�� ������ �����
			cosTheta = std::clamp(cosTheta, -1.f, 1.f);

			if (max <= cosTheta)								// ���� ����� ���� ã��
			{
				if (result != dir)
				{
					result = dir;
					info = data;
					max = cosTheta;
				}
			}
		}

 		return result;
	}

	void AttachHoldState::VariableSet()
	{
		using namespace Eigen;

		if (!attachble || !selectBody)
			return;

		AttachSystem::Instance()->SelectBody(attachble);
		EnableOutline(true);

		const Eigen::Vector3f objectPosition = selectBody->GetPosition();
		const Eigen::Vector3f playerPosition = player->data.modelCore->GetWorldPosition();

		Eigen::Vector3f lookTo = objectPosition - playerPosition;
		targetPosition.y() = lookTo.y();
		lookTo.y() = 0.f;
		LookToWorldDirection(lookTo);
		targetPosition.z() = lookTo.norm();

		info.info = None;

		Snap();
	}

	void AttachHoldState::VariableReset()
	{
		using namespace Eigen;

		playerVelocity = Eigen::Vector3f::Zero();

		targetVelocity = Eigen::Vector3f::Zero();
		targetAngularVelocity = Eigen::Vector3f::Zero();

		springL = Eigen::Vector3f::Zero();
		springR = Eigen::Vector3f::Zero();

		targetPosition = Eigen::Vector3f::Zero();
		targetRotation = Eigen::Quaternionf::Identity();

		diffWidth = -0.1f;
		selectBody = nullptr;
		attachble = nullptr;
	}

	bool AttachHoldState::TryTranslate(float _distance)
	{

		return true;
	}
#pragma endregion Content

#pragma region Debug
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
#pragma endregion Debug
}
