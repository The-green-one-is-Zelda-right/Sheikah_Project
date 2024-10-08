#include "Camera.h"

#include "SceneManager.h"
#include "GraphicsResourceManager.h"
#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	const float Camera::DEFAULT_NEAR = 1.0f;
	const float Camera::DEFAULT_FAR = 1000.0f;
	const float Camera::DEFAULT_FOV = 3.141592654f / 4.0f;

	PurahEngine::Camera::Camera()
		: cameraID(CameraID::ID_NULL),
		cameraNear(DEFAULT_NEAR),
		cameraFar(DEFAULT_FAR),
		fieldOfView(DEFAULT_FOV)
	{
		cameraID = GraphicsManager::GetInstance().resourceManager->CreateCamera();
	}

	PurahEngine::Camera::~Camera()
	{
		GraphicsManager::GetInstance().resourceManager->ReleaseCamera(cameraID);
		
		if (SceneManager::GetInstance().GetMainCamera() == this)
		{
			SceneManager::GetInstance().SetMainCamera(nullptr);
		}
	}

	void PurahEngine::Camera::SetCameraNear(float cameraNear)
	{
		this->cameraNear = cameraNear;
	}

	void PurahEngine::Camera::SetCameraFar(float cameraFar)
	{
		this->cameraFar = cameraFar;
	}

	void PurahEngine::Camera::SetCameraFOV(float fov)
	{
		this->fieldOfView = fov;
	}

	float PurahEngine::Camera::GetCameraNear()
	{
		return cameraNear;
	}

	float PurahEngine::Camera::GetCameraFar()
	{
		return cameraFar;
	}

	float PurahEngine::Camera::GetCameraFOV()
	{
		return fieldOfView;
	}

	void PurahEngine::Camera::SetMainCamera()
	{
		SceneManager::GetInstance().SetMainCamera(this);
	}

	bool Camera::IsRootEnable()
	{
		return GetGameObject()->IsRootEnable();
	}

	void PurahEngine::Camera::Render(IZeldaRenderer* renderer)
	{
		renderer->SetMainCamera(cameraID);

		auto worldTM = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		renderer->UpdateCamera(cameraID, worldTM, fieldOfView, cameraNear, cameraFar);
	}

	void PurahEngine::Camera::PreSerialize(json& jsonData) const
	{

	}

	void PurahEngine::Camera::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(cameraNear);
		PREDESERIALIZE_VALUE(cameraFar);
		PREDESERIALIZE_VALUE(fieldOfView);
		if (jsonData["mainCamera"] == true)
		{
			SetMainCamera();
		}
	}

	void PurahEngine::Camera::PostSerialize(json& jsonData) const
	{

	}

	void PurahEngine::Camera::PostDeserialize(const json& jsonData)
	{

	}
}