#include "EventCallbackSystem.h"

namespace PurahEngine
{
	void EventCallbackSystem::OnWake(const ZonaiPhysics::ZnRigidBody*)
	{
		// ���� �� ��
	}

	void EventCallbackSystem::OnSleep(const ZonaiPhysics::ZnRigidBody*)
	{
		// ���� �� ��
	}

	void EventCallbackSystem::OnConstraintBreak(const ZonaiPhysics::ZnJoint*)
	{
		// ���� �� ��
	}

	void EventCallbackSystem::OnTriggerEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision&)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision&)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision&)
	{
		// 
	}
}
