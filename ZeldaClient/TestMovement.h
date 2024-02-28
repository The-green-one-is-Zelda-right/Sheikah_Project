#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class GameObject;

	class TestMovement : public Component
	{
	public:
		TestMovement();
		~TestMovement();

		void Update();

		void FixedUpdate();

		XINPUT_VIBRATION vib;

		bool isVib = false;
		bool isPushed = false;
		const float vibDuration = 100.6f;
		float vibElapsedTime = 0.0f;

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}