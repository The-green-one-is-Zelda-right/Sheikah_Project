#pragma once
#include "PurahEngine.h"

// #define PROPERTY(type, value) type #value; \ 

namespace PurahEngine
{
	class ITween;



	class Controller : public Component
	{
	public:
		enum class State
		{
			IDLE,
			RUNNING,
		};

		~Controller() override;

		void Awake() override;
		void Start() override;

		void Update() override;

		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const Collider*) override;

	private:
		void GamePadInput();
		void Move();
		void RotateCamera();

		void HandsUp();

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		// void SetPlayer(GameObject*);

	private:
		// __declspec(property(get = , put = ));
		bool detect;
		float moveSpeed;
		float sensitivity;

		ITween* tween;

	private:
		RigidBody* playerRigidbody;
		Transform* modelCore;
		Transform* cameraArm;
		Transform* cameraCore;
		Animator* animator;

	private:
		State state = State::IDLE;
		Eigen::Vector3f movement = Eigen::Vector3f::Zero();

	private:
		GamePad* gamePad;
		bool onVibration = false;
		float LstickX;
		float LstickY;
		float LstickSize;
		float RstickSize;
		float RstickX;
		float RstickY;
		float LTrigger;
		float RTrigger;
		float vibrationL = 0.f;
		float vibrationR = 0.f;

	private:
		/// ����� ������
		Eigen::Vector3f startPosition;
		Eigen::Vector3f startLinearVelocity;
		Eigen::Vector3f startAngularVelocity;
		Eigen::Quaternionf startRotation;
	};
}

