#pragma once
#include <DirectXMath.h>

namespace flt
{
	struct VSConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldInvTrans;
		DirectX::XMMATRIX ViewProj;
		DirectX::XMMATRIX worldViewProj;
	};

	struct VSBoneConstantBuffer
	{
		DirectX::XMMATRIX boneTransform[96];
	};
}

