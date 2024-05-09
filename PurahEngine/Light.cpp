#include "Light.h"

#include "GraphicsManager.h"
#include "GraphicsResourceManager.h"
#include "GameObject.h"

namespace PurahEngine
{
	// ���߿� Angle ���õȰ� ������ �� Radian�� Degree ����

	Light::Light() :
		ambient({ 0.2f, 0.2f, 0.2f }),
		diffuse({ 1.0f, 1.0f, 1.0f }),
		specular({ 1.0f, 1.0f, 1.0f }),
		lightID(LightID::ID_NULL),
		range(10.0f),
		angle(45.0f)
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

	bool Light::IsRootEnable()
	{
		return GetGameObject()->IsRootEnable();
	}

	void Light::Render(IZeldaRenderer* renderer)
	{
		// ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ� �ִ� ��쿡�� �۵��Ѵ�.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };
			direction = GetGameObject()->GetTransform()->GetWorldMatrix().block<3, 3>(0, 0) * direction;
			direction.normalize();

			Eigen::Vector3f worldPosition = GetGameObject()->GetTransform()->GetWorldPosition();

			renderer->UpdateLight(lightID, ambient, diffuse, specular, shadowColor, direction, worldPosition, range, angle);
			renderer->DrawLight(lightID);
		}
	}

	void Light::CreateDirectionalLight()
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };
		direction = GetGameObject()->GetTransform()->GetWorldMatrix().block<3, 3>(0, 0) * direction;
		direction.normalize();

		lightID = GraphicsManager::GetInstance().resourceManager->CreateDirectionalLight(ambient, diffuse, specular, shadowColor, direction);
	}

	void Light::CreatePointLight(float range)
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f position = GetGameObject()->GetTransform()->GetWorldPosition();

		lightID = GraphicsManager::GetInstance().resourceManager->CreatePointLight(ambient, diffuse, specular, shadowColor, position, range);
	}

	void Light::CreateSpotLight(float range, float angle)
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };
		direction = GetGameObject()->GetTransform()->GetWorldMatrix().block<3, 3>(0, 0) * direction;
		direction.normalize();
		Eigen::Vector3f position = GetGameObject()->GetTransform()->GetWorldPosition();

		lightID = GraphicsManager::GetInstance().resourceManager->CreateSpotLight(ambient, diffuse, specular, shadowColor, direction, position, range, angle);
	}

	void Light::ReleaseLight()
	{
		assert(lightID != LightID::ID_NULL);

		GraphicsManager::GetInstance().resourceManager->ReleaseLight(lightID);
	}

	void Light::PreSerialize(json& jsonData) const
	{

	}

	void Light::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(range);
		PREDESERIALIZE_VALUE(angle);
		PREDESERIALIZE_VECTOR3F(ambient);
		PREDESERIALIZE_VECTOR3F(diffuse);
		PREDESERIALIZE_VECTOR3F(specular);
		PREDESERIALIZE_VECTOR3F(shadowColor);
	}

	void Light::PostSerialize(json& jsonData) const
	{

	}

	void Light::PostDeserialize(const json& jsonData)
	{

	}
}