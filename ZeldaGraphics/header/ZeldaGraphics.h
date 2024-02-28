#pragma once

#include "ZeldaGraphicsAPI.h"

#include <vector>

class IZeldaRenderer;

namespace ZeldaGraphics
{
	extern "C" ZELDAGRAPHICS_API IZeldaRenderer* CreateZeldaRenderer();
	extern "C" ZELDAGRAPHICS_API void ReleaseZeldaRenderer(IZeldaRenderer* renderer);
}