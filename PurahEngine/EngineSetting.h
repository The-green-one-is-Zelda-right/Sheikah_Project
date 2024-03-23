#pragma once
#include "PurahEngineAPI.h"
#include "SerializableDefine.h"

#include <unordered_map>

namespace PurahEngine
{
	class GameObject;

	class EngineSetting : public Serializable
	{
	public:
		void SetTag(std::wstring tagName, GameObject* object);
		std::vector<GameObject*> GetTagObject(std::wstring tagName);

	private:
		std::unordered_map<std::wstring, std::vector<GameObject*>> tagList;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		static EngineSetting& GetInstance();
	};
}

