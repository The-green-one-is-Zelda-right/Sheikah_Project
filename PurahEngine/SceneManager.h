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
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		SceneManager& operator=(const SceneManager& ref) = delete;

	public:
		// 씬에 게임오브젝트를 생성한다. 생성할 때, 이름도 같이 부여한다.
		GameObject* CreateGameObject(std::wstring objectName);
		Camera* GetMainCamera();

		void SetMainCamera(Camera * camera);

		void SetName(std::wstring name);

		void Update();

	private:
		void Initialize();

	private:
		std::wstring sceneName;

		// 씬에 생성된 오브젝트 리스트
		std::vector<GameObject*> objectList;

		// 화면을 띄울 메인 카메라
		Camera* mainCamera;
		Eigen::Vector3f cameraPosition;

		float physicsTime;

		// GameLoop에 friend선언을 해줌으로써 private설정이 되있는 Initialize()를 GameLoop에서 쓸 수 있다.
		friend GameLoop;
		friend GameObject;
		friend Transform;
	public:
		static SceneManager& GetInstance();
	};
}