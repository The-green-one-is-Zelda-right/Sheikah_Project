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
			Sphere = 2		// �� ������� ����
		};

		enum class ElementType
		{
			Move = 0,					// �� �������� �̵�
			Spread = 1,					// ������� ����
			SpreadInOneDirection = 2	// �� �������� ����
		};

	public:
		void Awake() override;
		void Update() override;

		// Renderer��(��) ���� ��ӵ�
		void Render(IZeldaRenderer* renderer) override;

		void Reset();
		void StopGenerate();

	private:
		GeneratorType generatorType;
		ElementType elementType;
		std::wstring textureName;
		Eigen::Vector2f textureScale;
		bool useAlphaTexture;
		Eigen::Vector4f alphaTextureColor;

		float generatorRadius;			// ���� ����(������)
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
		std::list<Element> elements;

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