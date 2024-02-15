#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
#include "IRenderer.h"

namespace PurahEngine
{
	class Transform;

	class PURAHENGINE_API Renderer : public Component, IRenderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		void Awake() override;

		//void AddTexture(TextureID textureID);
		//void AddLight(LightID lightID);

	protected:
		TextureID GetTextureID(std::wstring textureName);
		ModelID GetModelID(std::wstring modelName);

	protected:
		Transform* transform;
	};
}

