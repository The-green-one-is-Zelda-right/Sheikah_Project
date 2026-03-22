#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class AudioSource;

	class AudioTest : public Component
	{
	public:
		AudioTest();
		~AudioTest();

		void Awake();
		void OnDataLoadComplete();
		void Update();
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const Collider*);


	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		AudioSource* audioSource;
	};
}

