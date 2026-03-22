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

		void* bodyData = nullptr;							// 오브젝트 포인터
		void* colliderData = nullptr;						// 콜라이더 포인터
		Eigen::Vector3f position = Eigen::Vector3f::Zero();	// 충돌 위치
		float distance = FLT_MIN;							// 충돌 거리
		Eigen::Vector3f normal = Eigen::Vector3f::Zero();

		std::vector<void*> actors;
		std::vector<void*> shapes;
		// void* actors;
		// int actorsSize;
		// void* shapes;
		// int shapesSize;
	};
}
