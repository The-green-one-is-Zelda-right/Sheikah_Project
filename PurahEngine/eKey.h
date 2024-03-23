#pragma once


namespace PurahEngine
{
	enum class eKey : unsigned int
	{
		eKEY_MOUSE_L		= 0x01,
		eKEY_MOUSE_R		= 0x02,
		eKEY_CANCEL			= 0x03,
		eKEY_MOUSE_M		= 0x04,
		eKEY_MOUSE_X1		= 0x05,
		eKEY_MOUSE_X2		= 0x06,

		eKEY_BACK			= 0x08,
		eKEY_TAB			= 0x09,

		eKEY_CLEAR			= 0x0C,
		eKEY_RETURN			= 0x0D,
		eKEY_SHIFT			= 0x10,
		eKEY_CONTROL		= 0x11,
		eKEY_MENU			= 0x12,
		eKEY_PAUSE			= 0x13,

		eKEY_ESCAPE			= 0x1B,

		eKEY_SPACE			= 0x20,

		// 십자키
		eKEY_LEFT			= 0x25,
		eKEY_UP				= 0x26,
		eKEY_RIGHT			= 0x27,
		eKEY_DOWN			= 0x28,

		// 숫자키
		eKEY_0				= 0x30,
		eKEY_1				= 0x31,
		eKEY_2				= 0x32,
		eKEY_3				= 0x33,
		eKEY_4				= 0x34,
		eKEY_5				= 0x35,
		eKEY_6				= 0x36,
		eKEY_7				= 0x37,
		eKEY_8				= 0x38,
		eKEY_9				= 0x39,

		// 알파벳
		eKEY_A				= 0x41 ,
		eKEY_B				= 0x42 ,
		eKEY_C				= 0x43 ,
		eKEY_D				= 0x44 ,
		eKEY_E				= 0x45 ,
		eKEY_F				= 0x46 ,
		eKEY_G				= 0x47 ,
		eKEY_H				= 0x48 ,
		eKEY_I				= 0x49 ,
		eKEY_J				= 0x4A ,
		eKEY_K				= 0x4B ,
		eKEY_L				= 0x4C ,
		eKEY_M				= 0x4D ,
		eKEY_N				= 0x4E ,
		eKEY_O				= 0x4F ,
		eKEY_P				= 0x50 ,
		eKEY_Q				= 0x51 ,
		eKEY_R				= 0x52 ,
		eKEY_S				= 0x53 ,
		eKEY_T				= 0x54 ,
		eKEY_U				= 0x55 ,
		eKEY_V				= 0x56 ,
		eKEY_W				= 0x57 ,
		eKEY_X				= 0x58 ,
		eKEY_Y				= 0x59 ,
		eKEY_Z				= 0x5A ,

		// 숫자 키패드
		eKEY_NUM0			= 0x60 ,
		eKEY_NUM1			= 0x61 ,
		eKEY_NUM2			= 0x62 ,
		eKEY_NUM3			= 0x63 ,
		eKEY_NUM4			= 0x64 ,
		eKEY_NUM5			= 0x65 ,
		eKEY_NUM6			= 0x66 ,
		eKEY_NUM7			= 0x67 ,
		eKEY_NUM8			= 0x68 ,
		eKEY_NUM9			= 0x69 ,

		// Function 키
		eKEY_F1				= 0x70 ,
		eKEY_F2				= 0x71 ,
		eKEY_F3				= 0x72 ,
		eKEY_F4				= 0x73 ,
		eKEY_F5				= 0x74 ,
		eKEY_F6				= 0x75 ,
		eKEY_F7				= 0x76 ,
		eKEY_F8				= 0x77 ,
		eKEY_F9				= 0x78 ,
		eKEY_F10			= 0x79 ,
		eKEY_F11			= 0x7A ,
		eKEY_F12			= 0x7B ,

		//
		eKEY_LSHIFT			= 0xA0 ,
		eKEY_RSHIFT			= 0xA1 ,
		eKEY_LCONTROL		= 0xA2 ,
		eKEY_RCONTROL		= 0xA3 ,
	};
}
