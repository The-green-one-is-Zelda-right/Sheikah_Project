#pragma once

#include "SerializableDefine.h"
#include <map>

namespace PurahEngine
{
	class GameObject;
	class SceneManager;

	class DataManager : public Serializable
	{
	public:
		void DontDestroyOnLoad(GameObject* gameObject);
		void ClearDontDestroy();
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
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		DataManager& operator=(const DataManager& ref) = delete;

	public:
		static DataManager& GetInstance();
	};
}

