#pragma once
#include <Eigen/Dense>
#include <string>

namespace ZonaiPhysics
{
	struct ZnQueryInfo
	{
		void* bodyData;					// ������Ʈ ������
		void* colliderData;				// �ݶ��̴� ������
		Eigen::Vector3f position;	// �浹 ��ġ
		float distance;				// �浹 �Ÿ�
	};
}
