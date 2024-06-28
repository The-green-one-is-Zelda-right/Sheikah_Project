#pragma once
#include "PurahEngineAPI.h"
#include "SerializableDefine.h"

#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <eigen/Dense>
#include <map>

namespace PurahEngine
{
	class Component;
	class GameLoop;
	class GameObject;
	class Camera;
	class Transform;
	class DataManager;

	class PURAHENGINE_API SceneManager : public Serializable
	{
	private:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager& ref) = delete;
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ��������� delete�� ����Ͽ� ����� ���´�.
		SceneManager& operator=(const SceneManager& ref) = delete;

	public:
		// ���� ���ӿ�����Ʈ�� �����Ѵ�. ������ ��, �̸��� ���� �ο��Ѵ�.
		GameObject* CreateGameObject(std::wstring objectName);
		Camera* GetMainCamera();

		void SetMainCamera(Camera * camera);

		void SetName(std::wstring name);
		std::wstring GetName();

		void Update();

		void LoadScene(const std::wstring fileName);
		void LoadScene(int sceneNumber);

		void DeleteGameObject(GameObject* gameObject);

		void InitializationEvent();

		void DecommissionEvent();

		void ExcuteEventQueue();

		void ExcuteDestroyQueue();

	private:
		void LoadScene();
		void LoadSceneCompleteEvent();
		void LoadDontDestroyObject();
	protected:
		json sceneData;
	public:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		void Initialize();

	private:
		enum class RunningState
		{
			AWAKE,
			START,
			UPDATE,
			LATEUPDATE,
			DESTROY
		};

		RunningState state;

		std::wstring sceneName;
		std::wstring sceneBuffer;

		// ���� ������ ������Ʈ ����Ʈ
		std::vector<GameObject*> objectList;

		// ȭ���� ��� ���� ī�޶�
		Camera* mainCamera;
		Eigen::Vector3f cameraPosition;

		std::queue<std::pair<Component*, std::function<void(Component&)>>> eventQueue;
		std::queue<GameObject*> destroyQueue;

		float physicsTime;

		// GameLoop�� friend������ �������ν� private������ ���ִ� Initialize()�� GameLoop���� �� �� �ִ�.
		friend GameLoop;
		friend GameObject;
		friend Transform;
	public:
		static SceneManager& GetInstance();
	};
}