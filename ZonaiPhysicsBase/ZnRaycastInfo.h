#pragma once
#include <string>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo
	{
		std::wstring name;			// 충돌한 오브젝트의 이름
		void* data;					// 오브젝트의 포인터
		Eigen::Vector3f position;	// 충돌한 위치
	};
}
