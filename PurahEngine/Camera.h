#pragma once
#include "Component.h"
#include "IZeldaRenderer.h"
namespace PurahEngine
{
	class Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera();

		void UpdateCamera(const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar);
	private:
		CameraID cameraID;
		IZeldaRenderer* renderer;
	};
}

