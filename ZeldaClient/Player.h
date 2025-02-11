#pragma once
#include <memory>
#include "PurahEngine.h"
#include "EnumPlayerState.h"

namespace Phyzzle
{
	class PlayerController;
	class PlayerStateMachine;
	class PlayerMovement;
	class PlayerAnimation;
	class PlayerCamera;
	class PlayerUIManager;
	class PzObject;
	class GroundCheck;

	class Player : public PurahEngine::Component
	{
	public:
		~Player() override;

		void Start() override;
		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void SetStopUpdate(bool _stop) { data.stopUpdate = _stop; };

		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;

		void PreSerialize(json& jsonData) const override {}
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override {}
		void PostDeserialize(const json& jsonData) override;

		// 서브 시스템 Getter
		PlayerController* GetController() { return controller.get(); }
		PlayerStateMachine* GetStateMachine() { return stateMachine.get(); }
		PlayerMovement* GetMovement() { return movement.get(); }
		PlayerAnimation* GetAnimation() { return animation.get(); }
		PlayerCamera* GetCamera() { return camera.get(); }
		PlayerUIManager* GetUIManager() { return uiManager.get(); }

		// 상태 변경
		void ChangeAbilityState(AbilityState newState);
		AbilityState GetCurrentAbilityState() const;

		void LookInWorldDirection(const Eigen::Vector3f& _worldDirection) const;
		void LookInLocalDirection(const Eigen::Vector3f& _localDirection) const;

		PurahEngine::RigidBody* GetRigidbody() { return data.playerRigidbody; }
		PurahEngine::Transform* GetModelTransform() { return data.modelCore; }
		PurahEngine::Transform* GetCameraCoreTrnasform() { return data.cameraCore; }
		PurahEngine::Transform* GetCameraArmTrnasform() { return data.cameraArm; }

	private:
		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		void InitializePlayerAnimation();
		void InitializePlayerCamera();
		void InitializePlayerUI();

		void DeserializePlayer(const json& jsonData);
		void DeserializePlayerAbility(const json& jsonData);
		void DeserializePlayerAnimation(const json& jsonData);
		void DeserializePlayerCamera(const json& jsonData);

		void PostDeserializePlayer(const json& jsonData);
		void PostDeserializePlayerCamera(const json& jsonData);
		void PostDeserializePlayerUI(const json& jsonData);

		void ApplyImpulse();
		void PlayerFlyingUpdate();

		std::unique_ptr<PlayerController> controller;
		std::unique_ptr<PlayerStateMachine> stateMachine;
		std::unique_ptr<PlayerMovement> movement;
		std::unique_ptr<PlayerAnimation> animation;
		std::unique_ptr<PlayerCamera> camera;
		std::unique_ptr<PlayerUIManager> uiManager;

		struct PlayerData
		{
			bool stateChange = true;

			bool debugMode = false;
			bool stopUpdate = false;

#pragma region Player Variable
			float impactThreshold = 50.f;
			Eigen::Vector3f playerFlyingVelocity;
			float maxLinearVelocityY = 30.f;

			float moveSpeed = 10.f;				// 기본 속도
			float holdSpeed = 5.f;				// 어태치로 물건 들고 있을 때 움직이는 속도
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

			Phyzzle::GroundCheck* groundCheck;
#pragma endregion Player Component

			Phyzzle::PzObject* holdObject;
			PurahEngine::RigidBody* holdObjectBody;
		} data;

		Eigen::Vector4f color3;
	};
}