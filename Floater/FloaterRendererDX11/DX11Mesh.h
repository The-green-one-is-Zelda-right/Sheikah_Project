#pragma once
#include <d3d11.h>
#include "CommonMath.h"
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "DX11PixelShader.h"
#include "DX11VertexShader.h"

namespace flt
{
	struct Vertex
	{
		using type = struct _Type
		{
			Vector3f pos;
			Vector2f tex;
			Vector3f normal;
			Vector3f binormal;
			Vector3f tangent;
		};

		static constexpr unsigned int numElements = 5;
		static constexpr D3D11_INPUT_ELEMENT_DESC layout[numElements] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(type, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(type, tex), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(type, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(type, binormal), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(type, tangent), D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	};

	struct DX11Mesh
	{
		DX11Mesh(const DX11VertexShaderBuilder& vsBuilder, const DX11PixelShaderBuilder& psBuilder) :
			vertexShader(vsBuilder),
			pixelShader(psBuilder),
			vertexBuffer(nullptr),
			singleVertexSize(0),
			indexBuffer(nullptr),
			indexCount(0),
			texture(nullptr),
			sampler(nullptr)
		{}

		void Release();

		Resource<DX11VertexShader> vertexShader;
		Resource<DX11PixelShader> pixelShader;

		ID3D11Buffer* vertexBuffer;
		UINT singleVertexSize;

		ID3D11Buffer* indexBuffer;
		UINT indexCount;

		ID3D11ShaderResourceView* texture;
		ID3D11SamplerState* sampler;
	};

	template struct Resource<DX11Mesh>;

	struct DX11MeshBuilder : public IBuilder<DX11Mesh>
	{
		DX11MeshBuilder(const std::wstring& name) : IBuilder<DX11Mesh>(name), pDevice(nullptr), vsBuilder(), psBuilder() {}

		virtual DX11Mesh* build() const override;

		ID3D11Device* pDevice;
		DX11VertexShaderBuilder vsBuilder;
		DX11PixelShaderBuilder psBuilder;
	};

	struct DX11CubeBuilder : public DX11MeshBuilder
	{
		DX11CubeBuilder() : DX11MeshBuilder(L"flt::CubeBuilder"), pImmediateContext(nullptr) {}

		virtual DX11Mesh* build() const override;

		ID3D11DeviceContext* pImmediateContext;
	};
}
