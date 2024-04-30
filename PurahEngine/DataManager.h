#pragma once

#include "SerializableDefine.h"

namespace PurahEngine
{
	class GameObject;
	class SceneManager;

	class DataManager : public Serializable
	{
	public:
		void DontDestroyOnLoad(GameObject* gameObject);

	public:
		std::vector<GameObject*> dontDestroyObjectList;

	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		DataManager();
		~DataManager();
		DataManager(const DataManager& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		DataManager& operator=(const DataManager& ref) = delete;

	public:
		static DataManager& GetInstance();
	};
}

