#pragma once

#include "ElectricalDevice.h"

namespace Phyzzle
{
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
			Hold,		// �����ð����� ������, �� ���� ��� Ȱ��ȭ
			Push,		// ���� �� �����ð����� Ȱ��ȭ
			Release		// ������ �� ��, �����ð����� Ȱ��ȭ
		};

	public:
		void Awake() override;
		void Update() override;

		void FixedUpdate() override;

		void TargetPowerOn();
		void TargetPowerOff();

	private:
		void FastenButton(); // ��ư�� ���� ��ġ�� ������Ų��.

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