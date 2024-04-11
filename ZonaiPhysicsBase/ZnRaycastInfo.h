#pragma once
#include <string>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo
	{
		void* bodyData = nullptr;					// ������Ʈ ������
		void* colliderData = nullptr;				// �ݶ��̴� ������
		Eigen::Vector3f position{ 0.f, 0.f, 0.f };	// �浹 ��ġ
		float distance = FLT_MIN;				// �浹 �Ÿ�
	};
}
