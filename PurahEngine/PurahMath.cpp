#include "PurahMath.h"

namespace PurahEngine
{
	float PurahMath::CalculateRotationLength(const Eigen::Quaternionf& quatA, const Eigen::Quaternionf& quatB)
	{
		float dotProduct = quatA.dot(quatB);
		dotProduct = std::clamp(dotProduct, -1.0f, 1.0f); // dotProduct�� [-1, 1] ������ ������ ����
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

		// �� ��ȯ ����� ȸ�� �κ��� ���ʹϾ����� ��ȯ�մϴ�.
		//Eigen::Quaternionf quatA(A.block<3, 3>(0, 0));
		//Eigen::Quaternionf quatB(B.block<3, 3>(0, 0));

		// �� ���ʹϾ��� Slerp�� �����մϴ�.
		// Eigen::Quaternionf quatInterpolated = aRotation.slerp(t, target);
		Eigen::Quaternionf quatInterpolated = aRotation.slerp(t, target);

		// �� ��ȯ ����� ��ȯ(translation) �κ��� ���� �����մϴ�.
		Eigen::Vector3f translationA = A.block<3, 1>(0, 3);
		Eigen::Vector3f translationB = B.block<3, 1>(0, 3);
		Eigen::Vector3f translationInterpolated = (1 - t) * translationA + t * translationB;

		// ������ ȸ���� ��ȯ�� �����Ͽ� ���� ��ȯ ����� ����ϴ�.
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