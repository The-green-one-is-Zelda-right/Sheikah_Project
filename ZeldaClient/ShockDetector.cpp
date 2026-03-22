#include "ShockDetector.h"

#include "ElectricalDevice.h"

namespace Phyzzle
{
	std::list<ShockDetector*> ShockDetector::objectList;

	ShockDetector::ShockDetector()
	{
		objectList.push_back(this);
	}

	ShockDetector::~ShockDetector()
	{
		for (std::list<ShockDetector*>::iterator iter = objectList.begin(); iter != objectList.end(); iter++)
		{
			if (*iter == this)
			{
				iter = objectList.erase(iter);
			}
		}
	}

	void ShockDetector::Awake()
	{
		isPowerOn = false;
		shock = 0.0f;
	}

	void ShockDetector::Update()
	{
		static bool debug = false;
		static float debugShock = 0.0f;

		if (*objectList.begin() == this)
		{
			if (debugShock < shock) debugShock = shock;

			if (PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_CONTROL) &&
				PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_Z))
			{
				debug = !debug;
				debugShock = 0.0f;
			}

			if (debug)
			{
				PurahEngine::GraphicsManager::GetInstance().DrawString(L"Activation : " + std::to_wstring(activationShock), 50, 400, 200, 300, 30, 1, 1, 1, 1);
				PurahEngine::GraphicsManager::GetInstance().DrawString(L"Max Shock  : " + std::to_wstring(debugShock), 50, 500, 200, 300, 30, 1, 1, 1, 1);
			}

			isMainObject = true;
		}
		else
		{
			isMainObject = false;
		}

		if (activationShock < shock)
		{
			shock = 0.0f;

			TargetPowerOn();
			TargetPowerOff();
		}
	}

	void ShockDetector::LateUpdate()
	{
		if (isMainObject)
		{
			if (PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_SHIFT) &&
				PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_Z))
			{
				for (std::list<ShockDetector*>::iterator iter = objectList.begin(); iter != objectList.end(); iter++)
				{
					if (*iter == this)
					{
						iter = objectList.erase(iter);
					}
				}

				objectList.push_back(this);
			}
		}
	}

	void ShockDetector::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other)
	{
		float currentShock = collision.impulses.norm();
		if (shock < currentShock) shock = currentShock;
	}

	void ShockDetector::TargetPowerOn()
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

	void ShockDetector::TargetPowerOff()
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

	void ShockDetector::PreSerialize(json& jsonData) const
	{
	}
	void ShockDetector::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(onlyPowerOn);
		PREDESERIALIZE_VALUE(activationShock);
	}
	void ShockDetector::PostSerialize(json& jsonData) const
	{
	}
	void ShockDetector::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}