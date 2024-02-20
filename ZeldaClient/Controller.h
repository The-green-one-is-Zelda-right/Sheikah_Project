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
		void FixedUpdate() override;

	public:
		void SetPlayer(GameObject*);

	private:
		float speed;
		float drag;

		GameObject* player;
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

