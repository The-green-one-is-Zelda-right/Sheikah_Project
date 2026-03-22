#pragma once
#include <unordered_map>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Phyzzle
{
	class PzObject;

	class RewindSystem
	{
	public:
		static RewindSystem& Instance()
		{
			static RewindSystem instance;
			return instance;
		}

		void SaveState(PzObject* inObject);
		void StartRewind(PzObject* inObject);
		void UpdateRewind(float inDeltaTime);
		void EndRewind();
		bool CanUseRewind(PzObject* inObject) const;
		bool IsRewinding() const { return _isRewinding; }
		void ClearHistory(PzObject* inObject);

	private:
		RewindSystem() = default;
		~RewindSystem() = default;
		RewindSystem(const RewindSystem&) = delete;
		RewindSystem& operator=(const RewindSystem&) = delete;

		struct RewindSnapshot
		{
			Eigen::Vector3f position = Eigen::Vector3f::Zero();
			Eigen::Quaternionf rotation = Eigen::Quaternionf::Identity();
			Eigen::Vector3f velocity = Eigen::Vector3f::Zero();
			Eigen::Vector3f angularVelocity = Eigen::Vector3f::Zero();
		};

		using RewindHistory = std::vector<RewindSnapshot>;

	private:
		std::unordered_map<PzObject*, RewindHistory> _objectHistories;
		const size_t MAX_HISTORY_SIZE = 300;
		const float REWIND_DURATION = 5.0f;
		const float REWIND_SAMPLE_INTERVAL = 1.0f / 60.0f;

		float _rewindTimer = 0.0f;
		float _rewindStepAccumulator = 0.0f;
		size_t _currentHistoryIndex = 0;
		bool _isRewinding = false;
		PzObject* _currentRewindObject = nullptr;
	};
}
