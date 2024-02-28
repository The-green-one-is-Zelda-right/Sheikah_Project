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

		float GetDeltaTime(std::string timeName);
		void GetEventDeltaTime(std::string timeName);
		float GetFPS();
		void SetTimeScale(float timescale);
		float GetTimeScale();

		float GetDeltaTime();

	private:
		void Initialize(std::string timename);

		void Update(std::string timeName);

	private:
		std::unordered_map<std::string, std::chrono::system_clock::time_point> timeTable;
		std::unordered_map<std::string, float> deltaTime;
		std::unordered_map<std::string, float> eventDeltaTime;

		float timeScale;
		int framePerSecond; 

		std::string timeName;
		std::string physicName;
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

