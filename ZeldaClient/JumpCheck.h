#pragma once
#include "PurahEngine.h"

namespace Phyzzle
{
	class JumpCheck : public PurahEngine::Component
	{
	public:
		// Component��(��) ���� ��ӵ�
		void PreSerialize(json& jsonData) const override;

		void PreDeserialize(const json& jsonData) override;

		void PostSerialize(json& jsonData) const override;

		void PostDeserialize(const json& jsonData) override;
	};
}
