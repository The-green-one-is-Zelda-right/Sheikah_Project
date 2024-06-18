#pragma once
#include <set>

#include "PurahEngine.h"

namespace Phyzzle
{
	class Rewindable;
}

namespace Phyzzle
{
	class PzObject;
}

namespace ZonaiPhysics
{
	struct ZnQueryInfo;
	class Collider;
}

namespace Phyzzle
{
	class Holder;
	class IState;

	class Player final : public PurahEngine::Component
	{
	public:
		~Player() override;
		enum AnimationState
		{
			IDLE,
			WALK,
			ABILITY,
		};

		enum State
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
			float moveSpeed = 10.f;				// �⺻ �ӵ�
			float holdSpeed = 5.f;				// ����ġ�� ���� ��� ���� �� �����̴� �ӵ�

			float sensitivity = 90.f;			// ī�޶� ȸ�� �ӵ�
			float jumpPower = 10.f;				// ���� ��
			bool jumping = false;

			PurahEngine::RigidBody* playerRigidbody;
			PurahEngine::Transform* modelCore;
			PurahEngine::Transform* cameraArm;
			PurahEngine::Transform* cameraCore;
			PurahEngine::Animator* animator;
			PurahEngine::GameObject* crossHead;

			Eigen::Vector3f		coreDefaultPosition;
			Eigen::Quaternionf	coreDefaultRotation;

			Eigen::Vector3f		armDefaultPosition;
			Eigen::Quaternionf	armDefaultRotation;

			float xAngle = 0.f;					// ���� �ޱ�
			const float limitHighAngle = 80.f;	// ���� �ޱ�
			const float limitLowAngle = -70.f;	// �ο� �ޱ�

			unsigned int cameraCollisionLayers = 0;
			unsigned int attachRaycastLayers = 0;
			bool cameraUpdate = true;			// 
			float lerpFactor = 0.2f;			// 
			float acclerpFactor = 0.f;			//

			State state = ATTACH_SELECT;

			std::wstring idleAnimation;
			std::wstring runningAnimation;
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
#pragma region Event
		void Start() override;
		void Update() override;
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
#pragma endregion Event

		void DebugDraw();

	private:
		void GamePadInput();

		void Jump();
		void JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider);

		void PlayerMove(float _moveSpeed);
		void LookInWorldDirection(const Eigen::Vector3f& _worldDirection) const;
		void LookInLocalDirection(const Eigen::Vector3f& _localDirection) const;

		void CameraCoreUpdate();
		void CameraArmReset();
		void CameraCoreReset();
		void CameraReset();
		void CameraAround();
		void CameraLookTo(const Eigen::Vector3f& _direction);
		void CameraLookAt(const Eigen::Vector3f& _position);

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
		void ChangeState(State);

	private:
		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		std::unordered_map<State, IState*> stateSystem;
		std::set<State> stateChange;
		
		State prevState = DEFAULT;
		State currState = DEFAULT;

		PurahEngine::GamePad* gamePad;
		PlayerInput currInput;
		PlayerInput prevInput;

		PlayerData data;
		std::function<Eigen::Vector3f(const Eigen::Vector3f, const Eigen::Vector3f, float)> lerp;
		std::function<Eigen::Quaternionf(const Eigen::Quaternionf, const Eigen::Quaternionf, float)> slerp;

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

