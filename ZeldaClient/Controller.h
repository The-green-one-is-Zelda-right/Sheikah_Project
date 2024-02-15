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
		float speed;
		float drag;

		RigidBody* rigidbody;
		Transform* transform;
	};
}

