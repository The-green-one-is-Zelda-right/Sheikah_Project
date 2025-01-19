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
	std::vector<CollisionEventBuffer> EventCallback::collisionBuffer{};
	std::vector<TriggerEventBuffer> EventCallback::currTriggerBuffer{};
	std::vector<TriggerEventBuffer> EventCallback::prevTriggerBuffer{};

	void EventCallback::setInstance(ZnSimulationCallback* _instance)
	{
		assert(_instance);

		callback = _instance;
	}

	void EventCallback::SimulationEventCallback()
	{
		// collision event
		for (auto& buffer : collisionBuffer)
		{
			auto& thisCollider = buffer.col0;
			auto& otherCollider = buffer.col1;
			auto& thisCollision = buffer.collisionData;
			auto otherCollision = buffer.collisionData.Swap();
			auto event = buffer.event;

			switch (event)
			{
			case eCallback::eCollisionEnter:
				callback->OnCollisionEnter(thisCollider, otherCollider, thisCollision, otherCollision);
				break;
			case eCallback::eCollisionStay:
				callback->OnCollisionStay(thisCollider, otherCollider, thisCollision, otherCollision);
				break;
			case eCallback::eCollisionExit:
				callback->OnCollisionExit(thisCollider, otherCollider, thisCollision, otherCollision);
				break;
			default:
				break;
			}

			thisCollision.Release();
			otherCollision.Release();
		}

		collisionBuffer.clear();

		// trigger event
		for (auto& buffer : currTriggerBuffer)
		{
			auto& thisCollider = buffer.col0;
			auto& otherCollider = buffer.col1;
			auto event = buffer.event;

			switch (event)
			{
			case eCallback::eTriggerEnter:
				callback->OnTriggerEnter(thisCollider, otherCollider);
				break;
			case eCallback::eTriggerStay:
				callback->OnTriggerStay(thisCollider, otherCollider);
				break;
			case eCallback::eTriggerExit:
				callback->OnTriggerExit(thisCollider, otherCollider);
				break;
			default:
				break;
			}
		}

		// 현재 프레임의 트리거 상태를 이전 프레임의 상태로 업데이트
		prevTriggerBuffer = std::move(currTriggerBuffer);
		currTriggerBuffer.clear();
	}

	void EventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		// 파괴된 제약의 정보를 넘겨줌
		for (physx::PxU32 i = 0; i < count; i++)
		{
			auto constrain = constraints[i];

			const auto joint = static_cast<ZnJoint*>(constrain.constraint->userData);

			if (joint)
			{
				callback->OnConstraintBreak(joint);
			}
		}
	}

	void EventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		for (physx::PxU32 i = 0; i < count; i++)
		{
			auto actor = actors[i];

			const auto body = static_cast<ZnRigidBody*>(actor->userData);

			callback->OnWake(body);
		}
	}

	void EventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		for (physx::PxU32 i = 0; i < count; i++)
		{
			auto actor = actors[i];

			const auto body = static_cast<ZnRigidBody*>(actor->userData);

			callback->OnSleep(body);
		}
	}

	void EventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs,
		physx::PxU32 nbPairs)
	{
		using namespace physx;
		using namespace Eigen;

		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
			return;

		ZnCollision collision;
		PxContactPairExtraDataIterator pairData(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);

		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			// 접촉 페어에서 접촉점들을 가져옴
			const PxContactPair& cp = pairs[i];
			PxContactStreamIterator itr(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(), cp.patchCount, cp.contactCount);

			// 충격량의 배열
			const float* impulses = cp.contactImpulses;
			const bool hasImpulse = cp.flags.isSet(PxContactPairFlag::eINTERNAL_HAS_IMPULSES);
			const bool hasPostVelocities = !cp.flags.isSet(PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH);
			uint32_t contacts = 0;
			Eigen::Vector3f totalImpulse = Eigen::Vector3f::Zero();

			ZnCollider* const thisCollider = GetCollider(cp.shapes[0]);
			ZnCollider* const otherCollider = GetCollider(cp.shapes[1]);

			if (thisCollider == nullptr || otherCollider == nullptr)
				continue;

			while (itr.hasNextPatch())
			{
				itr.nextPatch();
				collision.contactCount = itr.getTotalContactCount();

				if (!collision.contactCount)
					continue;

				collision.contacts = std::make_unique<ZnContact[]>(collision.contactCount);

				while (itr.hasNextContact())
				{
					itr.nextContact();

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

					contacts++;
				}
			}

			if (hasPostVelocities && pairData.nextItemSet())
			{
				if (pairData.postSolverVelocity)
				{
					collision.thisPostLinearVelocity = PhysxToEigen(pairData.postSolverVelocity->linearVelocity[0]);
					collision.thisPostAngularVelocity = PhysxToEigen(pairData.postSolverVelocity->angularVelocity[0]);
					collision.otherPostLinearVelocity = PhysxToEigen(pairData.postSolverVelocity->linearVelocity[1]);
					collision.otherPostAngularVelocity = PhysxToEigen(pairData.postSolverVelocity->angularVelocity[1]);
				}
			}

			collision.impulses = totalImpulse;

			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				collisionBuffer.emplace_back(thisCollider, otherCollider, collision, eCallback::eCollisionEnter);

			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
				collisionBuffer.emplace_back(thisCollider, otherCollider, collision, eCallback::eCollisionStay);

			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				collisionBuffer.emplace_back(thisCollider, otherCollider, collision, eCallback::eCollisionExit);

			else
				assert(false);
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

			ZnCollider* const trigger = GetCollider(tp.triggerShape);
			ZnCollider* const other = GetCollider(tp.otherShape);

			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				currTriggerBuffer.emplace_back(trigger, other, eCallback::eTriggerEnter);
			}
			else if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				currTriggerBuffer.emplace_back(trigger, other, eCallback::eTriggerExit);
			}

			// 현재 프레임에서의 트리거 상태를 이전 프레임과 비교하여 eTriggerStay 이벤트 처리
			auto it = std::find(prevTriggerBuffer.begin(), prevTriggerBuffer.end(), TriggerEventBuffer{ trigger, other });
			if (it != prevTriggerBuffer.end() && !(tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST))
			{
				currTriggerBuffer.emplace_back(trigger, other, eCallback::eTriggerStay);
			}
		}
	}

	void EventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer,
		const physx::PxU32 count)
	{
		// 구현 안 함
	}

	ZnCollider* EventCallback::GetCollider(physx::PxShape* _shape)
	{
		return static_cast<ZnCollider*>(_shape->userData);
	}

	bool CharacterControllerCallback::filter(const physx::PxController& a, const physx::PxController& b)
	{
		// Early out to avoid crashing
		physx::PxShape* shapeA = getShape(a);
		if (!shapeA)
			return false;

		physx::PxShape* shapeB = getShape(b);
		if (!shapeB)
			return false;

		// Let triggers through
		if (physx::PxFilterObjectIsTrigger(shapeB->getFlags()))
			return false;

		// Trigger the contact callback for pairs (A,B) where the filtermask of A contains the ID of B and vice versa
		const physx::PxFilterData shapeFilterA = shapeA->getQueryFilterData();
		const physx::PxFilterData shapeFilterB = shapeB->getQueryFilterData();
		if (shapeFilterA.word0 & shapeFilterB.word1)
			return true;

		return false;
	}
}
