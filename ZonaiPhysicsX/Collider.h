#pragma once
#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#pragma warning (pop)

#include "ZnResourceID.h"
#include "ZnCollider.h"

namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxRigidDynamic;
	class PxRigidActor;
}

namespace ZonaiPhysics
{
	struct ZnTransform;
	struct ZnBound3;
	class RigidBody;

	class Collider : public ZnCollider
	{
	public:
									Collider() = delete;
									Collider(physx::PxShape*, RigidBody*);
									//Collider(physx::PxPhysics*&, RigidBody*);
									~Collider() override;

	public:
		void						Disable(bool) final;
		void						DisableQuery(bool) final;
		void						DisableSimulation(bool) final;
		void						SetTrigger(bool) final;

		void						SetLayerData(const uint32_t&) final;

		/**
		위치
		*/
		Eigen::Vector3f				GetPosition() const final;
		void						SetPosition(const Eigen::Vector3f& _position, bool _wakeup) final;
		Eigen::Vector3f				GetLocalPosition() const final;
		void						SetLocalPosition(const Eigen::Vector3f& _position) final;

		/**
		회전
		*/
		Eigen::Quaternionf			GetQuaternion() const final;
		void						SetQuaternion(const Eigen::Quaternionf& _quaternion, bool _wakeup) final;
		Eigen::Quaternionf			GetLocalQuaternion() const override;
		void						SetLocalQuaternion(const Eigen::Quaternionf& _quaternion) override;

		/**
		유저 데이터
		*/
		void*						GetUserData() const final;
		void						SetUserData(void* _userData) final;

		ZnBound3					GetBoundingBox(const Eigen::Vector3f&, const Eigen::Quaternionf&) final;

	public:
		void						SetMaterial(const ZnMaterialID& _id);
		void						SetMaterials(ZnMaterialID* _pointer, int size);
		void						UpdateInertiaTensor() const;

	protected:
		friend class ZnPhysicsX;
		RigidBody* znBody;

	public:
		physx::PxShape* pxShape;
	};
} // namespace ZonaiPhysics

