#define NOMINMAX
#include <algorithm>
#include <iostream>
#include <cstdio>

#include "PzObject.h"
#include "Rewindable.h"
#include "RewindSystem.h"
#include "AttachSystem.h"
#include "GroundCheck.h"

#include "Player.h"
#include "PlayerAbilityComponent.h"

#include "../GraphicsTest/CoreSystem.h"

namespace Phyzzle
{
	namespace
	{
		void LogPlayerGamePadBinding(int slot)
		{
			char buffer[160] = {};
			if (slot >= 0)
			{
				sprintf_s(buffer, "[Player] bound to XInput slot %d\n", slot);
			}
			else
			{
				sprintf_s(buffer, "[Player] no connected XInput slot found. Falling back to keyboard.\n");
			}
			OutputDebugStringA(buffer);
		}
	}

	Player::~Player()
	{
		delete abilityComponent;
		abilityComponent = nullptr;
	}

#pragma region Initialize
	void Player::InitializeGamePad()
	{
		ResolveGamePadBinding();
	}

	void Player::ResolveGamePadBinding()
	{
		if (gamePad != nullptr && gamePadSlot >= 0 && gamePad->IsConnected())
		{
			if (lastLoggedGamePadSlot != gamePadSlot)
			{
				LogPlayerGamePadBinding(gamePadSlot);
				lastLoggedGamePadSlot = gamePadSlot;
			}
			return;
		}

		for (int i = 0; i < 4; ++i)
		{
			PurahEngine::IGamePad* candidate = PurahEngine::GamePadManager::GetGamePad(i);
			if (candidate != nullptr && candidate->IsConnected())
			{
				PurahEngine::GamePadManager::AddGamePad(i);
				candidate->SetDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
				gamePad = candidate;
				gamePadSlot = i;

				if (lastLoggedGamePadSlot != gamePadSlot)
				{
					LogPlayerGamePadBinding(gamePadSlot);
					lastLoggedGamePadSlot = gamePadSlot;
				}
				return;
			}
		}

		gamePad = nullptr;
		gamePadSlot = -1;
		if (lastLoggedGamePadSlot != -1)
		{
			LogPlayerGamePadBinding(-1);
			lastLoggedGamePadSlot = -1;
		}
	}

	void Player::InitializeDefaultPositions()
	{
		camData.armDefaultPosition = data.cameraArm->GetLocalPosition();
		camData.armDefaultRotation = data.cameraArm->GetLocalRotation();

		camData.coreDefaultPosition = data.cameraCore->GetLocalPosition();
		camData.coreDefaultRotation = data.cameraCore->GetLocalRotation();

		camData.coreTargetRotation = Eigen::Quaternionf::Identity();

		camData.coreSelectPosition = camData.coreDefaultPosition + Eigen::Vector3f{ 0.5f, 0.5f, 0.f };

		highPosition = Eigen::Vector3f(0.f, 0.f, -10.f);
		lowPosition = Eigen::Vector3f(0.f, 0.f, -2.f);
		differenceHigh = highPosition - camData.coreDefaultPosition;
		differenceLow = lowPosition - camData.coreDefaultPosition;
	}

	void Player::InitializeLerpFunctions()
	{
		lerp = [this](const Eigen::Vector3f start, const Eigen::Vector3f end, float _t) -> Eigen::Vector3f
			{
				if (start == end)
					return start;

				_t = std::clamp(_t, 0.f, 1.f);

				return start + _t * (end - start);
			};
	}

	void Player::EnsureRuntimeComponents()
	{
		if (abilityComponent == nullptr)
		{
			abilityComponent = new PlayerAbilityComponent();
		}

		if (abilityComponent != nullptr)
		{
			abilityComponent->BindPlayer(this);
		}
	}

	void Player::InitializeStateSystem()
	{
		{
			// 애니메이션 문자열 맵 초기화
			animationString = {
				{IDLE, animData.idleAnimation},
				{WALK, animData.walkingAnimation},
				{RUN, animData.runningAnimation},
				{JUMP, animData.jumpAnimation},
				{JUMPING, animData.jumpingAnimation},
				{LANDING, animData.landingAnimation},
				{ABILITY_IDLE, animData.holdIdleAnimation},
				{ABILITY_FRONT, animData.holdFrontAnimation},
				{ABILITY_BACK, animData.holdBackAnimation},
				{ABILITY_RIGHT, animData.holdRightAnimation},
				{ABILITY_LEFT, animData.holdLeftAnimation}
			};
		}

		{
			// 애니메이션 상태 및 속도 컨트롤러 맵 초기화
			for (const auto& [type, animation] : animationString)
			{
				AddAnimationState(animationState, type, animation, data.animator);

				if (type == WALK || type == RUN || type >= ABILITY_FRONT)
				{
					AddAnimationSpeedController(
						animationSpeedController, type, 
						animation, data.animator
					);
				}
			}
		}
	}

	// 공통된 애니메이션 상태 추가 함수
	void Player::AddAnimationState(
		std::map<PlayerState, std::function<void()>>& stateMap,
		PlayerState type, const std::wstring& animation,
		PurahEngine::Animator* animator)
	{
		stateMap[type] = [animation, animator]()
			{
				animator->Play(animation);
			};
	}

	// 공통된 애니메이션 속도 컨트롤러 추가 함수
	void Player::AddAnimationSpeedController(
		std::map<PlayerState, std::function<void(float)>>& speedMap,
		PlayerState type, const std::wstring& animation,
		PurahEngine::Animator* animator)
	{
		speedMap[type] = [animation, animator](float speed)
			{
				animator->SetPlaySpeed(animation, speed);
			};
	}
#pragma endregion Initialize

#pragma region Debug
	void Player::DebugDraw()
	{
		using namespace Eigen;

		DrawStateInfo();

		Vector3f currvelo = data.playerRigidbody->GetLinearVelocity();
		float spd = currvelo.norm();

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"Player Current Speed : " + std::to_wstring(spd),
			400, 100,
			100, 100,
			15,
			255, 255, 255, 255
		);

		DrawJumpInfo();
	}

	void Player::DrawStateInfo() const
	{
		std::wstring str = GetAbilityLabel(data.state);
		AbilityState currentState = DEFAULT;
		if (abilityComponent != nullptr)
		{
			currentState = abilityComponent->GetCurrentState();
		}
		std::wstring str0 = GetStateString(currentState);

		PurahEngine::GraphicsManager::GetInstance().DrawString(L"SELETE STATE : " + str, 100, 100, 300, 100, 15, 255, 255, 255, 255);
		PurahEngine::GraphicsManager::GetInstance().DrawString(L"PLAYER STATE : " + str0, 100, 200, 400, 100, 15, 255, 255, 255, 255);
	}

	std::wstring Player::GetStateString(AbilityState state) const
	{
		switch (state)
		{
		case ATTACH_HOLD: return L"ATTACH_HOLD";
		case ATTACH_SELECT: return L"ATTACH_SELECT";
		case REWIND_SELECT: return L"REWIND_SELECT";
		case LOCK_SELECT: return L"LOCK_SELECT";
		default: return L"DEFAULT";
		}
	}

	std::wstring Player::GetAbilityLabel(AbilityState state) const
	{
		switch (state)
		{
		case ATTACH_SELECT:
		case ATTACH_HOLD:
			return L"ATTACH";
		case REWIND_SELECT:
			return L"REWIND";
		case LOCK_SELECT:
			return L"LOCK";
		default:
			return L"NONE";
		}
	}

	void Player::DrawJumpInfo() const
	{
		std::wstring jumpStatus = data.isGrounded ? L"Jumping" : L"can Jump";
		PurahEngine::GraphicsManager::GetInstance().DrawString(jumpStatus, 500, 200, 400, 200, 15, 255, 255, 255, 255);
	}

	void Player::DrawSelectedAbilityUI() const
	{
		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"Selected Ability : " + GetAbilityLabel(data.state),
			40, 40,
			360, 60,
			22,
			255, 255, 255, 255
		);
	}
#pragma endregion Debug

#pragma region Event
	void Player::OnDataLoadComplete()
	{
		EnsureRuntimeComponents();
	}

	void Player::Start()
	{
		InitializeGamePad();
		InitializeDefaultPositions();
		EnsureRuntimeComponents();
		if (abilityComponent != nullptr)
		{
			abilityComponent->InitializeRuntime();
		}
		InitializeStateSystem();
		InitializeLerpFunctions();
		AttachSystem::Instance()->SetOutlineColor(&color0, &color1, &color2);

		stopCount = 0;
	}

	void Player::FixedUpdate()
	{

	}

	void Player::Update()
	{
		using namespace Eigen;

		EnsureRuntimeComponents();
		if (abilityComponent != nullptr)
		{
			abilityComponent->InitializeRuntime();
		}

		RewindSystem::Instance().UpdateRewind(PurahEngine::TimeController::GetInstance().GetDeltaTime());

		data.isGrounded = data.groundCheck->IsGrounded();	// 밟고있는 것 체크
		data.isStandableSlope = SlopeCheck();				// 밟고있는 것 기울기 체크
		
		ApplyImpulse();
		PlayerFlyingUpdate();

		if (!data.isGrounded && abilityComponent != nullptr)
		{
			abilityComponent->CancelCurrentState();
		}

		data.stateChange = abilityComponent != nullptr && abilityComponent->UpdateStateChange();

		if (!data.stopUpdate)
		{
			HandleInput();

			if (!data.stateChange && abilityComponent != nullptr)
				abilityComponent->UpdateStateStay();

			animData.animationSpeed = currInput.Lstick.Size;
		}
	}

	void Player::LateUpdate()
	{
		if (!data.stopUpdate)
		{
			UpdatePlayerAnimationState();			// 이벤트 중에는 애니메이션 업데이트를 안 함

			if (!data.stateChange && abilityComponent != nullptr)
			{
				abilityComponent->PostUpdateState();			// 상태가 바뀐 직후엔 플레이어 업데이트를 안 함
			}
		}

		UpdateCameraLerp();							// 카메라 보간
		CharacterDisable();							// 카메라 위치에 따라서 캐릭터 비활성화
		DrawSelectedAbilityUI();

		if (data.debugMode)
			DebugDraw();

		if (abilityComponent != nullptr)
		{
			abilityComponent->AdvanceFrame();
		}
		prevPlayerState = currPlayerState;
	}

	void Player::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{
		PlayerOnMovingPlatform(zn_collision, collider);

		PlayerImpulseCheck(zn_collision, collider);
	}

	void Player::OnCollisionStay(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{
		PlayerOnMovingPlatform(zn_collision, collider);
		
		PlayerImpulseCheck(zn_collision, collider);
	}

	void Player::OnCollisionExit(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{
		
	}

#pragma endregion Event

	void Player::SetStopUpdate(bool _value)
	{
		if (_value)
		{
			stopCount += 1;

			if (stopCount >= 1)
			{
				if (abilityComponent != nullptr)
				{
					abilityComponent->CancelCurrentState();
				}

				data.stopUpdate = _value;
			}
		}
		else
		{
			stopCount -= 1;

			if (stopCount <= 0)
			{
				if (abilityComponent != nullptr)
				{
					abilityComponent->CancelCurrentState();
				}

				data.stopUpdate = _value;
			}
		}
	}

#pragma region Update
	void Player::UpdatePlayerAnimationState()
	{
		if (prevPlayerState != currPlayerState)
		{
			if (animationState.contains(currPlayerState))
				animationState[currPlayerState]();
		}
		else
		{
			if (animationSpeedController.contains(currPlayerState))
				animationSpeedController[currPlayerState](animData.animationSpeed);
		}
	}
#pragma endregion Update

	void Player::ChangeAbilityState(AbilityState _state)
	{
		EnsureRuntimeComponents();
		if (abilityComponent != nullptr)
		{
			abilityComponent->RequestState(_state);
		}
	}

	void Player::SelectAbility(AbilityState _state)
	{
		if (_state != ATTACH_SELECT && _state != REWIND_SELECT)
		{
			return;
		}

		data.state = _state;

		if (abilityComponent == nullptr)
		{
			return;
		}

		const AbilityState currentState = abilityComponent->GetCurrentState();
		if (currentState != DEFAULT && currentState != ATTACH_HOLD && currentState != _state)
		{
			ChangeAbilityState(_state);
		}
	}

	void Player::SelectNextAbility()
	{
		if (data.state == ATTACH_SELECT)
		{
			SelectAbility(REWIND_SELECT);
			return;
		}

		SelectAbility(ATTACH_SELECT);
	}

	void Player::SelectPreviousAbility()
	{
		if (data.state == REWIND_SELECT)
		{
			SelectAbility(ATTACH_SELECT);
			return;
		}

		SelectAbility(REWIND_SELECT);
	}

	void Player::UseSelectedAbility()
	{
		if (!data.isGrounded)
		{
			return;
		}

		switch (data.state)
		{
		case ATTACH_SELECT:
			ChangeAbilityState(ATTACH_SELECT);
			break;

		case REWIND_SELECT:
			if (RewindSystem::Instance().IsRewinding())
			{
				RewindSystem::Instance().EndRewind();
			}

			ChangeAbilityState(REWIND_SELECT);
			break;

		default:
			break;
		}
	}

	void Player::ChangePlayerAnimationState(PlayerState _state)
	{
		currPlayerState = _state;
	}

#pragma region Input
	void Player::HandleInput()
	{
		EnsureRuntimeComponents();
		ResolveGamePadBinding();
		if (abilityComponent != nullptr)
		{
			abilityComponent->InitializeRuntime();
		}

		if (gamePad != nullptr && gamePad->IsConnected())
		{
			if (abilityComponent != nullptr)
			{
				abilityComponent->HandleGamePadInput();
			}
		}
		else
		{
			HandleKeyboardInput();
		}

		HandleDebugToggle();
	}

	void Player::HandleDebugToggle()
	{
		bool keyCtrl = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_CONTROL);
		bool keyD = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_D);
		bool keyShift = PurahEngine::InputManager::Getinstance().IsKeyReleased(PurahEngine::eKey::eKEY_SHIFT);

		if (keyCtrl && keyD && keyShift)
		{
			data.debugMode = !data.debugMode;
		}
	}

	void Player::HandleKeyboardInput()
	{
		HandleMovementInput();
		if (abilityComponent != nullptr)
		{
			abilityComponent->HandleKeyboardStateInput();
		}

		HandleCameraRotationInput();
		if (abilityComponent != nullptr)
		{
			abilityComponent->HandleKeyboardActionInput();
			abilityComponent->HandleKeyboardAbilityInput();
		}

	}

	void Player::HandleMovementInput()
	{
		bool moveForward = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_W);
		bool moveBackward = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_S);
		bool moveLeft = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_A);
		bool moveRight = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_D);

		currInput.Lstick.Y = moveForward ? 1.0f : (moveBackward ? -1.0f : 0.0f);
		currInput.Lstick.X = moveLeft ? -1.0f : (moveRight ? 1.0f : 0.0f);

		float magnitude = currInput.Lstick.X * currInput.Lstick.X + currInput.Lstick.Y * currInput.Lstick.Y;
		if (magnitude > 0.f)
		{
			magnitude = std::sqrt(magnitude);
			currInput.Lstick.Y /= magnitude;
			currInput.Lstick.X /= magnitude;
		}
		currInput.Lstick.Size = std::clamp(std::sqrt(magnitude), 0.0f, 1.0f);
	}

	void Player::HandleCameraRotationInput()
	{
		bool rotateUp = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_UP);
		bool rotateDown = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_DOWN);
		bool rotateLeft = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_LEFT);
		bool rotateRight = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_RIGHT);

		currInput.Rstick.Y = rotateUp ? 1.0f : (rotateDown ? -1.0f : 0.0f);
		currInput.Rstick.X = rotateLeft ? -1.0f : (rotateRight ? 1.0f : 0.0f);

		float magnitude = currInput.Rstick.X * currInput.Rstick.X + currInput.Rstick.Y * currInput.Rstick.Y;
		if (magnitude > 0.f)
		{
			magnitude = std::sqrt(magnitude);
			currInput.Rstick.Y /= magnitude;
			currInput.Rstick.X /= magnitude;
		}
		currInput.Rstick.Size = std::clamp(std::sqrt(magnitude), 0.0f, 1.0f);
	}

#pragma endregion Input

#pragma region Player
	void Player::PlayerOnMovingPlatform(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		if (collider->GetGameObject()->tag.IsContain(L"MovingGround"))
		{
			PurahEngine::GameObject* gameobj = collider->GetGameObject();

			if (gameobj)
			{
				PurahEngine::RigidBody* groundBody = gameobj->GetComponent<PurahEngine::RigidBody>();

				if (groundBody)
				{
					Eigen::Vector3f movingGroundVelocity = groundBody->GetLinearVelocity();
					
					for (size_t i = 0; i < zn_collision.contactCount; ++i)
					{
						// 충돌 지점이 플레이어의 아래쪽인지 확인
						if (zn_collision.contacts[i].point.y() < data.modelCore->GetWorldPosition().y() + 0.5f)
						{
							if (data.onPlatformVelocity.norm() < movingGroundVelocity.norm())
							{
								data.onPlatformVelocity = movingGroundVelocity;
								break;
							}
						}
					}
				}
			}
		}
	}

	void Player::PlayerImpulseCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		// 충돌한 게임 오브젝트가 "슈터" 태그를 가지고 있는지 확인
		if (collider->GetGameObject()->tag.IsContain(L"Shooter"))
		{
			PurahEngine::GameObject* shooterObj = collider->GetGameObject();
			PurahEngine::RigidBody* shooterBody = shooterObj->GetComponent<PurahEngine::RigidBody>();

			if (shooterBody)
			{
				if (zn_collision.impulses.norm() >= data.impactThreshold)
				{
					// 플레이어를 날려 보내기 위한 힘 계산
					Eigen::Vector3f knockbackForce = zn_collision.impulses;

					// 플레이어의 속도에 힘을 추가하여 날려 보냄
					if (data.playerFlyingVelocity.norm() < knockbackForce.norm())
					{
						data.playerFlyingVelocity = knockbackForce;
						data.flying = true;
					}
				}
			}
		}
	}

	void Player::ApplyImpulse()
	{
		data.playerRigidbody->AddForce(data.playerFlyingVelocity, ZonaiPhysics::Accelration);

		data.playerFlyingVelocity = Eigen::Vector3f::Zero();
	}

	void Player::PlayerFlyingUpdate()
	{
		if (!data.flying)
			return;
		static float acc = 0.f;
		acc += PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (acc >= data.flyingTime)
		{
			data.flying = false;
			acc = 0.f;
		}
	}

	bool Player::SlopeCheck()
	{
		using namespace Eigen;

		if (!data.isGrounded)
			return false;

		Vector3f start = GetGameObject()->GetTransform()->GetWorldPosition();
		Vector3f to = Vector3f(0.f, -1.f, 0.f);
		float distance = 1.f;
		ZonaiPhysics::ZnQueryInfo info;

		bool hit = PurahEngine::Physics::Raycast(start, to, distance, camData.cameraCollisionLayers, info);

		if (!hit)
			return false;

		data.lastGroundNormal = info.normal;

		const Vector3f up = Vector3f::UnitY();
		float cosTheta = up.dot(info.normal);
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		float slope = std::acosf(cosTheta);

		return slope <= data.slopeLimit;
	}

	bool Player::TryJump()
	{
		using namespace Eigen;

		bool jumpable = data.isStandableSlope && data.isGrounded;

		if (jumpable)
		{
			Eigen::Vector3f power = Eigen::Vector3f::UnitY() * data.jumpPower;
			data.playerRigidbody->AddForce(power, ZonaiPhysics::ForceType::Accelration);

			return true;
		}

		return false;
	}

	bool Player::CanMove()
	{
		return data.isGrounded && data.isStandableSlope;
	}

	bool Player::TryPlayerMove(float _moveSpeed)
	{
		using namespace Eigen;

		// 이동 방향 벡터 계산
		const Vector3f cameraFront = data.cameraArm->GetFront();
		const Vector3f forward = Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Vector3f right = Vector3f::UnitY().cross(forward).normalized();
		Vector3f originDirection = forward * currInput.Lstick.Y + right * currInput.Lstick.X;
		Vector3f direction = Vector3f::Zero();
		const float moveSpeed = _moveSpeed * currInput.Lstick.Size;

		// 경사면 여부에 따라 이동 방향 벡터 계산
		if (!data.flying && data.isStandableSlope)
		{
			Vector3f movementRight = data.lastGroundNormal.cross(forward).normalized();
			Vector3f movementForward = movementRight.cross(data.lastGroundNormal).normalized();
			direction = movementForward * currInput.Lstick.Y + movementRight * currInput.Lstick.X;
		}
		else
		{
			direction = originDirection;
		}

		// 목표 속도 벡터 계산
		Vector3f targetVelocity = moveSpeed * direction.dot(originDirection) * direction;

		// 현재 속도 벡터 계산 및 y축 속도 제한
		Vector3f currentVelocity = data.playerRigidbody->GetLinearVelocity();
		currentVelocity.y() = std::clamp(currentVelocity.y(), -data.maxLinearVelocityY, data.maxLinearVelocityY);
		data.playerRigidbody->SetLinearVelocity(currentVelocity);
		currentVelocity.y() = 0.f;

		// 추가 속도 벡터 계산
		Vector3f additionalVelocity = targetVelocity - currentVelocity;

		// 날고 있는 경우
		if (data.flying)
		{
			// 비행 중 추가 속도 벡터 계산 및 적용
			data.playerRigidbody->AddForce(targetVelocity, ZonaiPhysics::Force);
		}
		// 지면에 있는 경우
		else if (data.isGrounded)
		{
			// 지면에 있는 경우 추가 속도 벡터 계산
			additionalVelocity.y() = 0.f;

			// 지면에 있는 경우 힘을 추가하여 속도 조절
			data.playerRigidbody->AddForce(additionalVelocity + data.onPlatformVelocity, ZonaiPhysics::ForceType::Accelration);

		}
		// 공중에 있는 경우
		else
		{
			data.playerRigidbody->AddForce(additionalVelocity * data.playerRigidbody->GetMass(), ZonaiPhysics::Force);
		}

		data.onPlatformVelocity = Vector3f::Zero();

		// 이동 여부 반환
		return currInput.Lstick.Size >= 1e-6;
	}
#pragma endregion Player

	/// \brief 모델을 월드 방향 벡터를 향해 회전
	void Player::LookInWorldDirection(const Eigen::Vector3f& _worldDirection) const
	{
		if (_worldDirection.isZero())
			return;

		// 플레이어의 월드 회전을 구함
		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();
		// Model의 로컬 방향을 구함.
		const Eigen::Vector3f localDirection = parentWorld.conjugate() * _worldDirection;

		LookInLocalDirection(localDirection);
	}

	/// \brief 모델을 Player의 로컬 방향 벡터를 향해 회전
	void Player::LookInLocalDirection(const Eigen::Vector3f& _localDirection) const
	{
		if (_localDirection.isZero())
			return;

		// 로컬 Z와 Local Direction 사이의 쿼터니언을 구함.
		const Eigen::Vector3f worldUp = Eigen::Vector3f::UnitY();
		const Eigen::Vector3f forward = _localDirection.normalized();
		const Eigen::Vector3f right = worldUp.cross(forward).normalized();
		const Eigen::Vector3f up = forward.cross(right).normalized();

		Eigen::Matrix3f rotation;
		rotation.col(0) = right;
		rotation.col(1) = up;
		rotation.col(2) = forward;

		// Model을 회전시킴
		data.modelCore->SetLocalRotation(Eigen::Quaternionf(rotation));
	}

#pragma region Camera
	void Player::UpdateDefaultCamera()
	{
		RotateCameraArm();
		UpdateDefaultCameraCore();
	}

	void Player::UpdateSelectCamera()
	{
		RotateCameraArm();
		UpdateSelectCameraCore();
	}

	// default 상태에서 camera core의 위치를 계산하는 함수 
	void Player::UpdateDefaultCameraCore()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();
		// 각도에 따라 위치를 우선 제한함
		CalculateDefaultCameraCorePosition(localPosition, worldPosition, false);

		// 오브젝트에 충돌하면 처리함
		if (ResolveCameraCollision(localPosition, worldPosition))
		{
			SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void Player::UpdateSelectCameraCore()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();
		// 각도에 따라 위치를 우선 제한함
		CalculateDefaultCameraCorePosition(localPosition, worldPosition, true);

		// 오브젝트에 충돌하면 처리함
		if (ResolveCameraCollision(localPosition, worldPosition))
		{
			SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void Player::CalculateDefaultCameraCorePosition(Eigen::Vector3f& localOut, Eigen::Vector3f& worldOut, bool _isSelect)
	{
		using namespace Eigen;

		const float ease = camData.xAngle >= 0.f ? camData.xAngle / camData.limitHighAngle : camData.xAngle / camData.limitLowAngle;
		auto easingFunc = camData.xAngle >= 0.f ?
			[](float x) { return 1.0f - (1.0f - x) * (1.0f - x); } :
			[](float x) { return 1.0f - std::pow(1.0f - x, 5.0f); };

		const Vector3f& diff = camData.xAngle >= 0.f ? differenceHigh : differenceLow;

		Vector3f offset = _isSelect ? Vector3f{ 0.5f, 0.5f, 0.f } : Vector3f::Zero();
		Vector3f pos = camData.coreDefaultPosition + offset;
		// Vector3f pos = data.coreTargetPosition;
		pos.z() = camData.coreDefaultPosition.z() + diff.z() * easingFunc(ease);

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		world.translate(pos);

		localOut = pos;
		worldOut = world.translation();
	}

	bool Player::ResolveCameraCollision(Eigen::Vector3f& localIn, Eigen::Vector3f& worldIn)
	{
		using namespace Eigen;

		static float prevDistance = FLT_MAX;
		static bool prevHit = false;

		float radius = camData.cameraCollisionRadius;
		const Vector3f coreLocalPos = localIn;
		const Vector3f coreWorldPos = worldIn;

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		const Vector3f worldStart = world.translation();

		Vector3f worldDir = coreWorldPos - worldStart;
		const float dis = worldDir.norm();
		worldDir.normalize();

		if (worldDir.isZero())
		{
			worldDir = data.cameraCore->GetWorldRotation() * -Vector3f::UnitZ();
		}

		unsigned int layers = camData.cameraCollisionLayers;
		ZonaiPhysics::ZnQueryInfo info;

		bool hit = PurahEngine::Physics::Spherecast(
			radius,
			worldStart, Quaternionf::Identity(),
			worldDir, dis,
			layers,
			info
		);

		float targetDistance = dis;

		if (hit)
			targetDistance = info.distance;

		// 보간을 안하는 경우
		if (hit && (targetDistance <= prevDistance))
		{
			// 충돌 했지만 이전 보다 가까운 경우
			prevDistance = targetDistance;

			Vector3f newPosition = worldStart + worldDir * prevDistance;
			Affine3f worldT{ data.cameraArm->GetWorldMatrix() };
			Vector3f localPos = worldT.inverse() * newPosition;

			// 보간을 안하는 경우 타겟포지션과 카메라 포지션을 같게함
			data.cameraCore->SetLocalPosition(localPos);
			camData.coreTargetPosition = localPos;

			localIn = localPos;
			worldIn = newPosition;
		}
		// 보간을 하는 경우
		else
		{
			if (hit && (targetDistance > prevDistance))
			{
				// 충돌 했지만 이전보다 먼 경우
				prevDistance = std::min(prevDistance + camData.smoothingSpeed, targetDistance);
			}
			else if (!hit)
			{
				// 충돌 하지 않은 경우
				prevDistance = std::min(prevDistance + camData.smoothingSpeed, dis);
			}

			Vector3f newPosition = worldStart + worldDir * prevDistance;
			Affine3f worldT{ data.cameraArm->GetWorldMatrix() };
			Vector3f localPos = worldT.inverse() * newPosition;

			camData.coreTargetPosition = localPos;

			localIn = localPos;
			worldIn = newPosition;
		}

		prevHit = hit;

		return hit;
	}

	void Player::UpdateCameraPositionLerp()
	{
		using namespace Eigen;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		Vector3f currPos = data.cameraCore->GetLocalPosition();
		Vector3f target = camData.coreTargetPosition;
		Vector3f diff = target - currPos;
		float dis = diff.norm();

		Vector3f pos = Vector3f::Zero();
		if (dis > 1e-6)
		{
			float inv = dt / camData.cameraLerpTime;
			// float inv = 0.001f / data.cameraLerpTime;
			inv = std::clamp(inv, 0.f, 1.f);
			pos = lerp(currPos, target, inv);
		}
		else
		{
			pos = target;
		}

		data.cameraCore->SetLocalPosition(pos);
	}

	void Player::UpdateCameraRotationLerp()
	{
		using namespace Eigen;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		Quaternionf currRot = data.cameraCore->GetLocalRotation();
		Quaternionf currRotM = Quaternionf(-currRot.w(), -currRot.x(), -currRot.y(), -currRot.z());
		Quaternionf curr = currRot;
		Quaternionf target = camData.coreTargetRotation;

		float costheta0 = currRot.dot(target);
		float costheta1 = currRotM.dot(target);

		if (costheta0 < costheta1)
		{
			curr = currRotM;
		}

		float inv = dt / camData.cameraLerpTime;
		// float inv = 0.001f / data.cameraLerpTime;
		inv = std::clamp(inv, 0.f, 1.f);
		Quaternionf newRot = curr.slerp(inv, target);

		data.cameraCore->SetLocalRotation(newRot);
	}

	void Player::UpdateCameraLerp()
	{
		UpdateCameraPositionLerp();
		UpdateCameraRotationLerp();
	}

	void Player::CharacterDisable()
	{
		using namespace Eigen;

		PurahEngine::Camera* mainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
		Vector3f cameraPos = mainCamera->GetGameObject()->GetTransform()->GetWorldPosition();
		Vector3f cameraArmPos = data.cameraArm->GetWorldPosition();
		Vector3f vec = cameraArmPos - cameraPos;
		float distance = vec.norm();
		PurahEngine::GameObject* obj = data.modelCore->GetGameObject();

		if (distance < 3.f)
		{
			obj->SetEnable(false);
		}
		else
		{
			obj->SetEnable(true);
		}
	}

	void Player::SetCameraCoreLocalTargetPosition(const Eigen::Vector3f& _localPosision)
	{
		camData.coreTargetPosition = _localPosision;
	}

	void Player::SetCameraCoreLocalTargetRotation(const Eigen::Quaternionf& _localRotation)
	{
		camData.coreTargetRotation = _localRotation;
	}

	void Player::SetCameraCoreWorldTargetPosition(const Eigen::Vector3f& _worldPosision)
	{
		using namespace Eigen;

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		Vector3f localPos = world.inverse() * _worldPosision;

		camData.coreTargetPosition = localPos;
	}

	void Player::SetCameraCoreWorldTargetRotation(const Eigen::Quaternionf& _worldRotation)
	{
		using namespace Eigen;

		Quaternion world{ data.cameraArm->GetWorldRotation() };
		Quaternionf localRot = world.inverse() * _worldRotation;

		camData.coreTargetRotation = localRot;
	}

	Eigen::Vector3f Player::CalculateCameraFocusPosition(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction)
	{
		using namespace Eigen;

		Vector3f xy;
		Vector3f xz;
		bool hasXY = IntersectXYPlane(cameraPos, direction, xy);
		bool hasXZ = IntersectXZPlane(cameraPos, direction, xz);

		if (hasXY && hasXZ)
		{
			if (xy.z() < xz.z())
			{
				return xy;
			}
			else
			{
				return xz;
			}
		}
		else if (hasXY)
		{
			return xy;
		}
		else
		{
			return xz;
		}
	}

	bool Player::IntersectXYPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out)
	{
		using namespace Eigen;

		if (std::fabs(direction.z()) < 1e-6)
		{
			return false;
		}

		float t = -cameraPos.z() / direction.z();
		out = cameraPos + t * direction;

		return true;
	}

	bool Player::IntersectXZPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out)
	{
		using namespace Eigen;

		if (std::fabs(direction.y()) < 1e-6)
		{
			return false;
		}

		float t = -cameraPos.y() / direction.y();
		out = cameraPos + t * direction;

		return true;
	}

	void Player::ResetCameraArmAndCameraCore()
	{
		using namespace Eigen;

		Vector3f corePos = data.cameraCore->GetWorldPosition();
		Quaternionf coreRot = data.cameraCore->GetWorldRotation();

		camData.xAngle = 0.f;
		data.cameraArm->SetLocalPosition(camData.armDefaultPosition);
		data.cameraArm->SetLocalRotation(data.modelCore->GetLocalRotation());

		data.cameraCore->SetWorldRotation(coreRot);
		data.cameraCore->SetWorldPosition(corePos);
	}

	void Player::ResetCameraCoreTarget()
	{
		SetCameraCoreLocalTargetPosition(camData.coreDefaultPosition);
		SetCameraCoreLocalTargetRotation(camData.coreDefaultRotation);
	}

	void Player::ResetCamera()
	{
		ResetCameraArm();
		ResetCameraCore();
	}

	void Player::ResetCameraArm()
	{
		camData.xAngle = 0.f;
		data.cameraArm->SetLocalPosition(camData.armDefaultPosition);
		data.cameraArm->SetLocalRotation(data.modelCore->GetLocalRotation());
	}

	void Player::ResetCameraCore()
	{
		data.cameraCore->SetLocalPosition(camData.coreDefaultPosition);
		data.cameraCore->SetLocalRotation(camData.coreDefaultRotation);
	}

	void Player::SetCameraArmFoward(const Eigen::Vector3f& _direction)
	{
		using namespace Eigen;

		Eigen::Vector3f up{ Vector3f::UnitY() };
		Eigen::Vector3f right = up.cross(_direction).normalized();

		Eigen::Vector3f newUp = _direction.cross(right).normalized();

		Eigen::Matrix3f rotation;
		rotation.col(0) = right;
		rotation.col(1) = newUp;
		rotation.col(2) = _direction;

		data.cameraArm->SetWorldRotation(Eigen::Quaternionf(rotation));

		float cosTheta = _direction.dot(up);
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		float theta = std::acosf(cosTheta);
		camData.xAngle = theta * (180.f / std::numbers::pi_v<float>);
	}

	void Player::RotateCameraArm()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();
		const float angle = data.sensitivity;

		const float xFactor = currInput.Rstick.X * currInput.Rstick.Size;
		const float yawAngle = xFactor * angle * deltaTime;
		RotateCameraArmYaw(yawAngle);

		const float yFactor = -currInput.Rstick.Y * currInput.Rstick.Size;
		const float pitchAngle = yFactor * angle * deltaTime;
		RotateCameraArmPitch(pitchAngle);
	}

	void Player::RotateCameraArmYaw(float yawAngle)
	{
		data.cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
	}

	void Player::RotateCameraArmPitch(float pitchAngle)
	{
		float deltaAngle = pitchAngle;
		camData.xAngle += pitchAngle;

		if (camData.xAngle > camData.limitHighAngle)
		{
			deltaAngle -= (camData.xAngle - camData.limitHighAngle);
			camData.xAngle = camData.limitHighAngle;
		}
		else if (camData.xAngle < camData.limitLowAngle)
		{
			deltaAngle -= (camData.xAngle - camData.limitLowAngle);
			camData.xAngle = camData.limitLowAngle;
		}

		const Eigen::Vector3f cameraRight = data.cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();
		data.cameraArm->Rotate(cameraRight, deltaAngle);
	}

	void Player::CameraLookTo(const Eigen::Vector3f& _direction)
	{
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), _direction);

		data.cameraArm->SetWorldRotation(targetRotation);
	}

	void Player::CameraLookAt(const Eigen::Vector3f& _position)
	{
		using namespace Eigen;

		Vector3f pos = data.cameraCore->GetWorldPosition();
		Vector3f direction = _position - pos;
		direction.normalize();

		Eigen::Vector3f up(0.0f, 1.0f, 0.0f);
		Eigen::Vector3f right = up.cross(direction).normalized();

		Eigen::Vector3f newUp = direction.cross(right).normalized();

		Eigen::Matrix3f rotation;
		rotation.col(0) = right;
		rotation.col(1) = newUp;
		rotation.col(2) = direction;

		data.cameraCore->SetWorldRotation(Eigen::Quaternionf(rotation));
	}
#pragma endregion Camera

#define PLAYER_PREDESERIALIZE_FIELD(target, field) \
	do \
	{ \
		auto field = (target).field; \
		PREDESERIALIZE_VALUE(field); \
		(target).field = field; \
	} while (false)

#define PLAYER_PREDESERIALIZE_WSTRING_FIELD(target, field) \
	do \
	{ \
		auto field = (target).field; \
		PREDESERIALIZE_WSTRING(field); \
		(target).field = field; \
	} while (false)

#define PLAYER_POSTDESERIALIZE_PTR_FIELD(target, field) \
	do \
	{ \
		auto field = (target).field; \
		POSTDESERIALIZE_PTR(field); \
		(target).field = field; \
	} while (false)

#pragma region 직렬화
	void Player::PreSerialize(json& jsonData) const
	{}

	void Player::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		// Player
		{
			PLAYER_PREDESERIALIZE_FIELD(data, moveSpeed);
			PLAYER_PREDESERIALIZE_FIELD(data, holdSpeed);
			PLAYER_PREDESERIALIZE_FIELD(data, sensitivity);
			PLAYER_PREDESERIALIZE_FIELD(data, jumpPower);
			PLAYER_PREDESERIALIZE_FIELD(data, slopeLimit);
		}

		// Ability
		{
			PLAYER_PREDESERIALIZE_FIELD(abilData, searchAroundbufferSize);
			PLAYER_PREDESERIALIZE_FIELD(abilData, searchAroundDistance);
			PLAYER_PREDESERIALIZE_FIELD(abilData, searchAroundLayers);
			PLAYER_PREDESERIALIZE_FIELD(abilData, attachRaycastLayers);
			PLAYER_PREDESERIALIZE_FIELD(abilData, attachRaycastDistance);
			PLAYER_PREDESERIALIZE_FIELD(abilData, targetPositionYSpeed);
			PLAYER_PREDESERIALIZE_FIELD(abilData, targetPositionZStep);
			PLAYER_PREDESERIALIZE_FIELD(abilData, minTargetPositionY);
			PLAYER_PREDESERIALIZE_FIELD(abilData, maxTargetPositionY);
			PLAYER_PREDESERIALIZE_FIELD(abilData, minTargetPositionZ);
			PLAYER_PREDESERIALIZE_FIELD(abilData, maxTargetPositionZ);
			PLAYER_PREDESERIALIZE_FIELD(abilData, targetPositionOffset);
			PLAYER_PREDESERIALIZE_FIELD(abilData, linearSpringDamping);
			PLAYER_PREDESERIALIZE_FIELD(abilData, linearSpringFrequency);
			PLAYER_PREDESERIALIZE_FIELD(abilData, linearMaxVelocity);
			PLAYER_PREDESERIALIZE_FIELD(abilData, angularSpringDamping);
			PLAYER_PREDESERIALIZE_FIELD(abilData, angularSpringFrequency);
			PLAYER_PREDESERIALIZE_FIELD(abilData, angularMaxVelocity);
			PLAYER_PREDESERIALIZE_FIELD(abilData, holdRotateAngle);
			PLAYER_PREDESERIALIZE_FIELD(abilData, arcRatio);
		}

		// Outline
		{
			Eigen::Vector4f outlineColor0;
			PREDESERIALIZE_VECTOR4F(outlineColor0);
			color0 = outlineColor0;

			Eigen::Vector4f outlineColor1;
			PREDESERIALIZE_VECTOR4F(outlineColor1);
			color1 = outlineColor1;

			Eigen::Vector4f outlineColor2;
			PREDESERIALIZE_VECTOR4F(outlineColor2);
			color2 = outlineColor2;

			Eigen::Vector4f outlineColor3;
			PREDESERIALIZE_VECTOR4F(outlineColor3);
			color3 = outlineColor3;
		}

		// Camera
		{
			PLAYER_PREDESERIALIZE_FIELD(camData, cameraCollisionLayers);
			PLAYER_PREDESERIALIZE_FIELD(camData, cameraCollisionRadius);
		}

		// Animation
		{
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, idleAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, walkingAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, runningAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, jumpAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, jumpingAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, landingAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, holdIdleAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, holdFrontAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, holdBackAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, holdRightAnimation);
			PLAYER_PREDESERIALIZE_WSTRING_FIELD(animData, holdLeftAnimation);
		}
	}

	void Player::PostSerialize(json& jsonData) const
	{}

	void Player::PostDeserialize(const json& jsonData)
	{
		{
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, playerRigidbody);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, modelCore);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, cameraArm);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, cameraCore);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, animator);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, crossHead01);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, crossHead02);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, groundCheck);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(data, rotationArow);
		}

		{
			PLAYER_POSTDESERIALIZE_PTR_FIELD(camData, attachLowCamera0);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(camData, attachLowCamera1);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(camData, attachDefaultCamera0);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(camData, attachDefaultCamera1);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(camData, attachHighCamera0);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(camData, attachHighCamera1);
		}

		{
			PLAYER_POSTDESERIALIZE_PTR_FIELD(uiData, Attach_Default);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(uiData, Attach_Hold_NoneStick);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(uiData, Attach_Hold_Stick);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(uiData, Rotation_NoneStick);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(uiData, Rotation_Stick);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(uiData, Catch_B);
			PLAYER_POSTDESERIALIZE_PTR_FIELD(uiData, Stick_B);
		}
	}
#pragma endregion 직렬화

#undef PLAYER_PREDESERIALIZE_FIELD
#undef PLAYER_PREDESERIALIZE_WSTRING_FIELD
#undef PLAYER_POSTDESERIALIZE_PTR_FIELD

#pragma region 플레이어SFX
	void Player::PlayFootStep()
	{
		GetGameObject()->GetComponent<PurahEngine::AudioSource>()->PlayAudio(L"footstep.mp3");
	}

	void Player::PlayJumping()
	{
		GetGameObject()->GetComponent<PurahEngine::AudioSource>()->PlayAudio(L"jumping.wav");
	}

	void Player::PlayLanding()
	{
		GetGameObject()->GetComponent<PurahEngine::AudioSource>()->PlayAudio(L"landing.wav");
	}
#pragma endregion 플레이어SFX
}
