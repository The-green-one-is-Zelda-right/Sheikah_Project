#pragma once
#include "ZnJoint.h"

namespace ZonaiPhysics
{
	class ZnDistanceJoint : public ZnJoint
	{
	public:
		ZnDistanceJoint() noexcept = default;
		~ZnDistanceJoint() noexcept override = default;


	public:
		// 현재 조인트 거리
		virtual float	GetDistance() const = 0;

		// 조인트 최소 거리
		virtual void	SetMinDistance(float _distance) = 0;
		virtual float	GetMinDistance() const = 0;

		// 조인트 최대 거리
		virtual void	SetMaxDistance(float _distance) = 0;
		virtual float	GetMaxDistance() const = 0;

		// 허용 범위를 벗어난 거리
		// 조인트가 활성화 되는 지점
		virtual void	SetTolerance(float _tolerance) = 0;
		virtual float	GetTolerance() const = 0;
		
		// 스프링 강도
		// 기본값 0.f
		virtual void	SetStiffness(float _stiffness) = 0;
		virtual float	GetStiffness() const = 0;
		
		// 스프링 감쇠
		// 기본값 0.f
		virtual void	SetDamping(float _damping) = 0;
		virtual float	GetDamping() const = 0;

		// 조인트 플래그
		virtual void	SetMinDistanceEnable(bool _value) = 0;
		virtual void	SetMaxDistanceEnable(bool _value) = 0;
		virtual void	SetSpringEnable(bool _value) = 0;
	};
}