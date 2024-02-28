#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class Test2 : public Component
	{
	public:
		Test2();
		~Test2();

		void Awake();
		void Start();
		void Update();
		void FixedUpdate();

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

