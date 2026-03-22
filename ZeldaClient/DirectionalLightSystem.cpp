#include "DirectionalLightSystem.h"

namespace Phyzzle
{
	void DirectionalLightSystem::Awake()
	{
		directionalLight = GetGameObject()->GetComponent<PurahEngine::DirectionalLight>();
		
		if (directionalLight != nullptr)
		{
			originColor = directionalLight->GetLightAmbient();
		}

		targetColor = Eigen::Vector3f::Zero();
		countUpdateLight = 0;
	}

	void DirectionalLightSystem::LateUpdate()
	{
		if (directionalLight != nullptr)
		{
			if (countUpdateLight == 0)
			{
				directionalLight->SetLightAmbient(originColor);
			}
			else
			{
				directionalLight->SetLightAmbient(targetColor);
			}
		}

		targetColor = Eigen::Vector3f::Zero();
		countUpdateLight = 0;
	}

	void DirectionalLightSystem::UpdateLight(Eigen::Vector3f lightColor, float distance)
	{
		if (targetColor.norm() < lightColor.norm())
		{
			targetColor = lightColor;
			countUpdateLight += 1;
		}
	}

	void DirectionalLightSystem::PreSerialize(json& jsonData) const
	{
	}
	void DirectionalLightSystem::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}
	void DirectionalLightSystem::PostSerialize(json& jsonData) const
	{
	}
	void DirectionalLightSystem::PostDeserialize(const json& jsonData)
	{
	}
}