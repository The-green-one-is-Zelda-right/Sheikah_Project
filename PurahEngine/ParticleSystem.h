#pragma once

#include "Renderer.h"
#include <list>
#include <random>

namespace PurahEngine
{
	class PURAHENGINE_API ParticleSystem final : public Renderer
	{
	public:
		enum class GeneratorType
		{
			Point = 0,		// 한 점에서 생성
			Circle = 1,		// 원 모양으로 생성
			Sphere = 2,		// 구 모양으로 생성
			Square = 3		// 사각형 모양으로 생성
		};

		enum class ElementType
		{
			Move = 0,					// 한 방향으로 이동
			Spread = 1,					// 사방으로 퍼짐
			SpreadInOneDirection = 2	// 한 방향으로 퍼짐
		};

	public:
		void Awake() override;
		void Start() override;

		void Update() override;

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void Play();
		void Reset();
		void StopGenerate();

	private:
		GeneratorType generatorType;
		ElementType elementType;
		std::wstring textureName;
		Eigen::Vector2f textureScale;
		bool useAlphaTexture;
		Eigen::Vector4f alphaTextureColor;

		bool playWithStart;

		float generatorRadius;			// 생성 범위(반지름) - Circle, Sphere
		float generatorSizeX;			// 생성 범위 - Square
		float generatorSizeY;			// 생성 범위 - Square
		float generatorSizeZ;			// 생성 범위 - Square
		float generationCycle;			// 생성 주기

		Eigen::Vector3f moveDirection;	// 이동(확산) 방향
		float spread;					// 확산 크기 (이동방향에 수직인 랜덤한 방향으로 이동하는 크기)
		float vibrationSpeed;			// 진동 속도
		float vibrationRange;			// 진동 크기 (이동방향에 수직인 랜덤한 방향으로 이동, vibrationRange를 벗어나지 않음)

		float moveSpeed;				// 이동 속도
		float lifeTime;					// 생존 기간
		float randomLifeTimeMax;		// 추가적인 랜덤 생존 기간의 최대치

		bool useChangeColor;
		Eigen::Vector4f changedColorInMove;		// 이동 방향으로 이동하면서 위치에 따른 색깔 변경
		Eigen::Vector4f changedColorInOther;	// 이동 방향이 아닌 방향에서 위치에 따른 색깔 변경

	private:
		void Generate();

	private:
		struct Element
		{
			Eigen::Matrix4f scaling;
			Eigen::Matrix4f translation;
			Eigen::Vector3f vibration;
			Eigen::Vector3f moveDirection;
			float moveSpeed;
			float lifeTime;
		};

	private:
		std::vector<std::unique_ptr<Element>> pool;

		std::unique_ptr<ParticleSystem::Element> Acquire();
		void Release(std::unique_ptr<Element> obj);

	private:
		std::list<std::unique_ptr<Element>> elements;

		bool isStopped;
		float generationElapsed;

		std::random_device randomDevice;
		std::mt19937 genRandom;
		std::uniform_real_distribution<float> positionDistribution;
		std::uniform_real_distribution<float> lifeTimeDistribution;
		std::uniform_real_distribution<float> vibrationDistribution;

	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}