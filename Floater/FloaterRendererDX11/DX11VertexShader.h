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
		DX11VertexShaderBuilder() : pDevice(nullptr), inputLayoutDesc() {}
		DX11VertexShaderBuilder(const std::wstring filePath) : IBuilder<DX11VertexShader>(filePath), pDevice(nullptr), inputLayoutDesc() {}
		DX11VertexShaderBuilder(const DX11VertexShaderBuilder& other) = delete;
		DX11VertexShaderBuilder(DX11VertexShaderBuilder&& other) noexcept : IBuilder<DX11VertexShader>(std::move(other)), pDevice(other.pDevice), inputLayoutDesc(std::move(other.inputLayoutDesc))
		{
			other.pDevice = nullptr;
		}

		DX11VertexShaderBuilder& operator=(const DX11VertexShaderBuilder& other) = delete;
		DX11VertexShaderBuilder& operator=(DX11VertexShaderBuilder&& other) noexcept
		{
			IBuilder<DX11VertexShader>::operator=(std::move(other));
			pDevice = other.pDevice;
			inputLayoutDesc = other.inputLayoutDesc;
			other.pDevice = nullptr;
			return *this;
		}

		virtual ~DX11VertexShaderBuilder();

		virtual DX11VertexShader* build() const override;

		ID3D11Device* pDevice;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	};
}


