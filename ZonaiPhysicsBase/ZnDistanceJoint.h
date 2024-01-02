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
		// ���� ����Ʈ �Ÿ�
		virtual float	GetDistance() const = 0;

		// ����Ʈ �ּ� �Ÿ�
		virtual void	SetMinDistance(float _distance) = 0;
		virtual float	GetMinDistance() const = 0;

		// ����Ʈ �ִ� �Ÿ�
		virtual void	SetMaxDistance(float _distance) = 0;
		virtual float	GetMaxDistance() const = 0;

		// ��� ������ ��� �Ÿ�
		// ����Ʈ�� Ȱ��ȭ �Ǵ� ����
		virtual void	SetTolerance(float _tolerance) = 0;
		virtual float	GetTolerance() const = 0;
		
		// ������ ����
		// �⺻�� 0.f
		virtual void	SetStiffness(float _stiffness) = 0;
		virtual float	GetStiffness() const = 0;
		
		// ������ ����
		// �⺻�� 0.f
		virtual void	SetDanmping(float _damping) = 0;
		virtual float	GetDanmping() const = 0;

		// ����Ʈ �÷���
		virtual void	SetMinDistanceEnable(bool _value) = 0;
		virtual void	SetMaxDistanceEnable(bool _value) = 0;
		virtual void	SetSpringEnable(bool _value) = 0;
	};
}