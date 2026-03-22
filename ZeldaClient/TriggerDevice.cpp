#include "TriggerDevice.h"

namespace Phyzzle
{
	void TriggerDevice::Awake()
	{
		isPowerOn = false;
		triggerColliders.clear();
	}

	void TriggerDevice::OnTriggerEnter(const PurahEngine::Collider* other)
	{
		PurahEngine::Tag& othertag = other->GetGameObject()->tag;
		
		// containTag가 없다면 리턴
		if (containTags.size() == 0)
		{
			return;
		}

		for (auto& containTag : containTags)
		{
			if (othertag.IsContain(containTag) == false)
			{
				// 하나의 containTag라도 만족하지 않는다면 리턴
				return;
			}
		}

		if (triggerColliders.size() == 0)
		{
			TargetPowerOn();
		}
		
		triggerColliders.insert(other);
	}

	void TriggerDevice::OnTriggerExit(const PurahEngine::Collider* other)
	{
		auto& othertag = other->GetGameObject()->tag;

		// containTag가 없다면 리턴
		if (containTags.size() == 0)
		{
			return;
		}

		for (auto& containTag : containTags)
		{
			if (othertag.IsContain(containTag) == false)
			{
				// 하나의 containTag라도 만족하지 않는다면 리턴
				return;
			}
		}

		triggerColliders.erase(other);

		if (triggerColliders.size() == 0)
		{
			TargetPowerOff();
		}
	}

	void TriggerDevice::TargetPowerOn()
	{
		if (isPowerOn == true)
		{
			return;
		}

		isPowerOn = true;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOn();
		}
	}

	void TriggerDevice::TargetPowerOff()
	{
		if (onlyPowerOn)
		{
			return;
		}

		if (isPowerOn == false)
		{
			return;
		}

		isPowerOn = false;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOff();
		}
	}

	void TriggerDevice::PreSerialize(json& jsonData) const
	{
	}

	void TriggerDevice::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(onlyPowerOn);
		
		containTags.clear();
		for (int i = 0; i < jsonData["containTags"].size(); i++)
		{
			std::string str = jsonData["containTags"][i];
			std::wstring wstr = std::wstring(str.begin(), str.end());

			containTags.push_back(wstr);
		}
	}

	void TriggerDevice::PostSerialize(json& jsonData) const
	{
	}

	void TriggerDevice::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}