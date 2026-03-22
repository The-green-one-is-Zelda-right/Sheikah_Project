#pragma once
#include <Eigen/Dense>

namespace physx
{
	class PxController;
}

namespace ZonaiPhysics
{
	// template <typename _Base, typename _Ty>
	class CharacterController
	{
	public:
		CharacterController(physx::PxController* _controller);

	public:
		/// <summary>
		/// Setter
		/// </summary>
		void SetCollisonData();
		void SetPosition(const Eigen::Vector3f& _position);
		void SetFootPosition(const Eigen::Vector3f& _position);
		void SetStepOffset(const float _offset);
		void SetContactOffset(const float _offset);
		void SetUpDirection(const Eigen::Vector3f& _direction);
		void SetSlopeLimit(const float _slope);

		/// <summary>
		/// Getter
		/// </summary>
		Eigen::Vector3f GetPosition();
		Eigen::Vector3f GetFootPosition();
		float GetStepOffset();
		float GetContactOffset();
		Eigen::Vector3f GetUpDirection();
		float GetSlopeLimit();

		/// <summary>
		/// Function
		/// </summary>
		void Move(Eigen::Vector3f _vec);
		void Jump(float _velocity);
		void ApplyImpulse(const Eigen::Vector3f& _impulse);
		void Apply(float _dt);
		bool IsInAir() const;

	public:
		physx::PxController* controller;
		bool isJumping;
		float jumpVelocity;
		Eigen::Vector3f pendingMovement;
		physx::PxControllerCollisionFlags collisionFlags;
	};
}
