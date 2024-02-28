#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class Animator;

	class PURAHENGINE_API ModelRenderer final : public Renderer
	{
	public:
		~ModelRenderer();

		void Initialize() override;

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetModelName(const std::wstring& modelName);

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		Animator* animator;
		
		std::wstring modelName;
		
		bool isBlending;
		std::wstring animationName1;
		std::wstring animationName2;
		float time1;
		float time2;
		float ratio;


		friend Animator;
	};
}