#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API SpriteRenderer final : public Renderer
	{
	public:
		void OnEnable() override;
		void Update() override;

		void Render(IZeldaRenderer* renderer) override;
		
		void SetTexture(const std::wstring& fileName);

	private:
		bool useEnableEvent;
		float eventTime;
		float sr, sg, sb, sa;

		std::wstring textureName;
		float r, g, b, a;

	private:
		float elapsedTime;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}