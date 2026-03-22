#include "Light.h"

#include "GraphicsManager.h"
#include "GraphicsResourceManager.h"
#include "GameObject.h"
#include "TimeController.h"

namespace PurahEngine
{
	// 나중에 Angle 관련된것 구현할 때 Radian과 Degree 주의

	Light::Light() :
		ambient({ 0.2f, 0.2f, 0.2f }),
		diffuse({ 1.0f, 1.0f, 1.0f }),
		specular({ 1.0f, 1.0f, 1.0f }),
		lightID(LightID::ID_NULL),
		range(10.0f),
		angle(45.0f),
		atten0(1.0f),
		atten1(0.0f),
		atten2(1.0f)
	{
		GraphicsManager::GetInstance().AddLight(this);
	}

	Light::~Light()
	{
		GraphicsManager::GetInstance().RemoveLight(this);
	}

	void Light::Awake()
	{
		// 진동 분포 초기화
		atten0Distribution = std::uniform_real_distribution<float>(0.0f, atten0VibrationRange);
		atten1Distribution = std::uniform_real_distribution<float>(0.0f, atten1VibrationRange);
		atten2Distribution = std::uniform_real_distribution<float>(0.0f, atten2VibrationRange);
		positionDistribution = std::uniform_real_distribution<float>(-1.0f, 1.0f);

		currentAtten0 = atten0;
		currentAtten1 = atten1;
		currentAtten2 = atten2;

		targetAtten0 = atten0;
		targetAtten1 = atten1;
		targetAtten2 = atten2;

		currentPosition = Eigen::Vector3f::Zero();
		targetPosition = Eigen::Vector3f::Zero();
	}

	void Light::Update()
	{
		float deltaTime = TimeController::GetInstance().GetDeltaTime();

		// Position의 진동
		if (positionVibrationRange > 0.0f)
		{
			if (currentPosition == targetPosition)
			{
				// 반지름 크기 1인 구 범위 내의 랜덤 위치 생성
				Eigen::Vector3f newPos;
				do
				{
					newPos = Eigen::Vector3f{ positionDistribution(genRandom), positionDistribution(genRandom), positionDistribution(genRandom) };
				} while (newPos.norm() > 1.0f);

				targetPosition = positionVibrationRange * newPos;
			}
			else
			{
				// 도착까지 움직여야 하는 거리 벡터
				Eigen::Vector3f needVec = targetPosition - currentPosition;
				float need = std::abs(needVec.norm());

				float moveDelta = std::abs(positionVibrationSpeed * deltaTime);

				if (need < moveDelta)
				{
					currentPosition = targetPosition;
				}
				else
				{
					Eigen::Vector3f direction = needVec.normalized();
					currentPosition += direction * moveDelta;
				}
			}
		}

		// Attenuation의 진동
		{
			float& _origin = atten0;
			float& _vRange = atten0VibrationRange;
			float& _current = currentAtten0;
			float& _target = targetAtten0;
			float& _vSpeed = atten0VibrationSpeed;
			auto& _distributaion = atten0Distribution;

			if (_vRange > 0.0f)
			{
				if (_current == _target)
				{
					_target = _origin + _distributaion(genRandom);
				}
				else
				{
					// 도착까지 움직여야 하는 거리
					float need = std::abs(_target - _current);

					// 이번에 움직일 수 있는 거리
					float moveDelta = std::abs(_vSpeed * deltaTime);

					if (need < moveDelta)
					{
						_current = _target;
					}
					else
					{
						float direction = (_target - _current) > 0.0f ? (1.0f) : (-1.0f);

						_current += direction * moveDelta;
					}
				}
			}
		}

		{
			float& _origin = atten1;
			float& _vRange = atten1VibrationRange;
			float& _current = currentAtten1;
			float& _target = targetAtten1;
			float& _vSpeed = atten1VibrationSpeed;
			auto& _distributaion = atten1Distribution;

			if (_vRange > 0.0f)
			{
				if (_current == _target)
				{
					_target = _origin + _distributaion(genRandom);
				}
				else
				{
					// 도착까지 움직여야 하는 거리
					float need = std::abs(_target - _current);

					// 이번에 움직일 수 있는 거리
					float moveDelta = std::abs(_vSpeed * deltaTime);

					if (need < moveDelta)
					{
						_current = _target;
					}
					else
					{
						float direction = (_target - _current) > 0.0f ? (1.0f) : (-1.0f);

						_current += direction * moveDelta;
					}
				}
			}
		}

		{
			float& _origin = atten2;
			float& _vRange = atten2VibrationRange;
			float& _current = currentAtten2;
			float& _target = targetAtten2;
			float& _vSpeed = atten2VibrationSpeed;
			auto& _distributaion = atten2Distribution;

			if (_vRange > 0.0f)
			{
				if (_current == _target)
				{
					_target = _origin + _distributaion(genRandom);
				}
				else
				{
					// 도착까지 움직여야 하는 거리
					float need = std::abs(_target - _current);

					// 이번에 움직일 수 있는 거리
					float moveDelta = std::abs(_vSpeed * deltaTime);

					if (need < moveDelta)
					{
						_current = _target;
					}
					else
					{
						float direction = (_target - _current) > 0.0f ? (1.0f) : (-1.0f);

						_current += direction * moveDelta;
					}
				}
			}
		}
	}
	
	void Light::SetLightAmbient(const Eigen::Vector3f& _ambient)
	{
		ambient = _ambient;
	}

	void Light::SetLightDiffuse(const Eigen::Vector3f& _diffuse)
	{
		diffuse = _diffuse;
	}

	void Light::SetLightSpecular(const Eigen::Vector3f& _specular)
	{
		specular = _specular;
	}

	Eigen::Vector3f Light::GetLightAmbient() const
	{
		return ambient;
	}

	Eigen::Vector3f Light::GetLightDiffuse() const
	{
		return diffuse;
	}

	Eigen::Vector3f Light::GetLightSpecular() const
	{
		return specular;
	}

	void Light::SetAttenuation(const Eigen::Vector3f& attenuation)
	{
		currentAtten0 += (attenuation.x() - atten0);
		currentAtten1 += (attenuation.y() - atten1);
		currentAtten2 += (attenuation.z() - atten2);

		atten0 = attenuation.x();
		atten1 = attenuation.y();
		atten2 = attenuation.z();
	}

	Eigen::Vector3f Light::GetAttenuation() const
	{
		return Eigen::Vector3f{ atten0, atten1, atten2 };
	}

	bool Light::IsRootEnable()
	{
		return GetGameObject()->IsRootEnable();
	}

	void Light::Render(IZeldaRenderer* renderer)
	{
		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
		if (GetGameObject()->IsRootEnable())
		{
			Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };
			direction = GetGameObject()->GetTransform()->GetWorldMatrix().block<3, 3>(0, 0) * direction;
			direction.normalize();

			Eigen::Vector3f worldPosition = GetGameObject()->GetTransform()->GetWorldPosition();

			renderer->UpdateLight(lightID, ambient, diffuse, specular, shadowColor, direction, worldPosition + currentPosition, range, angle, currentAtten0, currentAtten1, currentAtten2);
			renderer->DrawLight(lightID);
		}
	}

	void Light::CreateDirectionalLight()
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };
		direction = GetGameObject()->GetTransform()->GetWorldMatrix().block<3, 3>(0, 0) * direction;
		direction.normalize();

		lightID = GraphicsManager::GetInstance().resourceManager->CreateDirectionalLight(ambient, diffuse, specular, shadowColor, direction);
	}

	void Light::CreatePointLight(float range)
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f position = GetGameObject()->GetTransform()->GetWorldPosition();

		lightID = GraphicsManager::GetInstance().resourceManager->CreatePointLight(ambient, diffuse, specular, shadowColor, position, range, atten0, atten1, atten2);
	}

	void Light::CreateSpotLight(float range, float angle)
	{
		assert(lightID == LightID::ID_NULL);

		Eigen::Vector3f direction = { 0.0f , 0.0f, 1.0f };
		direction = GetGameObject()->GetTransform()->GetWorldMatrix().block<3, 3>(0, 0) * direction;
		direction.normalize();
		Eigen::Vector3f position = GetGameObject()->GetTransform()->GetWorldPosition();

		lightID = GraphicsManager::GetInstance().resourceManager->CreateSpotLight(ambient, diffuse, specular, shadowColor, direction, position, range, angle);
	}

	void Light::ReleaseLight()
	{
		assert(lightID != LightID::ID_NULL);

		GraphicsManager::GetInstance().resourceManager->ReleaseLight(lightID);
	}

	void Light::PreSerialize(json& jsonData) const
	{

	}

	void Light::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(range);
		PREDESERIALIZE_VALUE(angle);
		PREDESERIALIZE_VECTOR3F(ambient);
		PREDESERIALIZE_VECTOR3F(diffuse);
		PREDESERIALIZE_VECTOR3F(specular);
		PREDESERIALIZE_VECTOR3F(shadowColor);
		PREDESERIALIZE_VALUE(atten0);
		PREDESERIALIZE_VALUE(atten1);
		PREDESERIALIZE_VALUE(atten2);

		PREDESERIALIZE_VALUE(atten0VibrationSpeed);
		PREDESERIALIZE_VALUE(atten0VibrationRange);
		PREDESERIALIZE_VALUE(atten1VibrationSpeed);
		PREDESERIALIZE_VALUE(atten1VibrationRange);
		PREDESERIALIZE_VALUE(atten2VibrationSpeed);
		PREDESERIALIZE_VALUE(atten2VibrationRange);
		PREDESERIALIZE_VALUE(positionVibrationSpeed);
		PREDESERIALIZE_VALUE(positionVibrationRange);
	}

	void Light::PostSerialize(json& jsonData) const
	{

	}

	void Light::PostDeserialize(const json& jsonData)
	{

	}
}