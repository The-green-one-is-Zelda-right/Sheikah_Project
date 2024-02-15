#pragma once

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#pragma warning (pop)

#include "ForceType.h"

namespace ZonaiPhysics
{
	class RigidBodyHelper
	{
	public:
		static void				Detach(void* _body, void* _shape);
		static void				Attach(void* _body, void* _shape);

	public:
		static void				Release(void*);

		static void				WakeUp(void*);
		static bool				IsSleeping(void*);

		static void				UseGravity(void*, bool);

		static void				SetKinematic(void*, bool);

		static void				UpdateInertiaTensor(void*);

		static void				Disable(void*, bool);

		static uint8_t			GetDynamicLockFlags(void*);
		static void				SetDynamicLockFlag(void*, uint8_t flag, bool);
		static void				SetDynamicLockFlags(void*, uint8_t flags);

		static void*			GetUserData(void*);
		static void				SetUserData(void*, void*);

	public:
		// Áú·®
		static float			GetMass(void*);
		static void				SetMass(void*,float);
		static float			GetInvMass(void*);

		static Eigen::Vector3f	GetInertiaTensor(void*);
		static void				SetInertiaTensor(void*, const Eigen::Vector3f&);

		static float			GetLinearDamping(void*);
		static void				SetLinearDamping(void*, float);

		static float			GetAngularDamping(void*);
		static void				SetAngularDamping(void*, float);

	public:
		// ¼Óµµ
		static Eigen::Vector3f	GetLinearVelocity(void*);
		static void				SetLinearVelocity(void*, const Eigen::Vector3f&);

		static Eigen::Vector3f	GetAngularVelocity(void*);
		static void				SetAngularVelocity(void*, const Eigen::Vector3f&);

		static float			GetMaxLinearVelocity(void*);
		static void				SetMaxLinearVelocity(void*, const float&);

		static float			GetMaxAngularVelocity(void*);
		static void				SetMaxAngularVelocity(void*, const float&);

		static Eigen::Vector3f	GetPosition(void*);
		static void				SetPosition(void*, const Eigen::Vector3f&);

		static Eigen::Quaternionf	GetQuaternion(void*);
		static void				SetQuaternion(void*, const Eigen::Quaternionf&);

		// Èû
		static void				SetForceAndTorque(void*, const Eigen::Vector3f& _force, const Eigen::Vector3f& _torque, ForceType _type);

		static void				AddForce(void*, const Eigen::Vector3f&, ForceType);
		static void				ClearForce(void*);

		static void				AddTorque(void*, const Eigen::Vector3f&, ForceType);
		static void				ClearTorque(void*);
	};
}

