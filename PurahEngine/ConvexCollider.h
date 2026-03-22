#pragma once
#include "PurahEngineAPI.h"
#include "CustomCollider.h"

namespace PurahEngine
{
	class PURAHENGINE_API ConvexCollider : public CustomCollider
	{
	public:
		ConvexCollider() = default;
		~ConvexCollider() override;

	public:
		void PostInitialize() override;

	public:
		void PreDeserialize(const json& jsonData) override;
	};
}