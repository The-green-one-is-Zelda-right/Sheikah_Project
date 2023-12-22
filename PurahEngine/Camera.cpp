#include "Camera.h"

const float DEFAULT_NEAR = 1.0f;
const float DEFAULT_FAR = 1000.0f;
const float DEFAULT_FOV = 3.141592654f / 4.0f;

PurahEngine::Camera::Camera()
	: cameraID(CameraID::ID_NULL), renderer(nullptr), 
	cameraNear(DEFAULT_NEAR), cameraFar(DEFAULT_FAR), fieldOfView(DEFAULT_FOV)
{
	
}

PurahEngine::Camera::~Camera()
{

}

CameraID PurahEngine::Camera::GetCameraID()
{
	return cameraID;
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

void PurahEngine::Camera::SetCameraID(CameraID camera)
{
	cameraID = camera;
}

void PurahEngine::Camera::SetRenderer(IZeldaRenderer* render)
{
	renderer = render;
}

void PurahEngine::Camera::CreateCamera()
{
	cameraID = renderer->CreateCamera();
}

void PurahEngine::Camera::SetMainCamera()
{
	renderer->SetMainCamera(cameraID);
}

void PurahEngine::Camera::UpdateCamera(const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar)
{
	renderer->UpdateCamera(cameraID, worldMatrix, fieldOfView, cameraNear, cameraFar);
}
