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
		// ���� ����Ʈ �Ÿ�
		float GetDistance() const;

		// ����Ʈ �ּ� �Ÿ�
		void	SetMinDistance(float _distance) const;
		float	GetMinDistance() const;

		// ����Ʈ �ִ� �Ÿ�
		void	SetMaxDistance(float _distance) const;
		float	GetMaxDistance() const;

		// ��� ������ ��� �Ÿ�
		// ����Ʈ�� Ȱ��ȭ �Ǵ� ����
		void	SetTolerance(float _tolerance) const;
		float	GetTolerance() const;

		// ����Ʈ �÷���
		void	SetMinDistanceEnable(bool _value) const;
		void	SetMaxDistanceEnable(bool _value) const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		bool useMinDistance = false;
		float minDistance = 0.f;
		float maxDistance = 0.f;
		float Tolerance = 0.025f;
	};
}

