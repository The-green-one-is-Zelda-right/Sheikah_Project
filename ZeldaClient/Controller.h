#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class ITween;

	class Controller : public Component
	{
	public:
		~Controller() override;

		void Awake() override;
		void Start() override;

		void Update() override;

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
		void SetPlayer(GameObject*);

	private:
		bool detect;
		float moveSpeed;
		float sensitivity;

		ITween* tween;

	private:
		GameObject* playerGameObject;
		RigidBody* playerRigidbody;
		Transform* myTransform;

	private:
		GamePad* gamePad;
		bool onVibration = false;
		float LstickX;
		float LstickY;
		float RstickX;
		float RstickY;
		float LTrigger;
		float RTrigger;
		float vibrationL = 0.f;
		float vibrationR = 0.f;

	private:
		/// 사라질 변수들
		Eigen::Vector3f startPosition;
		Eigen::Vector3f startLinearVelocity;
		Eigen::Vector3f startAngularVelocity;
		Eigen::Quaternionf startRotation;
	};
}

