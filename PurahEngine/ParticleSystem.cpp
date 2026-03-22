#include "ParticleSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"

namespace PurahEngine
{
	void ParticleSystem::Awake()
	{
		genRandom = std::mt19937(randomDevice());
		positionDistribution = std::uniform_real_distribution<float>(-1.0f, 1.0f);
		lifeTimeDistribution = std::uniform_real_distribution<float>(0.0f, randomLifeTimeMax);
		vibrationDistribution = std::uniform_real_distribution<float>(-vibrationSpeed, vibrationSpeed);

		Reset();
	}

	void ParticleSystem::Start()
	{
		if (playWithStart)
		{
			isStopped = false;
		}
	}

	void ParticleSystem::Update()
	{
		assert(moveDirection != Eigen::Vector3f::Zero());

		float deltaTime = TimeController::GetInstance().GetDeltaTime();
		
		// 이동 & vibration & lifeTime 감소
		for (auto& element : elements)
		{
			element->lifeTime -= deltaTime;
			Eigen::Vector3f moveDelta = deltaTime * element->moveSpeed * element->moveDirection;
			Eigen::Matrix4f moveMatrix;
			moveMatrix <<
				1, 0, 0, moveDelta.x(),
				0, 1, 0, moveDelta.y(),
				0, 0, 1, moveDelta.z(),
				0, 0, 0, 1;


			Eigen::Vector3f vibrationDelta =
				deltaTime * Eigen::Vector3f{
				vibrationDistribution(genRandom),
				vibrationDistribution(genRandom) ,
				vibrationDistribution(genRandom)
			};

			element->vibration = element->vibration + vibrationDelta;
			if (std::abs(element->vibration.x()) < -vibrationRange) element->vibration.x() = -vibrationRange;
			if (vibrationRange < std::abs(element->vibration.x())) element->vibration.x() = vibrationRange;

			if (std::abs(element->vibration.y()) < -vibrationRange) element->vibration.y() = -vibrationRange;
			if (vibrationRange < std::abs(element->vibration.y())) element->vibration.y() = vibrationRange;

			if (std::abs(element->vibration.z()) < -vibrationRange) element->vibration.z() = -vibrationRange;
			if (vibrationRange < std::abs(element->vibration.z())) element->vibration.z() = vibrationRange;

			// vibration에서 moveDirection방향의 성분 제거
			element->vibration = -(element->vibration.dot(moveDirection) * moveDirection) + element->vibration;

			element->translation = moveMatrix * element->translation;
		}

		// lifeTime이 0보다 작거나 같아진 element 제거
		for (auto iter = elements.begin(); iter != elements.end();)
		{
			if ((*iter)->lifeTime <= 0.0f)
			{
				Release(std::move(*iter));
				iter = elements.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		generationElapsed += deltaTime;

		while (generationElapsed >= generationCycle)
		{
			generationElapsed -= generationCycle;
			Generate();
		}
	}

	void ParticleSystem::Render(IZeldaRenderer* renderer)
	{
		Eigen::Matrix4f worldMatrix = GetGameObject()->GetTransform()->GetWorldMatrix();

		// 게임오브젝트가 활성화 되어 있는 경우에만 작동한다.
		if (GetGameObject()->IsRootEnable())
		{
			TextureID textureID = TextureID::ID_NULL;
			if (textureName != L"")
			{
				textureID = GetTextureID(textureName);
			}

			for (auto& element : elements)
			{
				Eigen::Matrix4f elementMatrix = element->translation * worldMatrix * element->scaling;
				
				::Color color;
				color.r = alphaTextureColor.x();
				color.g = alphaTextureColor.y();
				color.b = alphaTextureColor.z();
				color.a = alphaTextureColor.w();

				if (useChangeColor)
				{
					Eigen::Vector3f pos = { element->translation(0, 3), element->translation(1, 3), element->translation(2, 3) };
					Eigen::Vector3f dir = element->moveDirection.normalized();

					float dirPos = dir.dot(pos);
					Eigen::Vector3f verPos = pos - (dir * dirPos);
					float verDist = verPos.size();

					color.r = color.r + (changedColorInMove.x() * dirPos) + (changedColorInOther.x() * verDist);
					color.g = color.g + (changedColorInMove.y() * dirPos) + (changedColorInOther.y() * verDist);
					color.b = color.b + (changedColorInMove.z() * dirPos) + (changedColorInOther.z() * verDist);
					color.a = color.a + (changedColorInMove.w() * dirPos) + (changedColorInOther.w() * verDist);

					if (color.r > 1.0f) color.r = 1.0f;
					if (color.g > 1.0f) color.g = 1.0f;
					if (color.b > 1.0f) color.b = 1.0f;
					if (color.a > 1.0f) color.a = 1.0f;
				}
				
				Eigen::Matrix4f vibrationMatrix;
				vibrationMatrix <<
					1, 0, 0, element->vibration.x(),
					0, 1, 0, element->vibration.y(),
					0, 0, 1, element->vibration.z(),
					0, 0, 0, 1;

				renderer->DrawBillBoard(vibrationMatrix * elementMatrix, textureID, 0.0f, true, useAlphaTexture, color);
			}
		}
	}

	void ParticleSystem::Play()
	{
		generationElapsed = 0.0f;
		isStopped = false;
	}

	void ParticleSystem::Reset()
	{
		generationElapsed = 0.0f;
		isStopped = true;

		elements.clear();
	}

	void ParticleSystem::StopGenerate()
	{
		isStopped = true;
	}

	void ParticleSystem::Generate()
	{
		if (isStopped)
		{
			return;
		}

		std::unique_ptr<Element> element = Acquire();

		switch (generatorType)
		{
			case ParticleSystem::GeneratorType::Point:
			{
				element->translation = Eigen::Matrix4f::Identity();
				break;
			}
			case ParticleSystem::GeneratorType::Circle:
			{
				float x, y, z;
				do
				{
					x = generatorRadius * positionDistribution(genRandom);
					y = 0.0f;
					z = generatorRadius * positionDistribution(genRandom);
				} while (x * x + y * y + z * z > generatorRadius * generatorRadius);
				element->translation <<
					1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1;
				break;
			}
			case ParticleSystem::GeneratorType::Sphere:
			{
				float x, y, z;
				do
				{
					x = generatorRadius * positionDistribution(genRandom);
					y = generatorRadius * positionDistribution(genRandom);
					z = generatorRadius * positionDistribution(genRandom);
				} while (x * x + y * y + z * z <= generatorRadius * generatorRadius);
				element->translation <<
					1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1;
				break;
			}
			case ParticleSystem::GeneratorType::Square:
			{
				float x, y, z;
				x = generatorSizeX * positionDistribution(genRandom) / 2.0f;
				y = 0.0f;
				z = generatorSizeZ * positionDistribution(genRandom) / 2.0f;

				element->translation <<
					1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1;
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}

		switch (elementType)
		{
			case PurahEngine::ParticleSystem::ElementType::Move:
			{
				element->moveDirection = moveDirection;
				break;
			}
			case PurahEngine::ParticleSystem::ElementType::Spread:
			{
				assert(0);
				break;
			}
			case PurahEngine::ParticleSystem::ElementType::SpreadInOneDirection:
			{
				assert(0);
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}

		element->scaling <<
			textureScale.x(), 0, 0, 0,
			0, textureScale.y(), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		element->vibration = Eigen::Vector3f::Zero();
		element->moveSpeed = moveSpeed;
		element->lifeTime = lifeTime + lifeTimeDistribution(genRandom);

		elements.push_back(std::move(element));
	}

	std::unique_ptr<ParticleSystem::Element> ParticleSystem::Acquire()
	{
		if (pool.empty())
		{
			return std::make_unique<Element>();
		}
		else
		{
			std::unique_ptr<Element> obj = std::move(pool.back());
			pool.pop_back();
			return obj;
		}
	}

	void ParticleSystem::Release(std::unique_ptr<Element> obj)
	{
		pool.push_back(std::move(obj));
	}

	void ParticleSystem::PreSerialize(json& jsonData) const
	{
	}

	void ParticleSystem::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(generatorType);
		PREDESERIALIZE_VALUE(elementType);
		PREDESERIALIZE_WSTRING(textureName);

		PREDESERIALIZE_VECTOR2F(textureScale);
		PREDESERIALIZE_VALUE(useAlphaTexture);
		PREDESERIALIZE_VECTOR4F(alphaTextureColor);

		PREDESERIALIZE_VALUE(playWithStart);

		PREDESERIALIZE_VALUE(generatorRadius);
		PREDESERIALIZE_VALUE(generatorSizeX);
		PREDESERIALIZE_VALUE(generatorSizeY);
		PREDESERIALIZE_VALUE(generatorSizeZ);
		PREDESERIALIZE_VALUE(generationCycle);
		
		PREDESERIALIZE_VECTOR3F(moveDirection);
		PREDESERIALIZE_VALUE(spread);
		PREDESERIALIZE_VALUE(vibrationSpeed);
		PREDESERIALIZE_VALUE(vibrationRange);

		PREDESERIALIZE_VALUE(moveSpeed);
		PREDESERIALIZE_VALUE(lifeTime);
		PREDESERIALIZE_VALUE(randomLifeTimeMax);

		PREDESERIALIZE_VALUE(useChangeColor);
		PREDESERIALIZE_VECTOR4F(changedColorInMove);
		PREDESERIALIZE_VECTOR4F(changedColorInOther);
	}

	void ParticleSystem::PostSerialize(json& jsonData) const
	{
	}

	void ParticleSystem::PostDeserialize(const json& jsonData)
	{
	}
}