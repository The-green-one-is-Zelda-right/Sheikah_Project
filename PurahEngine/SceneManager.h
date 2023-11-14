#pragma once

#include <string>
#include <vector>

namespace PurahEngine
{
	class GameLoop;
	class GameObject;
	class Camera;
	class Transform;

	class SceneManager
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

	private:
		void Initialize();

	private:
		// ���� ������ ������Ʈ ����Ʈ
		std::vector<GameObject*> objectList;

		// ȭ���� ��� ���� ī�޶�
		Camera* mainCamera;

		// GameLoop�� friend������ �������ν� private������ ���ִ� Initialize()�� GameLoop���� �� �� �ִ�.
		friend GameLoop;
		friend GameObject;
		friend Transform;
	public:
		static SceneManager& GetInstance();
	};
}