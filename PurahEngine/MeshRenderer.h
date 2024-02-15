#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API MeshRenderer : public Renderer
	{
	public:
		MeshRenderer();

		enum class MeshType
		{
			None,
			Cube
		};

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetTexture(const std::wstring& textureName);
		void SetMesh(MeshType type);

	private:
		std::wstring textureName;
		MeshType meshType;

		Color meshColor;
	};
}