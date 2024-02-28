#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class Controller : public Component
	{
	public:
		~Controller() override;

		void Awake() override;
		void Start() override;

		void Update() override;

	private:
		void Move();
		void HandsUp();


	public:
		void SetPlayer(GameObject*);

	private:
		bool detect;
		float speed;
		float drag;

		GameObject* playerBody;
		RigidBody* rigidbody;
		Transform* transform;

		/// 사라질 변수들
	private:
		Eigen::Vector3f startPosition;
		Eigen::Vector3f startLinearVelocity;
		Eigen::Vector3f startAngularVelocity;
		Eigen::Quaternionf startRotation;
	};
}

