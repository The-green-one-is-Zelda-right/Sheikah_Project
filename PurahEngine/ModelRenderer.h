#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class Animator;

	class PURAHENGINE_API ModelRenderer : public Renderer
	{
	public:
		void Initialize() override;

		//void OnDestroy();

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetModelName(const std::wstring& modelName);

	private:
		void RemoveAnimator();

		std::wstring modelName;

		bool loop;
		bool activeAnimationBlend;
		std::wstring animationName;
		std::wstring nextAnimationName;
		float time1;
		float time2;

		float playElapsed;
		float playSpeed;
		float animationTime;
		float blendTime;

		friend Animator;
	};
}