#include "Camera.h"

PurahEngine::Camera::Camera()
	: cameraID(renderer->CreateCamera())
{
	renderer->SetMainCamera(cameraID);	
}

PurahEngine::Camera::~Camera()
{

}

void PurahEngine::Camera::UpdateCamera(const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar)
{
	renderer->UpdateCamera(cameraID, worldMatrix, fieldOfView, cameraNear, cameraFar);
}
