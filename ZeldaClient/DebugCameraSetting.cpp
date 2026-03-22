#include "DebugCameraSetting.h"


std::vector<PurahEngine::Camera*> Phyzzle::DebugCameraSetting::cameraList{};
int Phyzzle::DebugCameraSetting::currIndex{};


Phyzzle::DebugCameraSetting::~DebugCameraSetting()
{
	cameraList.clear();
	currIndex = 0;
}

void Phyzzle::DebugCameraSetting::Awake()
{
	PurahEngine::Camera* curr = GetGameObject()->GetComponent<PurahEngine::Camera>();

	if (!curr)
	{
		GetGameObject()->DeleteComponent(this);
	}

	cameraList.emplace_back(curr);
}

void Phyzzle::DebugCameraSetting::Update()
{
	PurahEngine::Camera* curr = GetGameObject()->GetComponent<PurahEngine::Camera>();

	if (cameraList[currIndex] != curr)
	{
		check = false;
		return;
	}

	bool downShift = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_SHIFT);
	bool downF1 = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_F1);
	bool downF2 = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_F2);

	if ((downShift && downF1) || (downShift && downF2))
	{
		check = true;
	}
}

void Phyzzle::DebugCameraSetting::LateUpdate()
{
	if (!check)
	{
		return;
	}

	bool downShift = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_SHIFT);
	bool downF1 = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_F1);
	bool downF2 = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_F2);

	if (downShift && downF1)
	{
		currIndex--;

		if (currIndex < 0)
		{
			currIndex = cameraList.size() - 1;
		}
	}
	else if (downShift && downF2)
	{
		currIndex++;

		currIndex = currIndex % cameraList.size();
	}

	cameraList[currIndex]->SetMainCamera();
}

void Phyzzle::DebugCameraSetting::OnDestroy()
{

}
