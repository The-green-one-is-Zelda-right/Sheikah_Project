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
		float cameraNear;	// �ּ� �Ÿ�
		float cameraFar;	// �ִ� �Ÿ�
		float fieldOfView;	// �þ߰� (���� 45 ~ 75�� ����Ѵٰ� ��)

		const static float DEFAULT_FOV;
		const static float DEFAULT_NEAR;
		const static float DEFAULT_FAR;

	private:
		CameraID cameraID;

	};

}

