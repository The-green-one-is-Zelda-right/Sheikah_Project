#pragma once
#include "PurahEngineAPI.h"

#include <chrono>
#include <unordered_map>

namespace PurahEngine
{
	class GameLoop;
	class SceneManager;

	class PURAHENGINE_API TimeController
	{
	public:
		static const float TIMESCALE_DEFAULT;
		static const int FPS_DEFAULT;
		static const float MAX_DELTA;

		float GetDeltaTime(std::string timeName);
		float GetDeltaTime(std::string timeName, int pauseLevel);
		float GetEventDeltaTime(std::string timeName);
		float GetFPS();
		void SetTimeScale(float timescale);
		float GetTimeScale();

		float GetDeltaTime();
		float GetDeltaTime(int pauseLevel);

		/// <summary>
		/// 게임을 일시정지합니다.
		/// </summary>
		/// <returns> Pause Level </returns>
		int PauseAll();
		void ResumeAll();

	private:
		void Initialize(std::string timename);

		void Update(std::string timeName);
		void UpdateIgnoreDelta(std::string timeName);

		void LockAll();
		void UnlockAll();

		void MoveWindow();

	private:
		std::unordered_map<std::string, std::chrono::system_clock::time_point> timeTable;
		std::unordered_map<std::string, float> deltaTime;
		std::unordered_map<std::string, float> eventDeltaTime;

		float timeScale;
		int framePerSecond; 
		bool isRunning;
		std::chrono::system_clock::time_point lockTime;

		std::string timeName;
		std::string physicName;

	private:
		int pauseCount;

	public:
		static TimeController& GetInstance();

	private:
		TimeController();
		~TimeController();
		TimeController(const TimeController& ref);
		TimeController& operator=(const TimeController& ref) = delete;

		friend GameLoop;
		friend SceneManager;
	};
}

