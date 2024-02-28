#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
#include "IRenderer.h"

namespace PurahEngine
{
	class Transform;

	class PURAHENGINE_API Renderer : public Component, IRenderer
	{
	protected:
		Renderer();
		virtual ~Renderer();

	protected:
		TextureID GetTextureID(std::wstring textureName);
		ModelID GetModelID(std::wstring modelName);
	};
}

