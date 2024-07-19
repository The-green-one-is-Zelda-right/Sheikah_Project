#pragma once
#include <string>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnQueryInfo
	{
		~ZnQueryInfo()
		{
			// actors.clear();
			// shapes.clear();
		}

		void* bodyData = nullptr;							// ������Ʈ ������
		void* colliderData = nullptr;						// �ݶ��̴� ������
		Eigen::Vector3f position = Eigen::Vector3f::Zero();	// �浹 ��ġ
		float distance = FLT_MIN;							// �浹 �Ÿ�
		Eigen::Vector3f normal = Eigen::Vector3f::Zero();

		std::vector<void*> actors;
		std::vector<void*> shapes;
		// void* actors;
		// int actorsSize;
		// void* shapes;
		// int shapesSize;
	};
}
