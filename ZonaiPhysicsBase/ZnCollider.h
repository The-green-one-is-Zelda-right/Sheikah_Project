#pragma once
#include "ZnObject.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnCollider : public ZnObject
	{
	public:
							ZnCollider() = default;
		virtual				~ZnCollider() = default;

	public:
		// 트리거 설정
		virtual void		SetTrigger(bool) = 0;

		// 레이어 설정
		virtual void		SetLayerData(const uint32_t&) = 0;

		// 콜라이더 위치 오프셋
		virtual Eigen::Vector3f GetLocalPosition() const = 0;
		virtual void SetLocalPosition(const Eigen::Vector3f&) = 0;

		// 콜라이더 회전 오프셋
		virtual Eigen::Quaternionf GetLocalQuaternion() const = 0;
		virtual void SetLocalQuaternion(const Eigen::Quaternionf&) = 0;
	};
}
