#pragma once

namespace Phyzzle
{
	class IElectricalDevice
	{
	public:
		virtual void PowerOn() abstract;
		virtual void PowerOff() abstract;
	};
}