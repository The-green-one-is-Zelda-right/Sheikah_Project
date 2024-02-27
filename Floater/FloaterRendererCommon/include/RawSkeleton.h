#pragma once
#include "Transform.h"
#include <vector>

namespace flt
{
	struct RawSkeleton
	{
		std::vector<Transform> bones;
		int rootBoneIndex;
	};
}
