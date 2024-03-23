#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"

#include "Transform.h"
#include "SoundManager.h"

namespace PurahEngine
{
	class Transform;
	class GameObject;
	class SoundManager;

	class PURAHENGINE_API AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();

	public:
		void Awake();
		void Initialize();
		void OnDataLoadComplete();
		void Update();

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		Transform* soundTransform;
		std::wstring soundName;
	};
}

