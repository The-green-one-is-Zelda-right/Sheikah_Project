#pragma once

#include <d3d11.h>

class IConstantBuffer
{
protected:
	virtual void SetBuffer(ID3D11DeviceContext* deviceContext) abstract;

	friend class ConstantBufferManager;
};