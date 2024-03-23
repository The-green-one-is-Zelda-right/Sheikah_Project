#pragma once


namespace PurahEngine
{
	enum class ePad : unsigned int
	{
		ePAD_A				= 0x1000,
		ePAD_B				= 0x2000,
		ePAD_X				= 0x4000,
		ePAD_Y				= 0x8000,

		ePAD_UP				= 0x0001,
		ePAD_DOWN			= 0x0002,
		ePAD_LEFT			= 0x0004,
		ePAD_RIGHT			= 0x0008,

		ePAD_SHOULDER_L		= 0x0100,
		ePAD_SHOULDER_R		= 0x0200,

		ePAD_START			= 0x0010,
		ePAD_BACK			= 0x0020,

		ePAD_STICK_CLICK_L	= 0x0040,
		ePAD_STICK_CLICK_R	= 0x0080,
	};

	enum class ePadStick : unsigned int
	{
		ePAD_STICK_L,
		ePAD_STICK_R,
	};

	enum class ePadTrigger : unsigned int
	{
		ePAD_TRIGGER_L,
		ePAD_TRIGGER_R,
	};
}
