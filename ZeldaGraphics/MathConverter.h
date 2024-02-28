#pragma once

#include <DirectXMath.h>
#include <Eigen\Dense>

class MathConverter
{
public:
	static DirectX::XMMATRIX EigenMatrixToXMMatrix(const Eigen::Matrix4f& matrix);
};