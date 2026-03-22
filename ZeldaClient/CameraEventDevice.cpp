#include "CameraEventDevice.h"

#include "Player.h"

namespace Phyzzle
{
	CameraEventDevice::~CameraEventDevice()
	{
		if (running && timeStopDuringEvent)
		{
			PurahEngine::TimeController::GetInstance().ResumeAll();
		}
	}

	void CameraEventDevice::OnDestroy()
	{
		if (running && timeStopDuringEvent)
		{
			if (player != nullptr)
			{
				player->SetStopUpdate(false);
			}
		}
	}

	void CameraEventDevice::Awake()
	{
		running = false;
		powerCounter = 0;
		if (player != nullptr)
		{
			originMainCamera = SearchCamera(player->GetGameObject());
		}
		else
		{
			originMainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
		}
		workOnce = false;
		pauseLevel = 0;
	}

	void CameraEventDevice::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime(pauseLevel);
		eventElapsed += deltaTime;

		bool loop = true;

		if (running)
		{
			while (loop)
			{
				float moveElapsed = moveSpeed * eventElapsed;

				if (eventLevel == 0)
				{
					if (startDelay < eventElapsed)
					{
						eventElapsed -= startDelay;
						eventLevel += 1;
					}
					else
					{
						loop = false;
					}
				}
				else if (eventLevel < cameraPath.size())
				{
					PurahEngine::Transform* trs1 = cameraPath[eventLevel - 1];
					PurahEngine::Transform* trs2 = cameraPath[eventLevel];
					Eigen::Matrix4f m1 = trs1->GetWorldMatrix();
					Eigen::Matrix4f m2 = trs2->GetWorldMatrix();

					float currentLength = PurahEngine::PurahMath::GetInstance().CalculateTransformPathLength(m1, m2, 30);
					
					if (currentLength < moveElapsed)
					{
						eventElapsed = (moveElapsed - currentLength) / moveSpeed;
						eventLevel += 1;
					}
					else
					{
						float t = moveElapsed / currentLength;
						t = std::clamp(t, 0.f, 1.f);

						Eigen::Matrix4f interpolatedmatrix = PurahEngine::PurahMath::GetInstance().InterpolateTransform(m1, m2, t);

						targetCamera->GetGameObject()->GetTransform()->SetWorldMatrix(interpolatedmatrix);
						loop = false;
					}
				}
				else if (eventLevel == cameraPath.size())
				{
					if (powerDelay < eventElapsed)
					{
						for (int i = 0; i < targetDeviceList.size(); i++)
						{
							targetDeviceList[i]->PowerOn();
						}

						eventElapsed -= powerDelay;
						eventLevel += 1;
						pauseLevel = 0;

						if (timeStopDuringEvent)
						{
							PurahEngine::TimeController::GetInstance().ResumeAll();
							if (player != nullptr)
							{
								player->SetStopUpdate(false);
							}
						}
					}
					else
					{
						loop = false;
					}
				}
				else if (eventLevel == cameraPath.size() + 1)
				{
					if (delay < eventElapsed)
					{
						// 현재 카메라가 TargetCamera인 경우에만 원래 카메라로 변경한다.
						if (PurahEngine::SceneManager::GetInstance().GetMainCamera() == targetCamera)
						{
							originMainCamera->SetMainCamera();
						}
						running = false;
						eventLevel += 1;

						if (powerCounter < 1)
						{
							for (int i = 0; i < targetDeviceList.size(); i++)
							{
								targetDeviceList[i]->PowerOff();
							}
						}
					}
					else
					{
						loop = false;
					}
				}
				else
				{
					loop = false;
				}
			}
		}
	}

	void CameraEventDevice::PowerOn()
	{
		powerCounter += 1;

		if (powerCounter >= 1)
		{
			Run();
		}
	}
	void CameraEventDevice::PowerOff()
	{
		powerCounter -= 1;

		if (powerCounter < 1 && running == false)
		{
			for (int i = 0; i < targetDeviceList.size(); i++)
			{
				targetDeviceList[i]->PowerOff();
			}
		}
	}

	void CameraEventDevice::Run()
	{
		if (running)
		{
			return;
		}

		if ((worksOnlyOnce && workOnce) == false)
		{
			running = true;
			if (timeStopDuringEvent)
			{
				pauseLevel = PurahEngine::TimeController::GetInstance().PauseAll();

				if (player != nullptr)
				{
					player->SetStopUpdate(true);
				}
			}

			eventElapsed = 0.0f;
			eventLevel = 0;
			workOnce = true;

			if (cameraPath.size() != 0)
			{
				targetCamera->GetGameObject()->GetTransform()->SetWorldMatrix(cameraPath[0]->GetWorldMatrix());
			}

			// 메인 카메라 교체
			targetCamera->SetMainCamera();
		}
		else
		{
			for (int i = 0; i < targetDeviceList.size(); i++)
			{
				targetDeviceList[i]->PowerOn();
			}
		}
	}

	PurahEngine::Camera* CameraEventDevice::SearchCamera(PurahEngine::GameObject* obj)
	{
		PurahEngine::Camera* camera = obj->GetComponent<PurahEngine::Camera>();
		
		if (camera != nullptr)
		{
			return camera;
		}

		PurahEngine::Transform* trs = obj->GetTransform();
		std::vector<PurahEngine::Transform*> children = trs->GetChildren();

		for (PurahEngine::Transform* child : children)
		{
			PurahEngine::GameObject* cObj = child->GetGameObject();
			camera = SearchCamera(cObj);
			if (camera != nullptr)
			{
				return camera;
			}
		}

		return nullptr;
	}

	void CameraEventDevice::PreSerialize(json& jsonData) const
	{
	}

	void CameraEventDevice::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(timeStopDuringEvent);
		PREDESERIALIZE_VALUE(worksOnlyOnce);
		PREDESERIALIZE_VALUE(moveSpeed);
		PREDESERIALIZE_VALUE(startDelay);
		PREDESERIALIZE_VALUE(powerDelay);
		PREDESERIALIZE_VALUE(delay);
	}

	void CameraEventDevice::PostSerialize(json& jsonData) const
	{
	}

	void CameraEventDevice::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(targetCamera);
		POSTDESERIALIZE_VECTOR_PTR(cameraPath);
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);

		if (jsonData.contains("__ID__player"))
		{
			player = static_cast<decltype(player)>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData["__ID__player"]));
		}
		else
		{
			player = nullptr;
		}
	}
}