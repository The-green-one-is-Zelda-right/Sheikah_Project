#include "ZeldaGraphics.h"

#include "ZeldaDX11Renderer.h"

namespace ZeldaGraphics
{
	IZeldaRenderer* CreateZeldaRenderer()
	{
		return new ZeldaDX11Renderer();
	}

	void ReleaseZeldaRenderer(IZeldaRenderer* renderer)
	{
		delete renderer;
		renderer = nullptr;
	}
}