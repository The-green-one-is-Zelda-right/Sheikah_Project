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

		// OnDestroy에서 Animator가 가리키는 ModelRenderer를 제거하는 기능 구현 필요

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetModelName(const std::wstring& modelName);
		void SetDShadow(bool value);
		void SetShadow(bool value);

		void SetFastOutLine(bool value);
		bool GetFastOutLine() const;

		void SetOutLine(bool value);
		void SetOutLineColor(const Eigen::Vector4f& _color);
		bool GetOutLine() const;

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		Animator* animator;
		
		std::wstring modelName;
		bool dShadow;	// Directional Light Shadow
		bool shadow;	// Point Light Shadow
		bool wireFrame;
		bool fastOutLine;
		bool outLine;
		float outLineR;
		float outLineG;
		float outLineB;
		float outLineA;
		
		bool isBlending;
		std::wstring animationName1;
		std::wstring animationName2;
		float time1;
		float time2;
		float ratio;


		friend Animator;
	};
}