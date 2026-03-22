#pragma once

#include "Light.h"

namespace PurahEngine
{
	class PURAHENGINE_API PointLight final : public Light
	{
	public:
		void Initialize() override;
		~PointLight();
	};
}