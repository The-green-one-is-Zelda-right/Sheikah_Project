#pragma once

namespace ZonaiPhysics
{
	enum class eCallback : int
	{
		eError,
		eTriggerEnter,
		eTriggerStay,
		eTriggerExit,
		eCollisionEnter,
		eCollisionStay,
		eCollisionExit,
	};
}