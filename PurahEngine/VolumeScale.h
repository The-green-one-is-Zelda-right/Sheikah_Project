#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"
#include "SoundManager.h"


namespace PurahEngine
{
	class Image;

	class PURAHENGINE_API VolumeScale : public Component
	{
	public:
		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;

	private:
		SoundType soundType;
		Image* targetImage;
		std::wstring onImage;
		std::wstring offImage;
		float scaleBoundary;
		float bgmScale;
		float sfxScale;

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

