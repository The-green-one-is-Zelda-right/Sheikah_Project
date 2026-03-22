#pragma once

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#include "PxPhysicsAPI.h"

#include "ZnBound3.h"
#pragma warning (pop)

namespace ZonaiPhysics
{
	class ZnBVH
	{
	public:
		ZnBVH();
		~ZnBVH();

		static void Clear();
		static void Initialize();
		static void AddObject(const ZonaiPhysics::ZnBound3);
		static void Culling();

	private:
		physx::PxBVH* bvh;
	};
}