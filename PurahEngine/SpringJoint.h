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
		void OnDataLoadComplete() override;

	public:
		// ���� ����Ʈ �Ÿ�
		float	GetDistance() const;

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

		// ������ ����
		// �⺻�� 0.f
		void	SetStiffness(float _stiffness) const;
		float	GetStiffness() const;

		// ������ ����
		// �⺻�� 0.f
		void	SetDamping(float _damping) const;
		float	GetDamping() const;

		// ������ ���� ����
		void	SetSpringArg(float _stiffness, float _damping) const;

		// ����Ʈ �÷���
		void	SetMinDistanceEnable(bool _value) const;
		void	SetMaxDistanceEnable(bool _value) const;
		void	SetSpringEnable(bool _value) const;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		Eigen::Vector3f LocalAnchor;
		Eigen::Quaternionf LocalAnchorRotation;
		Eigen::Vector3f connectedLocalAnchor;
		Eigen::Quaternionf connectedLocalAnchorRotation;

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

