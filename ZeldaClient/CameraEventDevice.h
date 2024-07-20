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

		void EnableController() = delete;	// Ű ������ Ȱ��ȭ�Ѵ�.
		void DisableController() = delete;	// Ű ������ ��Ȱ��ȭ�Ѵ�.

	private:
		bool worksOnlyOnce;									// �� �ѹ��� �۵�
		float moveSpeed;									// ī�޶��� �̵��ӵ�
		float startDelay;									// ī�޶��� �̵��� ���۵Ǳ� ���� �����ð�
		float powerDelay;									// ī�޶��� �̵��� ����� ��, targetDevice�� ������ �����ϱ���� �����ð�
		float delay;										// targetDevice�� ���� ���� ��, �̺�Ʈ ��������� ���ð�
		PurahEngine::Camera* targetCamera;					// �̺�Ʈ�� ����� ī�޶�
		std::vector<PurahEngine::Transform*> cameraPath;
		std::vector<ElectricalDevice*> targetDeviceList;

		Player* player = nullptr;

	private:
		bool running;		// ī�޶� �̺�Ʈ ������
		int powerCounter;
		PurahEngine::Camera* originMainCamera; // ���� ���� ī�޶� ���� ī�޶� �ӽ� ����

		float eventElapsed;		// �������� �̺�Ʈ�� ���� �ð�
		int eventLevel;			// �������� �̺�Ʈ�� cameraPath������ ��ȣ
		bool workOnce;

		int pauseLevel;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}