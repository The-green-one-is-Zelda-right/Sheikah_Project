#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
#include "GraphicsManager.h"

namespace PurahEngine
{
	class GameObject;

	class PURAHENGINE_API Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera();

		float GetCameraNear();
		float GetCameraFar();
		float GetCameraFOV();

		CameraID GetCameraID();

		void SetCameraID(CameraID camera);
		void SetRenderer(IZeldaRenderer* render);

		void CreateCamera();
		void SetMainCamera();
		void UpdateCamera(const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar);

	private:
		float cameraNear;	// 최소 거리
		float cameraFar;	// 최대 거리
		float fieldOfView;	// 시야각 (보통 45 ~ 75를 사용한다고 함)

		const static float DEFUALT_FOV;
		const static float DEFUALT_NEAR;
		const static float DEFUALT_FAR;

	private:
		CameraID cameraID;
		IZeldaRenderer* renderer;

	};

}

