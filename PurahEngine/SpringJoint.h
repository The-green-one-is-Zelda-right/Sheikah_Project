#pragma once
#include "PurahEngineAPI.h"
#include "JointT.h"
#include "ZnDistanceJoint.h"

namespace PurahEngine
{
	class PURAHENGINE_API SpringJoint : public JointT<ZonaiPhysics::ZnDistanceJoint>
	{
	public:
		SpringJoint() = default;
		~SpringJoint() override;

	public:
		void PostInitialize() override;

	public:
		// 현재 조인트 거리
		float	GetDistance() const;

		// 조인트 최소 거리
		void	SetMinDistance(float _distance) const;
		float	GetMinDistance() const;

		// 조인트 최대 거리
		void	SetMaxDistance(float _distance) const;
		float	GetMaxDistance() const;

		// 허용 범위를 벗어난 거리
		// 조인트가 활성화 되는 지점
		void	SetTolerance(float _tolerance) const;
		float	GetTolerance() const;

		// 스프링 강도
		// 기본값 0.f
		void	SetStiffness(float _stiffness) const;
		float	GetStiffness() const;

		// 스프링 감쇠
		// 기본값 0.f
		void	SetDamping(float _damping) const;
		float	GetDamping() const;

		// 스프링 관련 변수
		void	SetSpringArg(float _stiffness, float _damping) const;

		// 조인트 플래그
		void	SetMinDistanceEnable(bool _value) const;
		void	SetMaxDistanceEnable(bool _value) const;
		void	SetSpringEnable(bool _value) const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		bool useSpring = false;
		float spring = 10.0f;
		float damper = 0.2f;

		bool useMinDistance = true;
		float minDistance = 0.f;
		bool useMaxDistance = true;
		float maxDistance = 0.f;
		float Tolerance = 0.025f;
	};
}

