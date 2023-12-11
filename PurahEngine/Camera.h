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
		float GetCameraNear();
		float GetCameraFar();
		float GetCameraFOV();

		void UpdateCamera(const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar);

	private:
		float cameraNear;	// �ּ� �Ÿ�
		float cameraFar;	// �ִ� �Ÿ�
		float fieldOfView;	// �þ߰� (���� 45 ~ 75�� ����Ѵٰ� ��)

		const static float DEFUALT_FOV;
		const static float DEFUALT_NEAR;
		const static float DEFUALT_FAR;

	private:
		CameraID cameraID;
		IZeldaRenderer* renderer;

	};

}

