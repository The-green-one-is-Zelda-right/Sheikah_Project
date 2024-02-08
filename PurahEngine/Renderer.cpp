#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsManager.h"

PurahEngine::Renderer::Renderer()
{
	PurahEngine::GraphicsManager::GetInstance().AddRenderer(this);
}

PurahEngine::Renderer::~Renderer()
{

}

void PurahEngine::Renderer::Render(IZeldaRenderer* renderer)
{
	if (GetGameObject()->IsEnable())
	{

		Eigen::Matrix4f position = GetGameObject()->GetComponent<PurahEngine::Transform>()->GetWorldMatrix();

		renderer->DrawCube(position, texture, false, 0.1, 0.6, 0.6, 1);

	}
	if (light != LightID::ID_NULL)
	{
		renderer->DrawLight(light);
	}

}

void PurahEngine::Renderer::AddTexture(TextureID textureID)
{
	texture = textureID;
}

void PurahEngine::Renderer::AddLight(LightID lightID)
{
	light = lightID;
}
