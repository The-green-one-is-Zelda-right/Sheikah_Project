#pragma once
#include <string>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo
	{
		void* bodyData;					// ������Ʈ ������
		void* colliderData;				// �ݶ��̴� ������
		Eigen::Vector3f position;	// �浹 ��ġ
		float distance;				// �浹 �Ÿ�
	};
}
