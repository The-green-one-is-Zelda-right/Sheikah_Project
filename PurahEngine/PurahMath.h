#pragma once

#include "PurahEngineAPI.h"

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace PurahEngine
{
	class PURAHENGINE_API PurahMath
	{
	public:
		// �� ���ʹϾ� ������ ȸ�� ���� (����) ���
		float CalculateRotationLength(const Eigen::Quaternionf& quatA, const Eigen::Quaternionf& quatB);
		// �� ��ȯ ���� ������ ��Ŭ���� �Ÿ� ���
		float CalculateTranslationLength(const Eigen::Vector3f& translationA, const Eigen::Vector3f& translationB);
		// �� ��ȯ ��� ������ ��� ���� ���
		float CalculateTransformPathLength(const Eigen::Matrix4f& A, const Eigen::Matrix4f& B, int steps);
		// �� ��ȯ ����� ���� ��� ���
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