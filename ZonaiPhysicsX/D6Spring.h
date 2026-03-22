#pragma once
#include "ZnDistanceJoint.h"
#include "TemplateJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

namespace ZonaiPhysics
{
class D6Spring : public TemplateJoint<ZnJoint, physx::PxD6Joint>
{
public:
	D6Spring(physx::PxD6Joint* _pxJoint, RigidBody* _znBody0, RigidBody* _znBody1);
	~D6Spring();

	void SetLinearLimit(float limit);
	float GetLinearLimit() const;

	void SetSpringProperties(float stiffness, float damping);
	void GetSpringProperties(float& stiffness, float& damping) const;

private:
	const physx::PxTolerancesScale* tolerances;
};
}