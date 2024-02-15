#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API ModelRenderer : public Renderer
	{
	public:
		// Renderer��(��) ���� ��ӵ�
		void Render(IZeldaRenderer* renderer) override;

		void SetModelName(const std::wstring& modelName);

	private:
		std::wstring modelName;

	};
}