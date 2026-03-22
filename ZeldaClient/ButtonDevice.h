#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class ElectricalDevice;

	class ButtonDevice : public PurahEngine::Component
	{
	private:
		enum class ButtonType : int
		{
			Toggle,
			Clickable,
			Timer
		};
		
		enum class TimerType : int
		{
			Hold,		// 일정시간동안 누르면, 그 이후 계속 활성화
			Push,		// 누른 후 일정시간동안 활성화
			Release		// 눌렀다 뗀 후, 일정시간동안 활성화
		};

	public:
		void Awake() override;
		void Update() override;

		void FixedUpdate() override;

		void TargetPowerOn();
		void TargetPowerOff();

	private:
		void FastenButton(); // 버튼을 현재 위치에 고정시킨다.

		void TimerButtonUpdate();

	private:
		ButtonType buttonType;
		TimerType timerType;
		float time;
		PurahEngine::GameObject* pushableObject;
		float releasedSpeed;
		float activationDepth;
		float deactivationDepth;
		float executeDelay;
		std::vector<ElectricalDevice*> targetDeviceList;


		PurahEngine::RigidBody* pushableRigidbody;
		Eigen::Vector3f originPos;

		float powerElapsed;
		float pushElapsed;

		bool isLastButtonDown;
		bool isPowerOn;
		bool isButtonDown;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}