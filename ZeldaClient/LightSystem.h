#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class LightSystem : public PurahEngine::Component
	{
	public:
		virtual void UpdateLight(Eigen::Vector3f lightColor, float distance) abstract;
	};
}