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

	class Player final : public PurahEngine::Component
	{
	public:
		~Player() override;
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
		};

		enum AbilityState
		{
			ATTACH_HOLD		= -1,	// ������ �� ����

			DEFAULT			= 0,	// �⺻ ����
			ATTACH_SELECT	= 1,	// ������ �����Ϸ��� ����
			REWIND_SELECT	= 2,	// ������ �ǵ������� ����
			LOCK_SELECT		= 3
		};

	private:
#pragma region Struct
		struct PlayerData
		{
			bool debugMode = false;
			bool stopUpdate = false;

#pragma region Player Variable
			float moveSpeed = 10.f;				// �⺻ �ӵ�
			float holdSpeed = 5.f;				// ����ġ�� ���� ��� ���� �� �����̴� �ӵ�
			float sensitivity = 90.f;			// ī�޶� ȸ�� �ӵ�
			float jumpPower = 10.f;				// ���� ��
			bool jumping = false;
			float slopeLimit = 36.f;			// ��� ����
#pragma endregion Player Variable

			float cameraLerpTime = 0.5f;			// ���� �ð�
			float cameraLerpTime0 = 1.0f;			// ��� ����

#pragma region Player Component
			PurahEngine::RigidBody* playerRigidbody;
			PurahEngine::Transform* modelCore;
			PurahEngine::Transform* cameraArm;
			PurahEngine::Transform* cameraCore;
			PurahEngine::Animator* animator;
			PurahEngine::GameObject* crossHead;
			PurahEngine::Transform* groundChechRaycast;
#pragma endregion Player Component

#pragma region Camera
			PurahEngine::Transform* attachLowCamera0;
			PurahEngine::Transform* attachLowCamera1;
			PurahEngine::Transform* attachDefaultCamera0;
			PurahEngine::Transform* attachDefaultCamera1;
			PurahEngine::Transform* attachHighCamera0;
			PurahEngine::Transform* attachHighCamera1;
#pragma endregion Camera

			Eigen::Vector3f		coreDefaultPosition;
			Eigen::Quaternionf	coreDefaultRotation;

			Eigen::Vector3f		coreSelectPosition;

			Eigen::Vector3f		armDefaultPosition;
			Eigen::Quaternionf	armDefaultRotation;

			Eigen::Vector3f coreCurrentPosition;
			Eigen::Quaternionf	coreCurrentRotation;
			Eigen::Vector3f coreTargetPosition;
			Eigen::Quaternionf	coreTargetRotation;
			Eigen::Vector3f coreTargetWorldPosition;
			Eigen::Quaternionf	coreTargetWorldRotation;

			Eigen::Vector3f armTargetPosition;
			Eigen::Quaternionf	armTargetRotation;

			float xAngle = 0.f;					// ���� �ޱ�
			const float limitHighAngle = 80.f;	// ���� �ޱ�
			const float limitLowAngle = -70.f;	// �ο� �ޱ�

			unsigned int cameraCollisionLayers = 0;
			float cameraCollisionRadius = 5.f;
			unsigned int attachRaycastLayers = 0;
			float attachRaycastDistance = 40.f;

			AbilityState state = ATTACH_SELECT;

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

			PzObject* holdObject;
			PurahEngine::RigidBody* holdObjectBody;
		};

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
#pragma endregion Struct

	public:
		void InitializeGamePad();
		void InitializeDefaultPositions();
		void InitializeAbilitySystem();
		void InitializeStateSystem();
		void InitializeLerpFunctions();

#pragma region Event
		void Start() override;
		void Update() override;
		void LateUpdate() override;
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
#pragma endregion Event

		void SetStopUpdate(bool _value);

	private:
		void DebugDraw();
		void DrawStateInfo() const;
		std::wstring GetStateString(AbilityState state) const;
		void DrawJumpInfo() const;

	private:
		void UpdateAbilityState();
		void UpdatePlayerState();
		void ChangeAbilityState(AbilityState);
		void ChangePlayerState(PlayerState);

		void HandleGamePadInput();
		void HandleStickInput();
		void HandleTriggerInput();
		void HandleButtonInput();
		void HandleButton(PurahEngine::ePad button, void (IState::* clickFunc)(), void (IState::* pressingFunc)(), void (IState::* upFunc)());

		bool TryJump();
		void JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider);
		bool IsOppositeDirection(const Eigen::Vector3f& velo, const Eigen::Vector3f& normal) const;
		bool IsGrounded(const Eigen::Vector3f& normal) const;

		bool TryPlayerMove(float _moveSpeed);
		void LookInWorldDirection(const Eigen::Vector3f& _worldDirection) const;
		void LookInLocalDirection(const Eigen::Vector3f& _localDirection) const;

#pragma region camera
		void UpdateDefaultCamera();					// ī�޶� ������Ʈ
		void UpdateHoldCamera();					// ī�޶� ������Ʈ

		void UpdateDefaultCameraCore();							// ī�޶� �ھ� ������Ʈ
		void UpdateHoldCameraCore();							// ī�޶� �ھ� ������Ʈ

		/// <summary>
		/// Camera Core ��ġ ���
		/// 
		/// ī�޶� ��ġ�� Arm�� ������ ���� ����
		/// </summary>
		/// <returns>ī�޶� �ھ� ���� ��ǥ</returns>
		void CalculateDefaultCameraCorePosition(Eigen::Vector3f& localOut, Eigen::Vector3f& worldOut);	// ī�޶� ��ġ ������Ʈ

		/// <summary>
		/// ī�޶� ���� ������ �浹 �Ǵ��� üũ�ϰ� ��ġ�� ������
		/// </summary>
		/// <param name="pos">ī�޶� �ھ� ���� ��ǥ</param>
		/// <returns>���� ������ �ε�ġ�� true</returns>
		bool ResolveCameraCollision(Eigen::Vector3f& localIn, Eigen::Vector3f& worldIn);

		void UpdateCameraPositionLerp();
		void UpdateCameraRotationLerp();
		void UpdateCameraLerp();
		
		void CharacterDisable();

		/// <summary>
		/// ī�޶� �ھ��� ��ǥ ���� ��ǥ�� ������
		/// 
		/// ���� ��ǥ�� �����ϰ� ��ǥ ��ǥ�� ������
		/// </summary>
		/// <param name="_worldPosision">�ھ��� ���� ��ǥ</param>
		void SetCameraCoreLocalTargetPosition(const Eigen::Vector3f& _localPosision);
		void SetCameraCoreLocalTargetRotation(const Eigen::Quaternionf& _localRotation);
		
		/// <summary>
		/// ī�޶� �ھ��� ��ǥ ���� ��ǥ�� ������
		/// 
		/// ���� ��ǥ�� �����ϰ� ��ǥ ��ǥ�� ������
		/// </summary>
		/// <param name="_worldPosision">�ھ��� ���� ��ǥ</param>
		void SetCameraCoreWorldTargetPosition(const Eigen::Vector3f& _worldPosision);
		void SetCameraCoreWorldTargetRotation(const Eigen::Quaternionf& _worldRotation);

		/// <summary>
		/// ī�޶� �ھ XY, XZ����� ��� ���� �ִ��� ���
		/// </summary>
		/// <param name="cameraPos">ī�޶� �ھ��� ���� ��ǥ</param>
		/// <param name="direction">ī�޶� �ھ��� ���� ���� ����</param>
		/// <returns>ī�޶� �ھ��� ���� XY, XZ ����� �� ��</returns>
		Eigen::Vector3f CalculateCameraFocusPosition(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction);
		
		/// <summary>
		/// ī�޶� �ھ ���� �ִ� ���� XY ����� ��ǥ�� ����ϴ� �Լ�
		/// </summary>
		/// <param name="cameraPos">ī�޶� �ھ� ���� ��ǥ</param>
		/// <param name="direction">ī�޶� �ھ� ���� ���� ����</param>
		/// <param name="out">���� ���� XY ����� ��ǥ</param>
		/// <returns>XY ���� �����ϸ� false</returns>
		bool IntersectXYPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out);
		
		/// <summary>
		/// ī�޶� �ھ ���� �ִ� ���� XZ ����� ��ǥ�� ����ϴ� �Լ�
		/// </summary>
		/// <param name="cameraPos">ī�޶� �ھ� ���� ��ǥ</param>
		/// <param name="direction">ī�޶� �ھ� ���� ���� ����</param>
		/// <param name="out">���� ���� XZ ����� ��ǥ</param>
		/// <returns>���� �����ϸ� false</returns>
		bool IntersectXZPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out);

		void ResetCamera();							// ī�޶� ��ġ �ʱ�ȭ
		void ResetCameraArm();						// ī�޶� �� ��ġ �ʱ�ȭ
		void ResetCameraCore();						// ī�޶� �ھ� ��ġ �ʱ�ȭ
		
		void RotateCameraArm();							// ī�޶� �� ȸ��
		void RotateCameraArmYaw(float yawAngle);		// ī�޶� �� yaw ȸ��
		void RotateCameraArmPitch(float pitchAngle);	// ī�޶� �� pitch ȸ��

		void CameraLookTo(const Eigen::Vector3f& _direction);
		void CameraLookAt(const Eigen::Vector3f& _position);

#pragma endregion camera

		bool RaycastFromCamera(
			float _distance, 
			PurahEngine::RigidBody** _outBody, 
			PzObject** _outAttachable, 
			Rewindable** _outRewindable
		);

	public:
#pragma region ����ȭ
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
#pragma endregion ����ȭ

	private:
		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		std::unordered_map<AbilityState, IState*> stateSystem;
		std::set<AbilityState> stateChange;
		std::unordered_map<PlayerState, std::function<void()>> animationState;
		std::unordered_map<PlayerState, std::function<void()>> animationSpeedController;
		
		AbilityState prevState = DEFAULT;
		AbilityState currState = DEFAULT;

		PlayerState prevPlayerState = IDLE;
		PlayerState currPlayerState = IDLE;

		PurahEngine::GamePad* gamePad;
		PlayerInput currInput;
		PlayerInput prevInput;

		PlayerData data;
		std::function<Eigen::Vector3f(const Eigen::Vector3f, const Eigen::Vector3f, float)> lerp;
		std::function<Eigen::Quaternionf(const Eigen::Quaternionf, const Eigen::Quaternionf, float)> slerp;

	private:
		int stopCount;

	private:
		/// ����� ������
		Eigen::Vector3f		highPosition;
		Eigen::Vector3f		lowPosition;

		Eigen::Vector3f		differenceHigh;
		Eigen::Vector3f		differenceLow;
	};

	inline Eigen::Vector3f MulMatrixVector(const Eigen::Matrix4f& _mat, const Eigen::Vector3f& _vec)
	{
		Eigen::Vector4f temp = _mat * Eigen::Vector4f(_vec.x(), _vec.y(), _vec.z(), 1.f);

		return { temp.x(), temp.y() , temp.z() };
	}
}

