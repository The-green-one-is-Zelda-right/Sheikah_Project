#pragma once
#include "ZnObject.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnCollider : public ZnObject
	{
	public:
							ZnCollider() noexcept = default;
		virtual				~ZnCollider() noexcept = default;

	public:
		virtual void		SetTrigger(bool) noexcept = 0;

		virtual Eigen::Vector3f GetLocalPosition() const noexcept = 0;
		virtual void SetLocalPosition(const Eigen::Vector3f&) noexcept = 0;

		virtual Eigen::Quaternionf GetLocalQuaternion() const noexcept = 0;
		virtual void SetLocalQuaternion(const Eigen::Quaternionf&) noexcept = 0;
	};
}
