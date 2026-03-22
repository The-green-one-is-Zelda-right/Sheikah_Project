#include "CubeMapRenderer.h"

namespace PurahEngine
{
	void CubeMapRenderer::Render(IZeldaRenderer* renderer)
	{
		TextureID textureID = TextureID::ID_NULL;
		if (textureName != L"")
		{
			textureID = GetTextureID(textureName);
			renderer->DrawCubeMap(textureID);
		}
	}
	void CubeMapRenderer::PreSerialize(json& jsonData) const
	{
	}
	void CubeMapRenderer::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_WSTRING(textureName);
	}
	void CubeMapRenderer::PostSerialize(json& jsonData) const
	{
	}
	void CubeMapRenderer::PostDeserialize(const json& jsonData)
	{
	}
}