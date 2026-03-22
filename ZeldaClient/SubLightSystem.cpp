#include "SubLightSystem.h"

#include "LightSystem.h"

namespace Phyzzle
{
	void SubLightSystem::Update()
	{
		if (lightSystem == nullptr || targetTransform == nullptr)
		{
			return;
		}

		Eigen::Vector3f myPos = GetGameObject()->GetTransform()->GetWorldPosition();
		Eigen::Vector3f otherPos = targetTransform->GetWorldPosition();

		float dist = std::abs((otherPos - myPos).norm());

		if (dist <= minDistance)
		{
			lightSystem->UpdateLight(minColor, dist);
		}
		else if (maxDistance <= dist)
		{
			lightSystem->UpdateLight(maxColor, dist);
		}
		else
		{
			float ratio = std::clamp((dist - minDistance) / (maxDistance - minDistance), 0.0f, 1.0f);
			Eigen::Vector3f interpolatedColor = ((1.0f - ratio) * minColor) + (ratio * maxColor);
			lightSystem->UpdateLight(interpolatedColor, dist);
		}
	}

	void SubLightSystem::OnTriggerEnter(const PurahEngine::Collider* other)
	{
		if (lightSystem == nullptr)
		{
			return;
		}

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

		if (targetTransform == nullptr)
		{
			targetTransform = other->GetGameObject()->GetTransform();
		}
	}

	void SubLightSystem::OnTriggerExit(const PurahEngine::Collider* other)
	{
		if (targetTransform != nullptr)
		{
			if (targetTransform == other->GetGameObject()->GetTransform())
			{
				targetTransform = nullptr;
			}
		}
	}

	void SubLightSystem::PreSerialize(json& jsonData) const
	{
	}
	void SubLightSystem::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		containTags.clear();
		for (int i = 0; i < jsonData["containTags"].size(); i++)
		{
			std::string str = jsonData["containTags"][i];
			std::wstring wstr = std::wstring(str.begin(), str.end());

			containTags.push_back(wstr);
		}

		PREDESERIALIZE_VALUE(maxDistance);
		PREDESERIALIZE_VALUE(minDistance);
		PREDESERIALIZE_VECTOR3F(maxColor);
		PREDESERIALIZE_VECTOR3F(minColor);
	}
	void SubLightSystem::PostSerialize(json& jsonData) const
	{
	}
	void SubLightSystem::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(lightSystem);
	}
}