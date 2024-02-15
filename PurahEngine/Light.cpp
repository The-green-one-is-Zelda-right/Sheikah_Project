#include "Light.h"

#include "GraphicsManager.h"
#include "GraphicsResourceManager.h"
#include "GameObject.h"

namespace PurahEngine
{
	Light::Light() :
		ambient({ 0.2f, 0.2f, 0.2f }),
		diffuse({ 1.0f, 1.0f, 1.0f }),
		specular({ 1.0f, 1.0f, 1.0f }),
		lightID(LightID::ID_NULL)
	{
		GraphicsManager::GetInstance().AddLight(this);
	}

	Light::~Light()
	{
		GraphicsManager::GetInstance().RemoveLight(this);
	}
	
	void Light::SetLightAmbient(const Eigen::Vector3f& _ambient)
	{
		ambient = _ambient;
	}

	void Light::SetLightDiffuse(const Eigen::Vector3f& _diffuse)
	{
		diffuse = _diffuse;
	}

	void Light::SetLightSpecular(const Eigen::Vector3f& _specular)
	{
		specular = _specular;
	}

	void Light::Render(IZeldaRenderer* renderer)
	{
		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
		if (GetGameObject()->IsEnable())
		{
			renderer->DrawLight(lightID);
		}
	}

	void Light::CreateDirectionalLight()
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };

		lightID = GraphicsManager::GetInstance().resourceManager->CreateDirectionalLight(ambient, diffuse, specular, direction);
	}

	void Light::CreatePointLight(float range)
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f position = { 0.0f , 0.0f, 0.0f };

		lightID = GraphicsManager::GetInstance().resourceManager->CreatePointLight(ambient, diffuse, specular, position, range);
	}

	void Light::CreateSpotLight(float range, float angle)
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };
		Eigen::Vector3f position = { 0.0f , 0.0f, 0.0f };

		lightID = GraphicsManager::GetInstance().resourceManager->CreateSpotLight(ambient, diffuse, specular, direction, position, range, angle);
	}

	void Light::ReleaseLight()
	{
		assert(lightID != LightID::ID_NULL);

		GraphicsManager::GetInstance().resourceManager->ReleaseLight(lightID);
	}
}