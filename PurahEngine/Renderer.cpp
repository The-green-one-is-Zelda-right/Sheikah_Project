#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsManager.h"
#include "GraphicsResourceManager.h"

namespace PurahEngine
{
	Renderer::Renderer() :
		transform(nullptr)
	{
		GraphicsManager::GetInstance().AddRenderer(this);
	}

	Renderer::~Renderer()
	{
		GraphicsManager::GetInstance().RemoveRenderer(this);
	}

	void Renderer::Awake()
	{
		transform = GetGameObject()->GetComponent<Transform>();
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