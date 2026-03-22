#pragma once
#include <Eigen/Dense>
#include <vector>

class BezierCurve
{
public:
	unsigned long binomialCoefficient(unsigned int n, unsigned int k) 
	{
		if (k > n) 
			return 0;

		if (k == 0 || k == n) 
			return 1;

		unsigned long res = 1;

		if (k > n - k) 
			k = n - k;

		for (unsigned int i = 0; i < k; ++i) 
		{
			res *= (n - i);
			res /= (i + 1);
		}

		return res;
	}

	Eigen::Vector3f BezierInterpolate(const std::vector<Eigen::Vector3f>& controlPoints, float t) 
	{
		unsigned int n = controlPoints.size() - 1;

		Eigen::Vector3f P = Eigen::Vector3f::Zero();

		for (unsigned int i = 0; i <= n; ++i) 
		{
			unsigned long binom = binomialCoefficient(n, i);
			float coeff = binom * std::pow(1 - t, n - i) * std::pow(t, i);
			P += coeff * controlPoints[i];
		}

		return P;
	}
};