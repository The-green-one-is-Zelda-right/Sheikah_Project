#pragma once
#include "PurahEngineAPI.h"
#include "ZnDistanceJoint.h"
#include "JointT.h"

namespace PurahEngine
{
	class PURAHENGINE_API DistanceJoint : public JointT<ZonaiPhysics::ZnDistanceJoint>
	{
	public:
		DistanceJoint() = default;
		~DistanceJoint() override;

	public:
		void OnDataLoadComplete() override;

	public:
		float GetDistance() const;

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

		// 조인트 플래그
		void	SetMinDistanceEnable(bool _value) const;
		void	SetMaxDistanceEnable(bool _value) const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		Eigen::Vector3f LocalAnchor;

		Eigen::Vector3f connectedLocalAnchor;

		bool useMinDistance = false;
		float minDistance = 0.f;
		float maxDistance = 0.f;
		float Tolerance = 0.025f;
	};
}

