#pragma once
#include "ZnCharaterController.h"

namespace ZonaiPhysics
{
	class ZnBoxController : public ZnCharaterController
	{
	public:
		virtual ~ZnBoxController() = 0;

	public:
		/// <summary>
		/// Setter
		/// </summary>
		virtual void SetPosition(const Eigen::Vector3f& _position) = 0;
		virtual void SetFootPosition(const Eigen::Vector3f& _position) = 0;
		virtual void SetStepOffset(const float _offset) = 0;
		virtual void SetContactOffset(const float _offset) = 0;
		virtual void SetUpDirection(const Eigen::Vector3f& _direction) = 0;
		virtual void SetSlope(const float _slope) = 0;
		virtual void SetHalfExtend(const Eigen::Vector3f& _extend) = 0;

		/// <summary>
		/// Getter
		/// </summary>
		virtual Eigen::Vector3f GetPosition() = 0;
		virtual Eigen::Vector3f GetFootPosition() = 0;
		virtual float GetStepOffset() = 0;
		virtual float GetContactOffset() = 0;
		virtual Eigen::Vector3f GetUpDirection() = 0;
		virtual float GetSlope() = 0;
		virtual Eigen::Vector3f GetHalfExtend() = 0;

		/// <summary>
		/// Function
		/// </summary>
		virtual void Move(Eigen::Vector3f _vec, float _dis, float _dt) = 0;
		virtual void Resize(float _height) = 0;
	};
}