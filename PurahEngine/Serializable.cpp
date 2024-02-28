#include "Serializable.h"

void PurahEngine::Serializable::Serialize(json& jsonData) const
{
	PreSerialize(jsonData);
	PostSerialize(jsonData);
}

void PurahEngine::Serializable::Deserialize(const json& jsonData)
{
	PreDeserialize(jsonData);
	PostDeserialize(jsonData);
}
