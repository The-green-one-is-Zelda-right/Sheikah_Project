#pragma once
#include "PurahEngineAPI.h"
#include "ISerializable.h"

namespace PurahEngine
{
	class PURAHENGINE_API Serializable : public ISerializable
	{
	public:
		virtual void Serialize(json& jsonData) const override;
		virtual void Deserialize(const json& jsonData) override;


	protected:
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
	};
}

