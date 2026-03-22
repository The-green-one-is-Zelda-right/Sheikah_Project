#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class ObjectGenerator final : public ElectricalDevice
	{
	public:
		enum class GeneratorType : int
		{
			Always = 0,
			MaintainingPower = 1,
			PowerOn = 2
		};

	public:
		void PowerOn() override;
		void PowerOff() override;

		void Awake() override;
		void Update() override;

	private:
		void GenerateObject(PurahEngine::GameObject* object);

	private:
		GeneratorType generatorType;
		float generateDelay;									// 반복 생성 지연시간(GeneratorType::PowerOn에서는 적용되지 않음)
		std::vector<PurahEngine::GameObject*> targetObjects;

	private:
		float delayElapsed;
		int powerCount;

	public:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;


	};
}