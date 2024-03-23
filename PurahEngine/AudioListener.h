#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"

#include "Transform.h"


namespace PurahEngine
{
	class Transform;
	class GameObject;
	class SoundManager;

	class PURAHENGINE_API AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener();

	public:
		void Initialize();
		void Update();
	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		Transform* listenerTransform;
	};
}