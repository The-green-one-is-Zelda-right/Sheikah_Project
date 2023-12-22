#pragma once

#include "IZeldaRenderer.h"

namespace PurahEngine
{
	class IRenderer
	{
	public:
		virtual void Render(IZeldaRenderer* renderer) abstract;
	};
}