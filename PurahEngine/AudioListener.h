#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class GameObject;

	class AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener();

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		Transform* listenerTransform;
	};
}

