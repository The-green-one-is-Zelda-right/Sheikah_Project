#include "PointLightSystem.h"

namespace Phyzzle
{
	void PointLightSystem::Awake()
	{
		pointLight = GetGameObject()->GetComponent<PurahEngine::PointLight>();

		if (pointLight != nullptr)
		{
			originColor = pointLight->GetAttenuation();
		}

		targetColor = Eigen::Vector3f::Zero();
		countUpdateLight = 0;
	}

	void PointLightSystem::LateUpdate()
	{
		if (pointLight != nullptr)
		{
			if (countUpdateLight == 0)
			{
				pointLight->SetAttenuation(originColor);
			}
			else
			{
				pointLight->SetAttenuation(targetColor);
			}
		}

		targetColor = Eigen::Vector3f::Zero();
		countUpdateLight = 0;
	}

	void PointLightSystem::UpdateLight(Eigen::Vector3f lightColor, float distance)
	{
		if (targetColor.norm() < lightColor.norm())
		{
			targetColor = lightColor;
			countUpdateLight += 1;
		}
	}

	void PointLightSystem::PreSerialize(json& jsonData) const
	{
	}
	void PointLightSystem::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}
	void PointLightSystem::PostSerialize(json& jsonData) const
	{
	}
	void PointLightSystem::PostDeserialize(const json& jsonData)
	{
	}
}