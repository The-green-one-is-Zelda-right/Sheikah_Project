#pragma once
#include <string>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnQueryInfo
	{
		void* bodyData = nullptr;							// ������Ʈ ������
		void* colliderData = nullptr;						// �ݶ��̴� ������
		Eigen::Vector3f position = Eigen::Vector3f::Zero();	// �浹 ��ġ
		float distance = FLT_MIN;							// �浹 �Ÿ�
		Eigen::Vector3f normal = Eigen::Vector3f::Zero();
	};
}
