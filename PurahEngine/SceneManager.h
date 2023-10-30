#pragma once

#include <string>
#include <vector>

namespace PurahEngine
{
	class GameObject;
	class Camera;

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
		friend class GameLoop;
	public:
		static SceneManager& GetInstance();
	};
}