#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
#include "GraphicsManager.h"
#include "IRenderer.h"

namespace PurahEngine
{
	class GameObject;

	class PURAHENGINE_API Camera : public Component, IRenderer
	{
	public:
		Camera();
		virtual ~Camera();

		void SetCameraNear(float cameraNear);
		void SetCameraFar(float cameraFar);
		void SetCameraFOV(float fov);

		float GetCameraNear();
		float GetCameraFar();
		float GetCameraFOV();

		void SetMainCamera();

		void Render(IZeldaRenderer* renderer) override;

	private:
		float cameraNear;	// 최소 거리
		float cameraFar;	// 최대 거리
		float fieldOfView;	// 시야각 (보통 45 ~ 75를 사용한다고 함)

		const static float DEFAULT_FOV;
		const static float DEFAULT_NEAR;
		const static float DEFAULT_FAR;

	private:
		CameraID cameraID;

	};

}

