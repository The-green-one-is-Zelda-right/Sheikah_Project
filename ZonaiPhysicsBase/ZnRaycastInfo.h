#pragma once
#include <string>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo
	{
		std::wstring name;			// �浹�� ������Ʈ�� �̸�
		void* data;					// ������Ʈ�� ������
		Eigen::Vector3f position;	// �浹�� ��ġ
	};
}
