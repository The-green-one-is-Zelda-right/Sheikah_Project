#pragma once
#include "PurahEngine.h"


namespace  Phyzzle
{
	class PlayerController final : public PurahEngine::Component
	{
	public:
		~PlayerController() override;

		void Start() override;

	public:
		void Move(const Eigen::Vector3f& _dir, float _speed);
		void LookTo(const Eigen::Vector3f& _worldDirection);
		void LookAt(const Eigen::Vector3f& _worldPosition);
		void Jump();

		void SetSlope(float _angle);

		bool CanJump();
		bool IsGround();
		bool IsWall();
		bool IsObstacle();

	public:
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionExit(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;

	protected:
		void PreSerialize(json& jsonData) const override {}
		void PreDeserialize(const json& jsonData) override {}
		void PostSerialize(json& jsonData) const override {}
		void PostDeserialize(const json& jsonData) override {}

	private:
		PurahEngine::RigidBody* body;
		PurahEngine::Transform* model;

	private:
		bool jumping = false;
		float slope;

		ZonaiPhysics::ZnQueryDesc groundCheck;
	};
}

