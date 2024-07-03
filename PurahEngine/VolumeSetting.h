#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"
#include "SoundManager.h"

namespace PurahEngine
{
	class Button;

	enum class VolumeType
	{
		UP,
		DOWN
	};

	class PURAHENGINE_API VolumeSetting : public Component
	{
	public:
		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;

	private:
		SoundType soundType;
		VolumeType volumeType;
		Button* button;
	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

