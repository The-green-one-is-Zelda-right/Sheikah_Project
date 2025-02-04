#pragma once
#include <set>

#include "PurahEngine.h"

namespace ZonaiPhysics
{
	struct ZnQueryInfo;
	class Collider;
}

namespace Phyzzle
{
	class Rewindable;
	class PzObject;
	class Holder;
	class IState;
	class GroundCheck;

	class Player final : public PurahEngine::Component
	{
	public:
		~Player() override;

#pragma region Enum
		enum PlayerState
		{
			IDLE,
			WALK,
			RUN,
			JUMP,
			JUMPING,
			LANDING,
			ABILITY_IDLE,
			ABILITY_FRONT,
			ABILITY_BACK,
			ABILITY_RIGHT,
			ABILITY_LEFT,
			EVENT,
		};

		enum AbilityState
		{
			ATTACH_HOLD = -1,	// 물건을 든 상태

			DEFAULT = 0,	// 기본 상태
			ATTACH_SELECT = 1,	// 물건을 부착하려는 상태
			REWIND_SELECT = 2,	// 물건을 되돌리려는 상태
			LOCK_SELECT = 3
		};
#pragma endregion Enum

	private:
#pragma region Struct
		struct PlayerData;
		struct AbilityData;
		struct StickData;
		struct PlayerInput;

#pragma region Input
		struct StickData
		{
			float X;
			float Y;
			float Size;

			const StickData& operator=(const StickData& _other)
			{
				if (this != &_other)
				{
					X = _other.X;
					Y = _other.Y;
					Size = _other.Size;
				}

				return *this;
			}
			const StickData& operator=(StickData&& _other) noexcept
			{
				if (this != &_other)
				{
					X = std::move(_other.X);
					Y = std::move(_other.Y);
					Size = std::move(_other.Size);
				}

				return *this;
			}
		};
#pragma endregion Input

#pragma region Camera
		struct CameraData
		{
			PurahEngine::Transform* attachLowCamera0;
			PurahEngine::Transform* attachLowCamera1;
			PurahEngine::Transform* attachDefaultCamera0;
			PurahEngine::Transform* attachDefaultCamera1;
			PurahEngine::Transform* attachHighCamera0;
			PurahEngine::Transform* attachHighCamera1;

			float cameraLerpTime = 0.5f;			// 보간 시간
			float cameraLerpTime0 = 1.0f;

			Eigen::Vector3f		coreDefaultPosition;
			Eigen::Quaternionf	coreDefaultRotation;

			Eigen::Vector3f		coreSelectPosition;

			Eigen::Vector3f		armDefaultPosition;
			Eigen::Quaternionf	armDefaultRotation;

			Eigen::Vector3f coreTargetPosition;
			Eigen::Quaternionf	coreTargetRotation;

			Eigen::Vector3f armTargetPosition;
			Eigen::Quaternionf	armTargetRotation;

			float xAngle = 0.f;					// 현재 앵글
			const float limitHighAngle = 80.f;	// 하이 앵글
			const float limitLowAngle = -70.f;	// 로우 앵글

			unsigned int cameraCollisionLayers = 0;
			float cameraCollisionRadius = 5.f;
			float smoothingSpeed = 0.2f;
		};
#pragma endregion Camera

#pragma region Player
		struct PlayerData
		{
			bool stateChange = true;

			bool debugMode = false;
			bool stopUpdate = false;

#pragma region Player Variable
			float impactThreshold = 50.f;
			Eigen::Vector3f playerFlyingVelocity;
			float maxLinearVelocityY = 30.f;

			float height = 1.f;
			float radius = 0.5f;

			float moveSpeed = 10.f;				// 기본 속도
			float holdSpeed = 5.f;				// 어태치로 물건 들고 있을 때 움직이는 속도
			float sensitivity = 90.f;			// 카메라 회전 속도
			float jumpPower = 10.f;				// 점프 힘
			float jumpCooldown = 0.1f;
			
			bool isGrounded = false;
			bool flying = false;
			float flyingTime = 0.5f;
			bool isStandableSlope = true;
			Eigen::Vector3f lastGroundNormal;

			Eigen::Vector3f onPlatformVelocity;

			float slopeLimit = 36.f;			// 경사 각도
			float slideFriction = 0.3f;
#pragma endregion Player Variable

#pragma region Player Component
			PurahEngine::RigidBody* playerRigidbody;
			PurahEngine::Transform* modelCore;
			PurahEngine::Transform* cameraArm;
			PurahEngine::Transform* cameraCore;
			PurahEngine::Animator* animator;
			PurahEngine::GameObject* crossHead01;
			PurahEngine::GameObject* crossHead02;
			Phyzzle::GroundCheck* groundCheck;
			PurahEngine::GameObject* rotationArow;

			// UI
			PurahEngine::GameObject* attachSelectUI;
			PurahEngine::GameObject* attachHoldUI;
			PurahEngine::GameObject* attachHoldCollisionUI;
#pragma endregion Player Component

			AbilityState state = ATTACH_SELECT;

			PzObject* holdObject;
			PurahEngine::RigidBody* holdObjectBody;
		};

		struct AbilityData
		{
#pragma region Select
			int searchAroundbufferSize = 64;
			float searchAroundDistance = 60.f;
			unsigned int searchAroundLayers = 0;

			unsigned int attachRaycastLayers = 0;
			float attachRaycastDistance = 40.f;
#pragma endregion Select

#pragma region Hold
			float targetPositionYSpeed = 4.f;
			float targetPositionZStep = 1.f;

			float minTargetPositionY = -4.f;
			float maxTargetPositionY = 10.f;
			float minTargetPositionZ = 1.f;
			float maxTargetPositionZ = 10.f;
			float targetPositionOffset = 1.5f;

			float linearSpringDamping = 3.f;		// 0 <= x <= 1
			float linearSpringFrequency = 100.f;	// 1.f rad/s ~ 10.f rad/s
			float linearMaxVelocity = 80.f;

			float angularSpringDamping = 2.f;
			float angularSpringFrequency = 80.f;
			float angularMaxVelocity = 100.f;

			float holdRotateAngle = 90.f;
			float arcRatio = 1.5f;
#pragma endregion Hold
		};

		struct UIData
		{
			PurahEngine::GameObject* Attach_Default;
			PurahEngine::GameObject* Attach_Hold_NoneStick;
			PurahEngine::GameObject* Attach_Hold_Stick;
			PurahEngine::GameObject* Rotation_NoneStick;
			PurahEngine::GameObject* Rotation_Stick;
			PurahEngine::GameObject* Catch_B;
			PurahEngine::GameObject* Stick_B;
		};

		struct PlayerInput
		{
			StickData Lstick;
			StickData Rstick;

			float LTrigger;
			float RTrigger;

			PlayerInput& operator=(const PlayerInput& _other)
			{
				if (this != &_other)
				{

					Lstick = _other.Lstick;
					Rstick = _other.Rstick;

					LTrigger = _other.LTrigger;
					RTrigger = _other.RTrigger;
				}

				return *this;
			}
			PlayerInput& operator=(PlayerInput&& _other) noexcept
			{
				if (this != &_other)
				{
					Lstick = std::move(_other.Lstick);
					Rstick = std::move(_other.Rstick);

					LTrigger = std::move(_other.LTrigger);
					RTrigger = std::move(_other.RTrigger);
				}

				return *this;
			}
		};
#pragma endregion Player

#pragma region Animation
		struct AnimationData
		{
			std::wstring idleAnimation;
			std::wstring walkingAnimation;
			std::wstring runningAnimation;
			std::wstring jumpAnimation;
			std::wstring jumpingAnimation;
			std::wstring landingAnimation;

			std::wstring holdIdleAnimation;
			std::wstring holdFrontAnimation;
			std::wstring holdBackAnimation;
			std::wstring holdRightAnimation;
			std::wstring holdLeftAnimation;

			float animationSpeed;
		};
#pragma endregion Animation

#pragma endregion Struct

#pragma region Initialize
		void InitializeGamePad();
		void InitializeDefaultPositions();
		void InitializeLerpFunctions();
		void InitializeAbilitySystem();
		void InitializeStateSystem();
		void AddAnimationState(
			std::map<PlayerState, std::function<void()>>& stateMap,
			PlayerState type, const std::wstring& animation,
			PurahEngine::Animator* animator);
		void AddAnimationSpeedController(
			std::map<PlayerState, std::function<void(float)>>& speedMap,
			PlayerState type, const std::wstring& animation,
			PurahEngine::Animator* animator);
#pragma endregion Initialize

#pragma region Debug
		void DebugDraw();
		void DrawStateInfo() const;
		std::wstring GetStateString(AbilityState state) const;
		void DrawJumpInfo() const;
#pragma endregion Debug

	public:
#pragma region Event
		void Start() override;
		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
#pragma endregion Event

		void SetStopUpdate(bool _value);

	private:
#pragma region Update
		bool UpdateAbilitChangeyState();
		void UpdateAbilityStayState();
		void PostUpdateAbilityState();
		void UpdatePlayerAnimationState();
#pragma endregion Update

		void ChangeAbilityState(AbilityState);
		void ChangePlayerAnimationState(PlayerState);


#pragma region Input
		void HandleInput();
		void HandleDebugToggle();

		void HandleGamePadInput();
		void HandleStickInput();
		void HandleTriggerInput();
		void HandleButtonInput();
		void HandleButton(PurahEngine::ePad button, void (IState::* clickFunc)(), void (IState::* pressingFunc)(), void (IState::* upFunc)());

		void HandleKeyboardInput();
		void HandleMovementInput();
		void HandleCameraRotationInput();
		void HandleActionInput();
		void HandleAbilityInput();
#pragma endregion Input

#pragma region Player
		void PlayerOnMovingPlatform(const ZonaiPhysics::ZnCollision& zn_collision,
			const PurahEngine::Collider* collider);
		void PlayerImpulseCheck(const ZonaiPhysics::ZnCollision& zn_collision,
			const PurahEngine::Collider* collider);
		void ApplyImpulse();
		void PlayerFlyingUpdate();
		bool SlopeCheck();
		bool TryJump();
		bool CanMove();
		bool TryPlayerMove(float _moveSpeed);
#pragma endregion Player

		void LookInWorldDirection(const Eigen::Vector3f& _worldDirection) const;
		void LookInLocalDirection(const Eigen::Vector3f& _localDirection) const;

#pragma region Camera
		void UpdateDefaultCamera();					// 카메라 업데이트
		void UpdateSelectCamera();					// 카메라 업데이트

		void UpdateDefaultCameraCore();							// 카메라 코어 업데이트
		void UpdateSelectCameraCore();							// 카메라 코어 업데이트

		/// <summary>
		/// Camera Core 위치 계산
		/// 
		/// 카메라 위치는 Arm의 각도에 의해 계산됨
		/// </summary>
		/// <returns>카메라 코어 로컬 좌표</returns>
		void CalculateDefaultCameraCorePosition(Eigen::Vector3f& localOut, Eigen::Vector3f& worldOut, bool _isSelect);	// 카메라 위치 업데이트

		/// <summary>
		/// 카메라가 지형 지물에 충돌 되는지 체크하고 위치를 변경함
		/// </summary>
		/// <param name="pos">카메라 코어 로컬 좌표</param>
		/// <returns>지형 지물에 부딪치면 true</returns>
		bool ResolveCameraCollision(Eigen::Vector3f& localIn, Eigen::Vector3f& worldIn);

		void UpdateCameraPositionLerp();
		void UpdateCameraRotationLerp();
		void UpdateCameraLerp();

		void CharacterDisable();

		/// <summary>
		/// 카메라 코어의 목표 로컬 좌표를 설정함
		/// 
		/// 현재 좌표를 저장하고 목표 좌표로 보간함
		/// </summary>
		/// <param name="_worldPosision">코어의 로컬 좌표</param>
		void SetCameraCoreLocalTargetPosition(const Eigen::Vector3f& _localPosision);
		void SetCameraCoreLocalTargetRotation(const Eigen::Quaternionf& _localRotation);

		/// <summary>
		/// 카메라 코어의 목표 월드 좌표를 설정함
		/// 
		/// 현재 좌표를 저장하고 목표 좌표로 보간함
		/// </summary>
		/// <param name="_worldPosision">코어의 월드 좌표</param>
		void SetCameraCoreWorldTargetPosition(const Eigen::Vector3f& _worldPosision);
		void SetCameraCoreWorldTargetRotation(const Eigen::Quaternionf& _worldRotation);

		/// <summary>
		/// 카메라 코어가 XY, XZ평면의 어디를 보고 있는지 계산
		/// </summary>
		/// <param name="cameraPos">카메라 코어의 로컬 좌표</param>
		/// <param name="direction">카메라 코어의 로컬 방향 벡터</param>
		/// <returns>카메라 코어의 로컬 XY, XZ 평면의 한 점</returns>
		Eigen::Vector3f CalculateCameraFocusPosition(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction);

		/// <summary>
		/// 카메라 코어가 보고 있는 로컬 XY 평면의 좌표를 계산하는 함수
		/// </summary>
		/// <param name="cameraPos">카메라 코어 로컬 좌표</param>
		/// <param name="direction">카메라 코어 로컬 방향 벡터</param>
		/// <param name="out">계산된 로컬 XY 평면의 좌표</param>
		/// <returns>XY 평면과 평행하면 false</returns>
		bool IntersectXYPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out);

		/// <summary>
		/// 카메라 코어가 보고 있는 로컬 XZ 평면의 좌표를 계산하는 함수
		/// </summary>
		/// <param name="cameraPos">카메라 코어 로컬 좌표</param>
		/// <param name="direction">카메라 코어 로컬 방향 벡터</param>
		/// <param name="out">계산된 로컬 XZ 평면의 좌표</param>
		/// <returns>평면과 평행하면 false</returns>
		bool IntersectXZPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out);

		/// <summary>
		/// 카메라 암과 카메라 코어의 트랜스폼을 초기화 함
		/// </summary>
		void ResetCameraArmAndCameraCore();
		void ResetCameraCoreTarget();
		void ResetCamera();							// 카메라 위치 초기화
		void ResetCameraArm();						// 카메라 암 위치 초기화
		void ResetCameraCore();						// 카메라 코어 위치 초기화

		void SetCameraArmFoward(const Eigen::Vector3f& _direction);
		void RotateCameraArm();							// 카메라 암 회전
		void RotateCameraArmYaw(float yawAngle);		// 카메라 암 yaw 회전
		void RotateCameraArmPitch(float pitchAngle);	// 카메라 암 pitch 회전

		void CameraLookTo(const Eigen::Vector3f& _direction);
		void CameraLookAt(const Eigen::Vector3f& _position);
#pragma endregion Camera

	public:
#pragma region 직렬화
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
#pragma endregion 직렬화

#pragma region 플레이어SFX
	public:
		void PlayFootStep();
		void PlayJumping();
		void PlayLanding();
#pragma endregion 플레이어SFX

	private:
		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		std::unordered_map<AbilityState, IState*> stateSystem;
		std::set<AbilityState> stateChange;
		std::map<PlayerState, std::wstring> animationString;
		std::map<PlayerState, std::function<void()>> animationState;
		std::map<PlayerState, std::function<void(float)>> animationSpeedController;

		AbilityState prevState = DEFAULT;
		AbilityState currState = DEFAULT;
		AbilityState nextState = DEFAULT;

		PlayerState prevPlayerState = IDLE;
		PlayerState currPlayerState = IDLE;

		PurahEngine::IGamePad* gamePad;
		PlayerInput currInput;
		PlayerInput prevInput;

		PlayerData data;
		AbilityData abilData;
		CameraData camData;
		AnimationData animData;
		UIData uiData;
		std::function<Eigen::Vector3f(const Eigen::Vector3f, const Eigen::Vector3f, float)> lerp;

		Eigen::Vector4f color0;
		Eigen::Vector4f color1;
		Eigen::Vector4f color2;
		Eigen::Vector4f color3;

	private:
		int stopCount;

	private:
		Eigen::Vector3f		highPosition;
		Eigen::Vector3f		lowPosition;

		Eigen::Vector3f		differenceHigh;
		Eigen::Vector3f		differenceLow;
	};
}

