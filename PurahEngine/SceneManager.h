#pragma once
#include "PurahEngineAPI.h"

#include <string>
#include <vector>
#include <eigen/Dense>

namespace PurahEngine
{
	class GameLoop;
	class GameObject;
	class Camera;
	class Transform;

	class PURAHENGINE_API SceneManager
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

		void Update();

	private:
		void Initialize();

	private:
		std::wstring sceneName;

		// ���� ������ ������Ʈ ����Ʈ
		std::vector<GameObject*> objectList;

		// ȭ���� ��� ���� ī�޶�
		Camera* mainCamera;
		Eigen::Vector3f cameraPosition;

		float physicsTime;

		// GameLoop�� friend������ �������ν� private������ ���ִ� Initialize()�� GameLoop���� �� �� �ִ�.
		friend GameLoop;
		friend GameObject;
		friend Transform;
	public:
		static SceneManager& GetInstance();
	};
}