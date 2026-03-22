#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class Player;

	class CameraEventDevice final : public ElectricalDevice
	{
	public:
		virtual ~CameraEventDevice();
		void OnDestroy() override;

		void Awake() override;

		void Update() override;

		void PowerOn() override;
		void PowerOff() override;

	private:
		void Run();

		void EnableController() = delete;	// 키 조작을 활성화한다.
		void DisableController() = delete;	// 키 조작을 비활성화한다.

	private:
		PurahEngine::Camera* SearchCamera(PurahEngine::GameObject* obj);

	private:
		bool timeStopDuringEvent;							// 이벤트 중, 시간 정지
		bool worksOnlyOnce;									// 단 한번만 작동
		float moveSpeed;									// 카메라의 이동속도
		float startDelay;									// 카메라의 이동이 시작되기 전의 지연시간
		float powerDelay;									// 카메라의 이동이 종료된 후, targetDevice에 전원을 공급하기까지 지연시간
		float delay;										// targetDevice에 전원 공급 후, 이벤트 종료까지의 대기시간
		PurahEngine::Camera* targetCamera;					// 이벤트에 사용할 카메라
		std::vector<PurahEngine::Transform*> cameraPath;
		std::vector<ElectricalDevice*> targetDeviceList;

		Player* player = nullptr;

	private:
		bool running;		// 카메라 이벤트 진행중
		int powerCounter;
		PurahEngine::Camera* originMainCamera = nullptr; // 원래 메인 카메라 였던 카메라를 임시 저장

		float eventElapsed;		// 진행중인 이벤트의 누적 시간
		int eventLevel;			// 진행중인 이벤트의 cameraPath에서의 번호
		bool workOnce;

		int pauseLevel;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}