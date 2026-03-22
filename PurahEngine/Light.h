#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"
#include "IRenderer.h"
#include <Eigen/Dense>
#include <random>

namespace PurahEngine
{
	class PURAHENGINE_API Light : public Component, IRenderer
	{
	protected:
		Light();
		virtual ~Light();

	public:
		void Awake() override;
		void Update() override;

	public:
		void SetLightAmbient(const Eigen::Vector3f&);
		void SetLightDiffuse(const Eigen::Vector3f&);
		void SetLightSpecular(const Eigen::Vector3f&);

		Eigen::Vector3f GetLightAmbient() const;
		Eigen::Vector3f GetLightDiffuse() const;
		Eigen::Vector3f GetLightSpecular() const;

		void SetAttenuation(const Eigen::Vector3f& attenuation);
		Eigen::Vector3f GetAttenuation() const;

		// IRenderer을(를) 통해 상속됨
		bool IsRootEnable() override;
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
		Eigen::Vector3f shadowColor;
		float range;
		float angle;
		float atten0;
		float atten1;
		float atten2;

		float atten0VibrationSpeed;
		float atten0VibrationRange;

		float atten1VibrationSpeed;
		float atten1VibrationRange;

		float atten2VibrationSpeed;
		float atten2VibrationRange;

		float positionVibrationSpeed;
		float positionVibrationRange;

		LightID lightID;

	protected:
		// 진동을 위한 추가 변수
		float currentAtten0;
		float currentAtten1;
		float currentAtten2;

		float targetAtten0;
		float targetAtten1;
		float targetAtten2;

		Eigen::Vector3f currentPosition;
		Eigen::Vector3f targetPosition;
		Eigen::Vector3f vibrationDirection;

		std::random_device randomDevice;
		std::mt19937 genRandom;
		std::uniform_real_distribution<float> atten0Distribution;
		std::uniform_real_distribution<float> atten1Distribution;
		std::uniform_real_distribution<float> atten2Distribution;
		std::uniform_real_distribution<float> positionDistribution;
	};
}