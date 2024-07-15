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
			Point = 0,		// �� ������ ����
			Circle = 1,		// �� ������� ����
			Sphere = 2,		// �� ������� ����
			Square = 3		// �簢�� ������� ����
		};

		enum class ElementType
		{
			Move = 0,					// �� �������� �̵�
			Spread = 1,					// ������� ����
			SpreadInOneDirection = 2	// �� �������� ����
		};

	public:
		void Awake() override;
		void Start() override;

		void Update() override;

		// Renderer��(��) ���� ��ӵ�
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

		float generatorRadius;			// ���� ����(������) - Circle, Sphere
		float generatorSizeX;			// ���� ���� - Square
		float generatorSizeY;			// ���� ���� - Square
		float generatorSizeZ;			// ���� ���� - Square
		float generationCycle;			// ���� �ֱ�

		Eigen::Vector3f moveDirection;	// �̵�(Ȯ��) ����
		float spread;					// Ȯ�� ũ�� (�̵����⿡ ������ ������ �������� �̵��ϴ� ũ��)
		float vibrationSpeed;			// ���� �ӵ�
		float vibrationRange;			// ���� ũ�� (�̵����⿡ ������ ������ �������� �̵�, vibrationRange�� ����� ����)

		float moveSpeed;				// �̵� �ӵ�
		float lifeTime;					// ���� �Ⱓ
		float randomLifeTimeMax;		// �߰����� ���� ���� �Ⱓ�� �ִ�ġ

		bool useChangeColor;
		Eigen::Vector4f changedColorInMove;		// �̵� �������� �̵��ϸ鼭 ��ġ�� ���� ���� ����
		Eigen::Vector4f changedColorInOther;	// �̵� ������ �ƴ� ���⿡�� ��ġ�� ���� ���� ����

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
		// Component��(��) ���� ��ӵ�
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}