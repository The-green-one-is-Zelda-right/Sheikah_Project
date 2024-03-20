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

		// ����Ʈ �ּ� �Ÿ�
		void	SetMinDistance(float _distance);
		float	GetMinDistance() const;

		// ����Ʈ �ִ� �Ÿ�
		void	SetMaxDistance(float _distance);
		float	GetMaxDistance() const;

		// ��� ������ ��� �Ÿ�
		// ����Ʈ�� Ȱ��ȭ �Ǵ� ����
		void	SetTolerance(float _tolerance);
		float	GetTolerance() const;

		// ����Ʈ �÷���
		void	SetMinDistanceEnable(bool _value);
		void	SetMaxDistanceEnable(bool _value);

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

