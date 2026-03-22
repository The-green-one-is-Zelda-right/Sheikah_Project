#pragma once

#include "ElectricalDevice.h"

namespace Phyzzle
{
	class LinearMotor final : public ElectricalDevice
	{
	public:
		void PowerOn() override;
		void PowerOff() override;

		void Awake();
		void Start();
		void Update();

	private:
		void StateUpdate();

	private:
		Eigen::Vector3f moveDirection;
		float pushPower;
		float pullPower;

		int pushPowerType;
		int pullPowerType;

		float pushTime;
		float pullTime;

	private:
		enum class State
		{
			Push,
			Pull,
			Wait,
			Sleep
		};

		State state;
		bool powerOnWithStart;

		PurahEngine::RigidBody* rigid = nullptr;

	private:
		int powerCounter;
		float elapsedTime;

	public:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}