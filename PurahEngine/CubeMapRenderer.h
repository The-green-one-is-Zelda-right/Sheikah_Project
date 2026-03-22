#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API CubeMapRenderer final : public Renderer
	{
	public:
		void Render(IZeldaRenderer* renderer) override;

	private:
		std::wstring textureName;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}