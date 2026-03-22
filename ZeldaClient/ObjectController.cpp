#include "ObjectController.h"

namespace Phyzzle
{
	void ObjectController::Awake()
	{
		powerCounter = 0;
	}

	void ObjectController::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			TargetOn();
		}
	}

	void ObjectController::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1)
		{
			TargetOff();
		}
	}

	void ObjectController::TargetOn()
	{
		for (auto& object : targetObjectList)
		{
			object->SetEnable(!inverse);
		}
	}
	void ObjectController::TargetOff()
	{
		for (auto& object : targetObjectList)
		{
			object->SetEnable(inverse);
		}
	}

	void ObjectController::PreSerialize(json& jsonData) const
	{
	}
	void ObjectController::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(inverse);
	}
	void ObjectController::PostSerialize(json& jsonData) const
	{
	}
	void ObjectController::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetObjectList);
	}
}