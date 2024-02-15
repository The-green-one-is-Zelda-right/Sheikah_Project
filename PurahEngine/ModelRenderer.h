#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API ModelRenderer : public Renderer
	{
	public:
		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetModelName(const std::wstring& modelName);

	private:
		std::wstring modelName;

	};
}