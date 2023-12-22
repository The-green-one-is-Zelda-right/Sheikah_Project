#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
#include "IRenderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API Renderer : public Component, IRenderer
	{
	public:
		Renderer();
		~Renderer();

		virtual void Render(IZeldaRenderer* renderer) override;

		void AddTexture(TextureID textureID);
	private:
		TextureID texture;

	};
}

