#pragma once
#include "TemplateFlag.h"

namespace PurahEngine
{
	enum class eObjectProperty : unsigned int
	{
		eConductor				= 0x01 << 0,	// 전도성
		eEnableHandling			= 0x01 << 1,	// 울트라핸드 가능 여부
		ePortable				= 0x01 << 2,	// 플레이어가 들 수 있는가
		eFlammable				= 0x01 << 3,	// 가연성
		e4						= 0x01 << 4,	// 
		e5						= 0x01 << 5,	// 
		e6						= 0x01 << 6,	// 
		e7						= 0x01 << 7,	// 
	};

	class ObjectProperty : public TemplateFlag<eObjectProperty, int>
	{
	public:
	};
}

