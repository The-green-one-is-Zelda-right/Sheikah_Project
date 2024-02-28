#include <extensions/PxJoint.h>

#include "JointHelper.h"

namespace ZonaiPhysics
{
	void JointHelper::Release(void* _pxJoint)
	{
		assert(_pxJoint != nullptr);

		static_cast<physx::PxJoint*>(_pxJoint)->release();
	}
}
