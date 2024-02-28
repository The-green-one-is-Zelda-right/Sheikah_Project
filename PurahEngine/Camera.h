#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
#include "GraphicsManager.h"
#include "IRenderer.h"

namespace PurahEngine
{
	class GameObject;

	class PURAHENGINE_API Camera final : public Component, IRenderer
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

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

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

