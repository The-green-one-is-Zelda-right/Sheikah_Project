#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class Burnable final : public PurahEngine::Component
	{
	public:
		void Awake() override;
		void Update() override;

		// Ʈ���Ű� �浹���� �� ȣ��
		void OnTriggerEnter(const PurahEngine::Collider*) override;
		// Ʈ���Ű� �浹�� ����� �� ȣ��
		void OnTriggerExit(const PurahEngine::Collider*) override;

	private:
		float burnTime; // ��Ÿ�µ� �ɸ��� �ð�
		PurahEngine::GameObject* burnableObject; // Ÿ�� ������Ʈ
		PurahEngine::GameObject* effectObject; // ����Ʈ�� ���� �Ѵµ� ����� ������Ʈ
		float destoryDelay; // ������Ʈ ���� �����ð�

	private:
		std::vector<const PurahEngine::Collider*> fireList;
		float burnTimeElapsed;
		bool waitDestroy;
		float destroyElapsed;

	public:
		// Component��(��) ���� ��ӵ�
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}