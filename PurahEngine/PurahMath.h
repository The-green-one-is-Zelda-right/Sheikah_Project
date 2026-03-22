#pragma once

#include "PurahEngineAPI.h"

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace PurahEngine
{
	class PURAHENGINE_API PurahMath
	{
	public:
		// 두 쿼터니언 사이의 회전 각도 (라디안) 계산
		float CalculateRotationLength(const Eigen::Quaternionf& quatA, const Eigen::Quaternionf& quatB);
		// 두 변환 벡터 사이의 유클리드 거리 계산
		float CalculateTranslationLength(const Eigen::Vector3f& translationA, const Eigen::Vector3f& translationB);
		// 두 변환 행렬 사이의 경로 길이 계산
		float CalculateTransformPathLength(const Eigen::Matrix4f& A, const Eigen::Matrix4f& B, int steps);
		// 두 변환 행렬의 보간 행렬 계산
		Eigen::Matrix4f InterpolateTransform(const Eigen::Matrix4f& A, const Eigen::Matrix4f& B, float t);

	public:
		static PurahMath& GetInstance();

	private:
		PurahMath();
		~PurahMath();
		PurahMath(const PurahMath& ref) = delete;
		PurahMath& operator=(const PurahMath& ref) = delete;
	};
}