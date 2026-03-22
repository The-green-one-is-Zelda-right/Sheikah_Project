#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "GraphicsResourceManager.h"

namespace PurahEngine
{
	Renderer::Renderer()
	{
		GraphicsManager::GetInstance().AddRenderer(this);
	}

	Renderer::~Renderer()
	{
		GraphicsManager::GetInstance().RemoveRenderer(this);
	}

	bool Renderer::IsRootEnable()
	{
		return GetGameObject()->IsRootEnable();
	}

	TextureID Renderer::GetTextureID(std::wstring textureName)
	{
		return GraphicsManager::GetInstance().resourceManager->GetTextureID(textureName);
	}

	ModelID Renderer::GetModelID(std::wstring modelName)
	{
		return GraphicsManager::GetInstance().resourceManager->GetModelID(modelName);
	}
}