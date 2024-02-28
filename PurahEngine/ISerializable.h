#pragma once
#include "PurahEngineAPI.h"
#include "nlohmann/json.hpp"


using json = nlohmann::json;


namespace PurahEngine
{
	class PURAHENGINE_API ISerializable
	{
	public:
		virtual void Serialize(json& jsonData) const abstract;
		virtual void Deserialize(const json& jsonData) abstract;


	protected:
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
	};
}