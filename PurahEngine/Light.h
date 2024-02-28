#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"
#include "IRenderer.h"
#include <Eigen/Dense>

namespace PurahEngine
{
	class PURAHENGINE_API Light : public Component, IRenderer
	{
	protected:
		Light();
		virtual ~Light();

	public:
		void SetLightAmbient(const Eigen::Vector3f&);
		void SetLightDiffuse(const Eigen::Vector3f&);
		void SetLightSpecular(const Eigen::Vector3f&);

		// IRenderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

	protected:
		void CreateDirectionalLight();
		void CreatePointLight(float range);
		void CreateSpotLight(float range, float angle);

		void ReleaseLight();

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	protected:
		Eigen::Vector3f ambient;
		Eigen::Vector3f diffuse;
		Eigen::Vector3f specular;
		float range;
		float angle;

		LightID lightID;
	};
}