#include "PurahMath.h"

namespace PurahEngine
{
	float PurahMath::CalculateRotationLength(const Eigen::Quaternionf& quatA, const Eigen::Quaternionf& quatB)
	{
		float dotProduct = quatA.dot(quatB);
		dotProduct = std::clamp(dotProduct, -1.0f, 1.0f); // dotProduct가 [-1, 1] 범위에 있음을 보장
		return 2.0f * std::acos(dotProduct);
	}

	float PurahMath::CalculateTranslationLength(const Eigen::Vector3f& translationA, const Eigen::Vector3f& translationB)
	{
		return (translationB - translationA).norm();
	}

	float PurahMath::CalculateTransformPathLength(const Eigen::Matrix4f& A, const Eigen::Matrix4f& B, int steps)
	{
		Eigen::Quaternionf quatA(A.block<3, 3>(0, 0));
		Eigen::Quaternionf quatB(B.block<3, 3>(0, 0));
		Eigen::Vector3f translationA = A.block<3, 1>(0, 3);
		Eigen::Vector3f translationB = B.block<3, 1>(0, 3);

		float totalLength = 0.0f;
		for (int i = 0; i < steps; ++i) {
			float t1 = static_cast<float>(i) / steps;
			float t2 = static_cast<float>(i + 1) / steps;

			Eigen::Quaternionf quat1 = quatA.slerp(t1, quatB);
			Eigen::Quaternionf quat2 = quatA.slerp(t2, quatB);
			Eigen::Vector3f trans1 = (1 - t1) * translationA + t1 * translationB;
			Eigen::Vector3f trans2 = (1 - t2) * translationA + t2 * translationB;

			float rotationSegmentLength = CalculateRotationLength(quat1, quat2);
			float translationSegmentLength = CalculateTranslationLength(trans1, trans2);

			totalLength += std::sqrt(rotationSegmentLength * rotationSegmentLength + translationSegmentLength * translationSegmentLength);
		}

		return totalLength;
	}

	Eigen::Matrix4f PurahMath::InterpolateTransform(const Eigen::Matrix4f& A, const Eigen::Matrix4f& B, float t)
	{
		using namespace Eigen;

		Affine3f aAffine(A);
		Vector3f aPosition{ aAffine.translation() };
		Quaternionf aRotation{ aAffine.rotation() };

		Affine3f bAffine(B);
		Vector3f bPosition{ bAffine.translation() };
		Quaternionf bRotation{ bAffine.rotation() };
		Quaternionf minusBRotation{ -bRotation.w(), -bRotation.x() , -bRotation.y() , -bRotation.z()};
		float theta0 = aRotation.dot(bRotation);
		float theta1 = aRotation.dot(minusBRotation);
		Quaternionf target = bRotation;
		if (theta0 > theta1)
		{
			target = minusBRotation;
		}

		// 두 변환 행렬의 회전 부분을 쿼터니언으로 변환합니다.
		//Eigen::Quaternionf quatA(A.block<3, 3>(0, 0));
		//Eigen::Quaternionf quatB(B.block<3, 3>(0, 0));

		// 두 쿼터니언을 Slerp로 보간합니다.
		// Eigen::Quaternionf quatInterpolated = aRotation.slerp(t, target);
		Eigen::Quaternionf quatInterpolated = aRotation.slerp(t, target);

		// 두 변환 행렬의 변환(translation) 부분을 선형 보간합니다.
		Eigen::Vector3f translationA = A.block<3, 1>(0, 3);
		Eigen::Vector3f translationB = B.block<3, 1>(0, 3);
		Eigen::Vector3f translationInterpolated = (1 - t) * translationA + t * translationB;

		// 보간된 회전과 변환을 결합하여 최종 변환 행렬을 만듭니다.
		Eigen::Matrix4f result = Eigen::Matrix4f::Identity();
		result.block<3, 3>(0, 0) = quatInterpolated.toRotationMatrix();
		result.block<3, 1>(0, 3) = translationInterpolated;

		return result;
	}

	PurahMath& PurahMath::GetInstance()
	{
		static PurahMath instance;
		return instance;

	}
	PurahMath::PurahMath()
	{
	}
	PurahMath::~PurahMath()
	{
	}
}