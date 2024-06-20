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
		// �������� ����
		virtual void				DisableQuery(bool) = 0;

		// �浹 ����
		// (������ �ٵ� �޾Ƴ��� �ùķ��̼��� ���� )
		virtual void				DisableSimulation(bool) = 0;

		// Ʈ���� ����
		virtual void				SetTrigger(bool) = 0;

		// ���̾� ����
		virtual void				SetLayerData(const uint32_t&) = 0;

		// �ݶ��̴� ��ġ ������
		virtual Eigen::Vector3f		GetLocalPosition() const = 0;
		virtual void				SetLocalPosition(const Eigen::Vector3f&) = 0;

		// �ݶ��̴� ȸ�� ������
		virtual Eigen::Quaternionf	GetLocalQuaternion() const = 0;
		virtual void				SetLocalQuaternion(const Eigen::Quaternionf&) = 0;

		virtual ZnBound3			GetBoundingBox(const Eigen::Vector3f&, const Eigen::Quaternionf&) = 0;
	};
}
