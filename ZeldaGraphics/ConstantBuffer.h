#pragma once

#include "IConstantBuffer.h"
#include "ZeldaGraphicsDefine.h"

template<typename bufferType, ShaderType shaderType>
class ConstantBuffer : public IConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* device) :
		dxBuffer(nullptr)
	{
		assert((sizeof(bufferType) % 16) == 0);

		D3D11_BUFFER_DESC matrixBufferDesc;

		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(bufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HRESULT result;
		result = device->CreateBuffer(&matrixBufferDesc, nullptr, &dxBuffer);
		if (FAILED(result))
		{
			MessageBox(0, L"Failed to Create Buffer", L"ConstantBuffer Error", MB_OK);
		}
	}

	void SetData(const bufferType& data)
	{
		this->data = data;
	}

	void SetBuffer(ID3D11DeviceContext* deviceContext, unsigned int registerNum) override
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		HRESULT result;
		result = deviceContext->Map(dxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			MessageBox(0, L"Failed to Set Buffer", L"ConstantBuffer Error", MB_OK);
			return;
		}

		memcpy(mappedResource.pData, &data, sizeof(bufferType));

		deviceContext->Unmap(dxBuffer, 0);

		switch (shaderType)
		{
			case ShaderType::VertexShader:
			{
				deviceContext->VSSetConstantBuffers(registerNum, 1, &dxBuffer);
				break;
			}
				
			case ShaderType::PixelShader:
			{
				deviceContext->PSSetConstantBuffers(registerNum, 1, &dxBuffer);
				break;
			}

			default:
			{
				assert(0);
				break;
			}
		}
	}

private:
	ID3D11Buffer* dxBuffer;
	bufferType data;

};

