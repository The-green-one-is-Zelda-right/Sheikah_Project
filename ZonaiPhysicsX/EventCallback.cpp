#include <cassert>

#include <PxShape.h>

#include "ZnJoint.h"
#include "ZnCollider.h"
#include "ZnSimulationCallback.h"

#include "ZnCollision.h"
#include "ZnUtil.h"

#include "EventCallback.h"

namespace ZonaiPhysics
{
	ZnSimulationCallback* EventCallback::callback = nullptr;

	void EventCallback::setInstance(ZnSimulationCallback* _instance)
	{
		assert(_instance);

		callback = _instance;
	}

	void EventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		// �ı��� ������ ������ �Ѱ���
		for (physx::PxU32 i = 0; i < count; i++)
			callback->OnConstraintBreak(static_cast<ZnJoint*>(constraints[i].constraint->userData));
	}

	void EventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		// ���� �� ��
	}

	void EventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		// ���� �� ��
	}

	void EventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs,
		physx::PxU32 nbPairs)
	{
		using namespace physx;

		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
			return;

		ZnCollision collision;
		PxContactPairExtraDataIterator pairData(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);

		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			// ���� ���� ���������� ������
			const PxContactPair& cp = pairs[i];
			PxContactStreamIterator itr(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(), cp.patchCount, cp.contactCount);

			// ��ݷ��� �迭
			const float* impulses = cp.contactImpulses;
			const bool hasImpulse = cp.flags.isSet(PxContactPairFlag::eINTERNAL_HAS_IMPULSES);
			const bool hasPostVelocities = !cp.flags.isSet(PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH);
			uint32_t contacts = 0;
			Eigen::Vector3f totalImpulse{ 0.f, 0.f, 0.f };

			const auto thisCollider = GetCollider(cp.shapes[0]);
			const auto otherCollider = GetCollider(cp.shapes[1]);

			assert(thisCollider != nullptr && otherCollider != nullptr);

			while (itr.hasNextPatch())
			{
				itr.nextPatch();

				while (itr.hasNextContact(), contacts++)
				{
					itr.nextContact();

					collision.contactCount = itr.getTotalContactCount();
					collision.contacts = new ZnContact[collision.contactCount];

					const auto point = PhysxToEigen(itr.getContactPoint());
					const auto normal = PhysxToEigen(itr.getContactNormal());
					const auto separation = itr.getSeparation();

					if (hasImpulse)
					{
						const auto impulse = impulses[contacts];
						totalImpulse += normal * impulse;
					}

					ZnContact& contact = collision.contacts[contacts];
					contact.normal = normal;
					contact.point = point;
					contact.separation = separation;
				}
			}

			if (hasPostVelocities && pairData.nextItemSet())
			{
				assert(pairData.contactPairIndex == i);

				if (pairData.postSolverVelocity)
				{
					collision.thisLinearVelocity = PhysxToEigen(pairData.postSolverVelocity->linearVelocity[0]);
					collision.otherLinearVelocity = PhysxToEigen(pairData.postSolverVelocity->linearVelocity[1]);

					collision.thisAngularVelocity = PhysxToEigen(pairData.postSolverVelocity->angularVelocity[0]);
					collision.otherAngularVelocity = PhysxToEigen(pairData.postSolverVelocity->angularVelocity[1]);
				}
			}

			collision.impulses = totalImpulse;

			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				callback->OnCollisionEnter(thisCollider, otherCollider, collision);

			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
				callback->OnCollisionStay(thisCollider, otherCollider, collision);

			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				callback->OnCollisionExit(thisCollider, otherCollider, collision);
			else
				assert(false);

			delete[] collision.contacts;
		}
	}

	void EventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		using namespace physx;

		for (physx::PxU32 i = 0; i < count; i++)
		{
			const PxTriggerPair& tp = pairs[i];

			if (tp.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;

			const auto trigger = GetCollider(tp.triggerShape);
			const auto other = GetCollider(tp.otherShape);

			assert(trigger != nullptr && other != nullptr);

			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				callback->OnTriggerEnter(trigger, other);

			else if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
				callback->OnTriggerExit(trigger, other);

			else
				assert(false);
		}
	}

	void EventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer,
		const physx::PxU32 count)
	{
		// ���� �� ��
	}

	ZnCollider* EventCallback::GetCollider(physx::PxShape* _shape)
	{
		return static_cast<ZnCollider*>(_shape->userData);
	}
}
