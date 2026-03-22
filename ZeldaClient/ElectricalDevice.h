#pragma once

#include "PurahEngine.h"
#include "IElectricalDevice.h"

namespace Phyzzle
{
	class ElectricalDevice : public PurahEngine::Component, public IElectricalDevice
	{

	};
}