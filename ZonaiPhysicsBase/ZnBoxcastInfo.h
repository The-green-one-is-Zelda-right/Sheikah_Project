#pragma once
#include <string>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo
	{
		void* bodyData;					// 오브젝트 포인터
		void* colliderData;				// 콜라이더 포인터
		Eigen::Vector3f position;	// 충돌 위치
		float distance;				// 충돌 거리
	};
}
