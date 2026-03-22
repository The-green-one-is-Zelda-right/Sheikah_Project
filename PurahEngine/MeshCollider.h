#pragma once
#include "PurahEngineAPI.h"
#include "CustomCollider.h"

namespace PurahEngine
{
	class PURAHENGINE_API MeshCollider : public CustomCollider
	{
	public:
		MeshCollider() = default;
		~MeshCollider() override;

	public:
		void PostInitialize() override;

	public:
		void PreDeserialize(const json& jsonData) override;
	};
}