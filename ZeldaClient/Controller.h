#pragma once
#include "Component.h"

namespace PurahEngine
{
	class IGamePad;
	class Transform;
}

namespace Phyzzle
{
	class Controller final : public PurahEngine::Component
	{
	public:
		~Controller() override;

	private:
		PurahEngine::Transform* transform = nullptr;
		PurahEngine::IGamePad* gamePad = nullptr;

	private:
		static std::vector<PurahEngine::Transform*> controllerable;

		bool Keybord = false;

		float LstickX = 0.f;
		float LstickY = 0.f;
		float LstickSize = 0.f;
		float RstickSize = 0.f;
		float RstickX = 0.f;
		float RstickY = 0.f;
		float LTrigger = 0.f;
		float RTrigger = 0.f;

	private:
		float sensitivity = 0.f;
		float moveSpeed = 0.f;
		float xAngle = 0.f;

	public:
		void Awake() override;
		void Update() override;

	private:
		void Input();
		void Move();
		void Around();

	protected:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}

