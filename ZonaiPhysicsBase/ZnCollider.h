#pragma once
#include "ZnObject.h"
#include "ZnBound3.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnBound3;
	struct ZnTransform;

	class ZnCollider : public ZnObject
	{
	public:
									ZnCollider() = default;
		virtual						~ZnCollider() = default;

	public:
		virtual void				Disable(bool) = 0;

		// 쿼리에서 제외
		virtual void				DisableQuery(bool) = 0;

		// 충돌 설정
		// (리지드 바디를 달아놓고 시뮬레이션을 끄면 )
		virtual void				DisableSimulation(bool) = 0;

		// 트리거 설정
		virtual void				SetTrigger(bool) = 0;

		// 레이어 설정
		virtual void				SetLayerData(const uint32_t&) = 0;

		// 콜라이더 위치 오프셋
		virtual Eigen::Vector3f		GetLocalPosition() const = 0;
		virtual void				SetLocalPosition(const Eigen::Vector3f&) = 0;

		// 콜라이더 회전 오프셋
		virtual Eigen::Quaternionf	GetLocalQuaternion() const = 0;
		virtual void				SetLocalQuaternion(const Eigen::Quaternionf&) = 0;

		virtual ZnBound3			GetBoundingBox(const Eigen::Vector3f&, const Eigen::Quaternionf&) = 0;
	};
}
