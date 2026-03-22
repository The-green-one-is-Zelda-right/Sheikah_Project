#pragma once
#include <d3d11.h>
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"

namespace flt
{
	class ResourceMgr;

	struct DX11VertexShader
	{
		DX11VertexShader() : pVertexShader(nullptr), pInputLayout(nullptr), pConstantBuffer(nullptr), pBoneConstantBuffer(nullptr) {}

		void Release()
		{
			if (pVertexShader)
			{
				pVertexShader->Release();
			}

			if (pInputLayout)
			{
				pInputLayout->Release();
			}

			if (pConstantBuffer)
			{
				pConstantBuffer->Release();
			}

			if (pBoneConstantBuffer)
			{
				pBoneConstantBuffer->Release();
			}
		}

		ID3D11VertexShader* pVertexShader;
		ID3D11InputLayout* pInputLayout;
		ID3D11Buffer* pConstantBuffer;
		ID3D11Buffer* pBoneConstantBuffer;
	};

	template struct Resource<DX11VertexShader>;

	struct DX11VertexShaderBuilder : public IBuilder<DX11VertexShader>
	{
		DX11VertexShaderBuilder() : pDevice(nullptr), pInputLayoutDesc(nullptr), descElementCount(0) {}
		DX11VertexShaderBuilder(const std::wstring filePath) : IBuilder<DX11VertexShader>(filePath), pDevice(nullptr), pInputLayoutDesc(nullptr), descElementCount(0) {}

		virtual DX11VertexShader* build() const override;

		ID3D11Device* pDevice;
		const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc;
		UINT descElementCount;
	};
}


