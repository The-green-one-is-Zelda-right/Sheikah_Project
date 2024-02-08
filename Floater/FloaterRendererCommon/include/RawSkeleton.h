#pragma once
#include <vector>

namespace flt
{
	class Transform;

	struct RawSkeleton
	{
		std::vector<Transform*> bones;
		
	};
}
