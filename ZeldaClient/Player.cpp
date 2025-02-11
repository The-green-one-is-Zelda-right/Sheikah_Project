#include <map>

#include "Player.h"

#include "PlayerController.h"
#include "PlayerStateMachine.h"
#include "PlayerMovement.h"
#include "PlayerAnimation.h"
#include "PlayerCamera.h"
#include "PlayerUIManager.h"

#include "AttachSystem.h"

namespace Phyzzle
{
	Player::~Player()
	{
	}

	void Player::Start()
	{
		InitializePlayerAnimation();
		InitializePlayerCamera();
	}

	void Player::FixedUpdate()
	{

	}

	void Player::Update()
	{
		movement->UpdateMovement();

		controller->HandleInput();					// 입력 처리
		
		stateMachine->UpdateAbilitChangeyState();	// 상태 관리

		if (!data.stopUpdate)
		{
			stateMachine->UpdateAbilityStayState();		// 상태 관리
		}
	}

	void Player::LateUpdate()
	{
		animation->UpdateAnimation();  // 애니메이션 후처리

		stateMachine->PostUpdateAbilityState();  // 상태 관리
		
		camera->UpdateCamera();  // 카메라 위치 보간
	}

	void Player::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{
		movement->PlayerOnMovingPlatform(zn_collision, collider);

		movement->PlayerImpulseCheck(zn_collision, collider);
	}

	void Player::OnCollisionStay(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{
		movement->PlayerOnMovingPlatform(zn_collision, collider);

		movement->PlayerImpulseCheck(zn_collision, collider);
	}

	void Player::OnCollisionExit(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{

	}

	void Player::ChangeAbilityState(AbilityState newState)
	{
		stateMachine->SetAbilityState(newState);
	}

	Phyzzle::AbilityState Player::GetCurrentAbilityState() const
	{
		return stateMachine->CurrentAbilityStateEnum();
	}

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

	void Player::InitializePlayerAnimation()
	{
		// 애니메이션 문자열 맵 초기화
		std::map<AnimationState, std::wstring> animationString = 
		{
			{AnimationState::IDLE, animation->animData.idleAnimation},
			{AnimationState::WALK, animation->animData.walkingAnimation},
			{AnimationState::RUN, animation->animData.runningAnimation},
			{AnimationState::JUMP, animation->animData.jumpAnimation},
			{AnimationState::JUMPING, animation->animData.jumpingAnimation},
			{AnimationState::LANDING, animation->animData.landingAnimation},
			{AnimationState::ABILITY_IDLE, animation->animData.holdIdleAnimation},
			{AnimationState::ABILITY_FRONT, animation->animData.holdFrontAnimation},
			{AnimationState::ABILITY_BACK, animation->animData.holdBackAnimation},
			{AnimationState::ABILITY_RIGHT, animation->animData.holdRightAnimation},
			{AnimationState::ABILITY_LEFT, animation->animData.holdLeftAnimation}
		};

		{
			// 애니메이션 상태 및 속도 컨트롤러 맵 초기화
			for (const auto& [type, animationStr] : animationString)
			{
				animation->AddAnimationState(type, animationStr, data.animator);

				if (type == AnimationState::WALK || type == AnimationState::RUN || type >= AnimationState::ABILITY_FRONT)
				{
					animation->AddAnimationSpeedController(
						type,
						animationStr, data.animator
					);
				}
			}
		}
	}

	void Player::InitializePlayerCamera()
	{

	}

	void Player::InitializePlayerUI()
	{

	}

	void Player::DeserializePlayer(const json& jsonData)
	{
		float moveSpeed = data.moveSpeed;
		PREDESERIALIZE_VALUE(moveSpeed);
		data.moveSpeed = moveSpeed;

		float holdSpeed = data.holdSpeed;
		PREDESERIALIZE_VALUE(holdSpeed);
		data.holdSpeed = holdSpeed;

		float sensitivity = camera->sensitivity;
		PREDESERIALIZE_VALUE(sensitivity);
		camera->sensitivity = sensitivity;

		float jumpPower = data.jumpPower;
		PREDESERIALIZE_VALUE(jumpPower);
		data.jumpPower = jumpPower;

		float slopeLimit = data.slopeLimit;
		PREDESERIALIZE_VALUE(slopeLimit);
		data.slopeLimit = slopeLimit;
	}

	void Player::DeserializePlayerAbility(const json& jsonData)
	{
		auto searchAroundbufferSize = stateMachine->searchAroundbufferSize;
		PREDESERIALIZE_VALUE(searchAroundbufferSize);
		stateMachine->searchAroundbufferSize = searchAroundbufferSize;

		auto searchAroundDistance = stateMachine->searchAroundDistance;
		PREDESERIALIZE_VALUE(searchAroundDistance);
		stateMachine->searchAroundDistance = searchAroundDistance;

		auto searchAroundLayers = stateMachine->searchAroundLayers;
		PREDESERIALIZE_VALUE(searchAroundLayers);
		stateMachine->searchAroundLayers = searchAroundLayers;

		auto attachRaycastLayers = stateMachine->attachRaycastLayers;
		PREDESERIALIZE_VALUE(attachRaycastLayers);
		stateMachine->attachRaycastLayers = attachRaycastLayers;

		float attachRaycastDistance = stateMachine->attachRaycastDistance;
		PREDESERIALIZE_VALUE(attachRaycastDistance);
		stateMachine->attachRaycastDistance = attachRaycastDistance;

		auto targetPositionYSpeed = stateMachine->targetPositionYSpeed;
		PREDESERIALIZE_VALUE(targetPositionYSpeed);
		stateMachine->targetPositionYSpeed = targetPositionYSpeed;

		auto targetPositionZStep = stateMachine->targetPositionZStep;
		PREDESERIALIZE_VALUE(targetPositionZStep);
		stateMachine->targetPositionZStep = targetPositionZStep;

		auto minTargetPositionY = stateMachine->minTargetPositionY;
		PREDESERIALIZE_VALUE(minTargetPositionY);
		stateMachine->minTargetPositionY = minTargetPositionY;

		auto maxTargetPositionY = stateMachine->maxTargetPositionY;
		PREDESERIALIZE_VALUE(maxTargetPositionY);
		stateMachine->maxTargetPositionY = maxTargetPositionY;

		auto minTargetPositionZ = stateMachine->minTargetPositionZ;
		PREDESERIALIZE_VALUE(minTargetPositionZ);
		stateMachine->minTargetPositionZ = minTargetPositionZ;

		auto maxTargetPositionZ = stateMachine->maxTargetPositionZ;
		PREDESERIALIZE_VALUE(maxTargetPositionZ);
		stateMachine->maxTargetPositionZ = maxTargetPositionZ;

		auto targetPositionOffset = stateMachine->targetPositionOffset;
		PREDESERIALIZE_VALUE(targetPositionOffset);
		stateMachine->targetPositionOffset = targetPositionOffset;

		auto linearSpringDamping = stateMachine->linearSpringDamping;
		PREDESERIALIZE_VALUE(linearSpringDamping);
		stateMachine->linearSpringDamping = linearSpringDamping;

		auto linearSpringFrequency = stateMachine->linearSpringFrequency;
		PREDESERIALIZE_VALUE(linearSpringFrequency);
		stateMachine->linearSpringFrequency = linearSpringFrequency;

		auto linearMaxVelocity = stateMachine->linearMaxVelocity;
		PREDESERIALIZE_VALUE(linearMaxVelocity);
		stateMachine->linearMaxVelocity = linearMaxVelocity;

		auto angularSpringDamping = stateMachine->angularSpringDamping;
		PREDESERIALIZE_VALUE(angularSpringDamping);
		stateMachine->angularSpringDamping = angularSpringDamping;

		auto angularSpringFrequency = stateMachine->angularSpringFrequency;
		PREDESERIALIZE_VALUE(angularSpringFrequency);
		stateMachine->angularSpringFrequency = angularSpringFrequency;

		auto angularMaxVelocity = stateMachine->angularMaxVelocity;
		PREDESERIALIZE_VALUE(angularMaxVelocity);
		stateMachine->angularMaxVelocity = angularMaxVelocity;

		auto holdRotateAngle = stateMachine->holdRotateAngle;
		PREDESERIALIZE_VALUE(holdRotateAngle);
		stateMachine->holdRotateAngle = holdRotateAngle;

		auto arcRatio = stateMachine->arcRatio;
		PREDESERIALIZE_VALUE(arcRatio);
		stateMachine->arcRatio = arcRatio;
	}

	void Player::DeserializePlayerAnimation(const json& jsonData)
	{
		auto idleAnimation = animation->animData.idleAnimation;
		PREDESERIALIZE_WSTRING(idleAnimation);
		animation->animData.idleAnimation = idleAnimation;

		auto walkingAnimation = animation->animData.walkingAnimation;
		PREDESERIALIZE_WSTRING(walkingAnimation);
		animation->animData.walkingAnimation = walkingAnimation;

		auto runningAnimation = animation->animData.runningAnimation;
		PREDESERIALIZE_WSTRING(runningAnimation);
		animation->animData.runningAnimation = runningAnimation;

		auto jumpAnimation = animation->animData.jumpAnimation;
		PREDESERIALIZE_WSTRING(jumpAnimation);
		animation->animData.jumpAnimation = jumpAnimation;

		auto jumpingAnimation = animation->animData.jumpingAnimation;
		PREDESERIALIZE_WSTRING(jumpingAnimation);
		animation->animData.jumpingAnimation = jumpingAnimation;

		auto landingAnimation = animation->animData.landingAnimation;
		PREDESERIALIZE_WSTRING(landingAnimation);
		animation->animData.landingAnimation = landingAnimation;

		auto holdIdleAnimation = animation->animData.holdIdleAnimation;
		PREDESERIALIZE_WSTRING(holdIdleAnimation);
		animation->animData.holdIdleAnimation = holdIdleAnimation;

		auto holdFrontAnimation = animation->animData.holdFrontAnimation;
		PREDESERIALIZE_WSTRING(holdFrontAnimation);
		animation->animData.holdFrontAnimation = holdFrontAnimation;

		auto holdBackAnimation = animation->animData.holdBackAnimation;
		PREDESERIALIZE_WSTRING(holdBackAnimation);
		animation->animData.holdBackAnimation = holdBackAnimation;

		auto holdRightAnimation = animation->animData.holdRightAnimation;
		PREDESERIALIZE_WSTRING(holdRightAnimation);
		animation->animData.holdRightAnimation = holdRightAnimation;

		auto holdLeftAnimation = animation->animData.holdLeftAnimation;
		PREDESERIALIZE_WSTRING(holdLeftAnimation);
		animation->animData.holdLeftAnimation = holdLeftAnimation;
	}

	void Player::DeserializePlayerCamera(const json& jsonData)
	{
		int cameraCollisionLayers = camera->cameraCollisionLayers;
		PREDESERIALIZE_VALUE(cameraCollisionLayers);
		camera->cameraCollisionLayers = cameraCollisionLayers;

		float cameraCollisionRadius = camera->cameraCollisionRadius;
		PREDESERIALIZE_VALUE(cameraCollisionRadius);
		camera->cameraCollisionRadius = cameraCollisionRadius;
	}

	void Player::PostDeserializePlayer(const json& jsonData)
	{
		auto playerRigidbody = data.playerRigidbody;
		POSTDESERIALIZE_PTR(playerRigidbody);
		data.playerRigidbody = playerRigidbody;

		auto modelCore = data.modelCore;
		POSTDESERIALIZE_PTR(modelCore);
		data.modelCore = modelCore;

		auto cameraArm = data.cameraArm;
		POSTDESERIALIZE_PTR(cameraArm);
		data.cameraArm = cameraArm;

		auto cameraCore = data.cameraCore;
		POSTDESERIALIZE_PTR(cameraCore);
		data.cameraCore = cameraCore;

		auto animator = data.animator;
		POSTDESERIALIZE_PTR(animator);
		data.animator = animator;

		auto groundCheck = data.groundCheck;
		POSTDESERIALIZE_PTR(groundCheck);
		data.groundCheck = groundCheck;
	}

	void Player::PostDeserializePlayerUI(const json& jsonData)
	{
		auto Attach_Default = uiManager->Attach_Default;
		POSTDESERIALIZE_PTR(Attach_Default);
		uiManager->Attach_Default = Attach_Default;

		auto Attach_Hold_NoneStick = uiManager->Attach_Hold_NoneStick;
		POSTDESERIALIZE_PTR(Attach_Hold_NoneStick);
		uiManager->Attach_Hold_NoneStick = Attach_Hold_NoneStick;

		auto Attach_Hold_Stick = uiManager->Attach_Hold_Stick;
		POSTDESERIALIZE_PTR(Attach_Hold_Stick);
		uiManager->Attach_Hold_Stick = Attach_Hold_Stick;

		auto Rotation_NoneStick = uiManager->Rotation_NoneStick;
		POSTDESERIALIZE_PTR(Rotation_NoneStick);
		uiManager->Rotation_NoneStick = Rotation_NoneStick;

		auto Rotation_Stick = uiManager->Rotation_Stick;
		POSTDESERIALIZE_PTR(Rotation_Stick);
		uiManager->Rotation_Stick = Rotation_Stick;

		auto Catch_B = uiManager->Catch_B;
		POSTDESERIALIZE_PTR(Catch_B);
		uiManager->Catch_B = Catch_B;

		auto Stick_B = uiManager->Stick_B;
		POSTDESERIALIZE_PTR(Stick_B);
		uiManager->Stick_B = Stick_B;

		auto crossHead01 = uiManager->crossHead01;
		POSTDESERIALIZE_PTR(crossHead01);
		uiManager->crossHead01 = crossHead01;

		auto crossHead02 = uiManager->crossHead02;
		POSTDESERIALIZE_PTR(crossHead02);
		uiManager->crossHead02 = crossHead02;

		auto rotationArow = uiManager->rotationArow;
		POSTDESERIALIZE_PTR(rotationArow);
		uiManager->rotationArow = rotationArow;
	}

	void Player::PostDeserializePlayerCamera(const json& jsonData)
	{
		auto attachLowCamera0 = camera->attachLowCamera0;
		POSTDESERIALIZE_PTR(attachLowCamera0);
		camera->attachLowCamera0 = attachLowCamera0;

		auto attachLowCamera1 = camera->attachLowCamera1;
		POSTDESERIALIZE_PTR(attachLowCamera1);
		camera->attachLowCamera1 = attachLowCamera1;

		auto attachDefaultCamera0 = camera->attachDefaultCamera0;
		POSTDESERIALIZE_PTR(attachDefaultCamera0);
		camera->attachDefaultCamera0 = attachDefaultCamera0;

		auto attachDefaultCamera1 = camera->attachDefaultCamera1;
		POSTDESERIALIZE_PTR(attachDefaultCamera1);
		camera->attachDefaultCamera1 = attachDefaultCamera1;

		auto attachHighCamera0 = camera->attachHighCamera0;
		POSTDESERIALIZE_PTR(attachHighCamera0);
		camera->attachHighCamera0 = attachHighCamera0;

		auto attachHighCamera1 = camera->attachHighCamera1;
		POSTDESERIALIZE_PTR(attachHighCamera1);
		camera->attachHighCamera1 = attachHighCamera1;
	}

	void Player::PreDeserialize(const json& jsonData)
	{
		controller = std::make_unique<PlayerController>(this);
		stateMachine = std::make_unique<PlayerStateMachine>(this);
		movement = std::make_unique<PlayerMovement>(this);
		animation = std::make_unique<PlayerAnimation>(this);
		camera = std::make_unique<PlayerCamera>(this);
		uiManager = std::make_unique<PlayerUIManager>(this);

		// Player
		{
			DeserializePlayer(jsonData);
		}

		// Ability
		{
			DeserializePlayerAbility(jsonData);
		}

		// Outline
		{
			Eigen::Vector4f outlineColor0;
			PREDESERIALIZE_VECTOR4F(outlineColor0);

			Eigen::Vector4f outlineColor1;
			PREDESERIALIZE_VECTOR4F(outlineColor1);

			Eigen::Vector4f outlineColor2;
			PREDESERIALIZE_VECTOR4F(outlineColor2);

			Eigen::Vector4f outlineColor3;
			PREDESERIALIZE_VECTOR4F(outlineColor3);
			color3 = outlineColor3;

			AttachSystem::Instance()->SetOutlineColor(&outlineColor0, &outlineColor1, &outlineColor2);
		}

		// Animation
		{
			DeserializePlayerAnimation(jsonData);
		}

		// Camera
		{
			DeserializePlayerCamera(jsonData);
		}
	}

	void Player::PostDeserialize(const json& jsonData)
	{
		{
			PostDeserializePlayer(jsonData);
		}

		{
			PostDeserializePlayerCamera(jsonData);
		}

		{
			PostDeserializePlayerUI(jsonData);
		}
	}
}