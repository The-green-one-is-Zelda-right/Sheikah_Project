#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{

	class Test : public Component
	{
	public:
		Test();
		~Test();

		void Awake();
		void Start();
		void Update();

	private:
		int a = 1;
		int b = 2;
		int c = 3;

		Transform* trans;

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

