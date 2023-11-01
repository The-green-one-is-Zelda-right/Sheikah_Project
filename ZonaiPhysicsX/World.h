#pragma once
#include "ZonaiPhysicsBase.h"

namespace ZonaiPhysics
{
	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;
	class ZnTransform;


	class World : public ZonaiPhysicsBase
	{
	public:
		World() noexcept = default;
		virtual ~World() noexcept;

	public:
		virtual void		Initialize() noexcept;
		virtual void		Finalize() noexcept;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody* CreateRigidBody() noexcept;
		
		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*	CreatBoxCollider() noexcept;
		virtual ZnCollider*	CreatPlaneCollider() noexcept;
		virtual ZnCollider*	CreatSphereCollider() noexcept;
		virtual ZnCollider*	CreateCapsuleCollider() noexcept;
		virtual ZnCollider*	CreateCustomCollider() noexcept;
		
		/// <summary>
		/// Create Joint
		/// </summary>
		virtual ZnJoint*		CreatD6Joint() noexcept;			// D6 ����Ʈ		*��� ���� ��
		virtual ZnJoint*		CreatFixedJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;			// ���� ����Ʈ
		virtual ZnJoint*		CreatDistanceJoint() noexcept;		// �Ÿ� ����Ʈ
		virtual ZnJoint*		CreatSphericalJoint() noexcept;		// ���� ����Ʈ
		virtual ZnJoint*		CreatRevoluteJoint() noexcept;		// ȸ�� ����Ʈ
		virtual ZnJoint*		CreatPrismaticJoint() noexcept;		// ������ ����Ʈ
	};
}

