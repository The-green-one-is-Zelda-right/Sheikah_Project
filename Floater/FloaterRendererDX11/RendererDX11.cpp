#include "RendererDX11.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "DX11VSConstantBuffer.h"
#include "DX11Mesh.h"
#include <DirectXColors.h>

#include <new>

#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>
#endif


#pragma region testIncludes
#include "TestBuilder.h"

#pragma endregion

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")


flt::RendererDX11::RendererDX11() :
	_hwnd(NULL),
	_isRunRenderEngine(false),
	_useVsync(false),
	_monitorIndex(0),
	_refreshRatesIndex(1),
	_windowWidth(1280),
	_windowHeight(720),
	_displayWidth(0),
	_displayHeight(0),
	_displayRefreshRates(),
	_device(),
	_immediateContext(),
	_dxgiFactory(),
	_adapters(),
	_outputs(),
	_swapChain(),
	_backBuffer(),
	_depthStencil(),
	_renderTargetView(),
	_depthStencilView(),
	_rasterizerState(),
	_renderableObjects(),
	_cameras()

{

}

flt::RendererDX11::~RendererDX11()
{
	Finalize();
}

bool flt::RendererDX11::Initialize(HWND hwnd)
{
	_hwnd = hwnd;

	D3D_FEATURE_LEVEL maxSupportedFeatureLevel = D3D_FEATURE_LEVEL_9_1;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) || defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&_device,
		&maxSupportedFeatureLevel,
		&_immediateContext
	);

	if (result != S_OK)
	{
		ASSERT(false, "Device 생성 실패");
		return false;
	}

	UINT numQualityLevels = 0;
	result = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);

	if (result != S_OK)
	{
		ASSERT(false, "멀티 샘플 품질레벨 체크 실패");
		return false;
	}

	result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, __uuidof(IDXGIFactory2), (void**)(&_dxgiFactory));

	if (result != S_OK)
	{
		ASSERT(false, "DXGI Factory 생성 실패");
		return false;
	}

	if (!InitDisplayInfo())
	{
		ASSERT(false, "모니터 모드 초기화 실패");
		return false;
	}

	if (!InitGPUInfo())
	{
		ASSERT(false, "GPU 정보 초기화 실패");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC1 desc = { 0, };

	desc.Width = 0; // CreateSwapChainForHwnd 로 호출 시 0으로 세팅하면 해당 hwnd에서 런타임에 출력창에서 너비를 가져옵니다.
	desc.Height = 0; // CreateSwapChainForHwnd 로 호출 시 0으로 세팅하면 해당 hwnd에서 런타임에 출력창에서 높이를 가져옵니다.
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Stereo = FALSE;
	desc.SampleDesc = { 1, 0 };
	desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = { };
	fullScreenDesc.RefreshRate = { 0, 0 };
	fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;
	fullScreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	fullScreenDesc.Windowed = true;

	result = _dxgiFactory->CreateSwapChainForHwnd(_device.Get(), _hwnd, &desc, &fullScreenDesc, NULL, &_swapChain);

	if (result != S_OK)
	{
		ASSERT(false, "스왑체인 생성 실패");
		return false;
	}

	OnResize();
	//if (!Resize(1280, 720))
	//{
	//	ASSERT(false, "화면 사이즈 변경 실패");
	//	return false;
	//}

	D3D11_RASTERIZER_DESC rasterizerDesc = { };
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = _device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
	if (result != S_OK)
	{
		ASSERT(false, "RasterizerState 생성 실패");
		return false;
	}

	_immediateContext->RSSetState(_rasterizerState.Get());

	_isRunRenderEngine = true;
	return true;
}

bool flt::RendererDX11::Finalize()
{
	_device = nullptr;
	_immediateContext = nullptr;
	_dxgiFactory = nullptr;
	_swapChain = nullptr;
	_backBuffer = nullptr;
	_depthStencil = nullptr;
	_renderTargetView = nullptr;
	_depthStencilView = nullptr;
	_rasterizerState = nullptr;

	for (auto& adapter : _adapters)
	{
		adapter = nullptr;
	}
	_adapters.clear();

	for (auto& output : _outputs)
	{
		output = nullptr;
	}
	_outputs.clear();

	_isRunRenderEngine = false;

	return true;
}

bool flt::RendererDX11::Render(float deltaTime)
{
	// 렌더링을 시작하기 위해 초기화
	_immediateContext->ClearRenderTargetView(_renderTargetView.Get(), DirectX::Colors::Black);
	_immediateContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	_immediateContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// 각 mesh별로 들어가야하지만 일단 여기서 만들자.
	D3D11_RASTERIZER_DESC rasterizerDesc = { };
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	_device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());

	_immediateContext->RSSetState(rasterizerState.Get());

	for (auto& node : _renderableObjects)
	{
		DX11Mesh* mesh = static_cast<DX11Mesh*>(*node->mesh);

		if (!node->mesh)
		{
			continue;
		}

		// 렌더링
		//Matrix4f worldMatrix = GetWorldMatrixRecursive(&node->transform);
		Matrix4f worldMatrix = node->transform.GetWorldMatrix4f();
		DirectX::XMMATRIX world = ConvertXMMatrix(worldMatrix);

		DirectX::XMMATRIX worldViewProj = world;

		DX11VertexShader* vertexShader = static_cast<DX11VertexShader*>(mesh->vertexShader);
		DX11PixelShader* pixelShader = static_cast<DX11PixelShader*>(mesh->pixelShader);

		_immediateContext->IASetInputLayout(vertexShader->pInputLayout);
		_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_immediateContext->VSSetShader(vertexShader->pVertexShader, nullptr, 0);
		_immediateContext->PSSetShader(pixelShader->pPixelShader, nullptr, 0);

		_immediateContext->PSSetShaderResources(0, 1, &(mesh->texture));

		D3D11_MAPPED_SUBRESOURCE mappedResource = { };
		HRESULT hResult = _immediateContext->Map(vertexShader->pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (hResult != S_OK)
		{
			ASSERT(false, "상수 버퍼 맵핑 실패");
			return false;
		}
		DirectX::XMMATRIX* data = (DirectX::XMMATRIX*)mappedResource.pData;
		*data = worldViewProj;
		_immediateContext->Unmap(vertexShader->pConstantBuffer, 0);

		_immediateContext->VSSetConstantBuffers(0, 1, &(vertexShader->pConstantBuffer));

		_immediateContext->PSSetSamplers(0, 1, &mesh->sampler);

		UINT offset = 0;
		_immediateContext->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &mesh->singleVertexSize, &offset);
		_immediateContext->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		_immediateContext->DrawIndexed(mesh->indexCount, 0, 0);
	}


	// 수직동기화 여부에 따라서 present
	if (_useVsync)
	{
		_swapChain->Present(1, 0);
	}
	else
	{
		_swapChain->Present(0, 0);
	}

	return true;
}

flt::HOBJECT flt::RendererDX11::RegisterObject(RendererObject& renderable)
{
	DX11Node* node = new DX11Node(renderable.node.transform, renderable.isDraw);
	if (!node)
	{
		return false;
	}

	node->mesh = CreateBox();
	if (!node->mesh)
	{
		return false;
	}

	_renderableObjects.push_back(node);

	if (node->camera)
	{
		_cameras.push_back(node->camera);
	}

	return true;
}

bool flt::RendererDX11::DeregisterObject(HOBJECT renderable)
{
	return false;
}

bool flt::RendererDX11::Test()
{
	test::BuilderTester tester;
	return tester.Test();
}

bool flt::RendererDX11::Resize(unsigned __int32 windowWidth, unsigned __int32 windowHeight)
{
	if (!_swapChain)
	{
		return false;
	}

	DXGI_MODE_DESC desc = { 0, };
	desc.Width = windowWidth;
	desc.Height = windowHeight;
	if (_useVsync)
	{
		// 일단은 임시로 세팅해 놓자.
		desc.RefreshRate.Numerator = _displayRefreshRates[_monitorIndex][_refreshRatesIndex].Numerator;
		desc.RefreshRate.Denominator = _displayRefreshRates[_monitorIndex][_refreshRatesIndex].Denominator;
	}
	else
	{
		desc.RefreshRate.Numerator = 0;
		desc.RefreshRate.Denominator = 1;
	}

	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	_swapChain->ResizeTarget(&desc);

	OnResize();

	_windowWidth = windowHeight;
	_windowHeight = windowHeight;

	_displayWidth = windowHeight;
	_displayHeight = windowHeight;
	return true;
}

bool flt::RendererDX11::InitDisplayInfo()
{
	if (_dxgiFactory == nullptr)
	{
		ASSERT(false, "dxgiFactory 부터 만드세요");
		return false;
	}

	IDXGIAdapter1* adapter = nullptr;
	for (int i = 0; _dxgiFactory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		_adapters.emplace_back(adapter);
		adapter->Release();
	}

	IDXGIOutput1* output = nullptr;
	for (int i = 0; _adapters[0]->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		_outputs.emplace_back(output);
		output->Release();
	}

	_displayRefreshRates.resize(_outputs.size());

	for (int i = 0; i < _outputs.size(); ++i)
	{
		unsigned __int32 displayModeNum = 0;
		HRESULT result = _outputs[i]->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &displayModeNum, NULL);
		if (result != S_OK)
		{
			ASSERT(false, "디스플레이 모드 리스트 갯수 가져오기 실패");
			return false;
		}
		if (displayModeNum == 0)
		{
			ASSERT(false, "디스플레이 모드 리스트 갯수가 0개 입니다.");
			return false;
		}

		DXGI_MODE_DESC1* dxgiModeArr = new(std::nothrow) DXGI_MODE_DESC1[displayModeNum];

		if (dxgiModeArr == nullptr)
		{
			ASSERT(false, "디스플레이 모드 리스트 할당 실패");
			return false;
		}

		result = _outputs[i]->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &displayModeNum, dxgiModeArr);
		if (result != S_OK)
		{
			delete[] dxgiModeArr;
			return false;
		}

		for (unsigned int j = 0; j < displayModeNum; ++j)
		{
			if (dxgiModeArr[j].Width == _displayWidth
				&& dxgiModeArr[j].Height == _displayHeight)
			{
				_displayRefreshRates[i].emplace_back(dxgiModeArr[j].RefreshRate);
			}
		}

		delete[] dxgiModeArr;
	}

	return true;
}

bool flt::RendererDX11::InitGPUInfo()
{
	if (_adapters.empty())
	{
		ASSERT(false, "디스플레이 모드를 먼저 초기화 해주세요");
		return false;
	}

	for (int i = 0; i < _adapters.size(); ++i)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		_adapters[i]->GetDesc1(&adapterDesc);

		SIZE_T gpuMemoryMB = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
		std::wstring gpuName = adapterDesc.Description;
	}

	return true;
}

bool flt::RendererDX11::OnResize()
{
	if (!_immediateContext)
	{
		return false;
	}

	_immediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	_renderTargetView = nullptr;
	_depthStencilView = nullptr;
	_depthStencil = nullptr;
	_backBuffer = nullptr;

	HRESULT result = _swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
	if (result != S_OK)
	{
		ASSERT(false, "스왑체인 리사이즈 실패");
		return false;
	}

	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&_backBuffer));

	if (result != S_OK)
	{
		ASSERT(false, "버퍼 가져오기 실패");
		return false;
	}

	result = _device->CreateRenderTargetView(_backBuffer.Get(), nullptr, &_renderTargetView);

	if (result != S_OK)
	{
		ASSERT(false, "렌더 타겟 뷰 생성 실패");
		return false;
	}

	D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
	_backBuffer->GetDesc(&backBufferDesc);

	// 뎁스 스텐실 뷰 만들기
	CD3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Width = backBufferDesc.Width;
	depthStencilDesc.Height = backBufferDesc.Height;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_device->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencil);

	_device->CreateDepthStencilView(_depthStencil.Get(), nullptr, &_depthStencilView);

	_immediateContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// 뷰포트 생성
	D3D11_VIEWPORT viewPort[1]{};
	viewPort[0].TopLeftX = 0.0f;
	viewPort[0].TopLeftY = 0.0f;
	viewPort[0].Width = (FLOAT)backBufferDesc.Width;
	viewPort[0].Height = (FLOAT)backBufferDesc.Height;
	viewPort[0].MinDepth = 0.0f;
	viewPort[0].MaxDepth = 1.0f;

	_immediateContext->RSSetViewports(1, viewPort);

	_displayWidth = backBufferDesc.Width;
	_displayHeight = backBufferDesc.Height;

	// 테스트를 위한 배경 색상 변경
	float bgColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	_immediateContext->ClearRenderTargetView(_renderTargetView.Get(), bgColor);

	return true;
}

void flt::RendererDX11::RenderSingleNodeRecursive(DX11Node* node, const Matrix4f& parentMatrix)
{
	Matrix4f worldMatrix = node->transform.GetLocalMatrix4f() * parentMatrix;
	Matrix4f viewProjMatrix = Matrix4f::Identity();

	for (auto& [name, child] : node->children)
	{
		RenderSingleNodeRecursive(child, worldMatrix);
	}

	if (!node->mesh)
	{
		return;
	}

	// 렌더링
	VSConstantBuffer vsConstantBuffer
	{
		ConvertXMMatrix(worldMatrix),
		ConvertXMMatrix(worldMatrix.Inverse().Transpose()),
		ConvertXMMatrix(viewProjMatrix),
		ConvertXMMatrix(worldMatrix * viewProjMatrix)
	};
}

bool flt::RendererDX11::SetVsConstantBuffer(ID3D11Buffer* vsConstantBuffer, void* pData, size_t dataSize, UINT slot)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hResult = _immediateContext->Map(vsConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (hResult != S_OK)
	{
		return false;
	}

	memcpy(mappedResource.pData, pData, dataSize);
	_immediateContext->Unmap(vsConstantBuffer, 0);

	_immediateContext->VSSetConstantBuffers(slot, 1, &vsConstantBuffer);

	return true;
}

flt::Resource<flt::DX11Mesh>* flt::RendererDX11::CreateBox()
{
	DX11CubeBuilder cubeBuilder;
	cubeBuilder.pDevice = _device.Get();
	cubeBuilder.pImmediateContext = _immediateContext.Get();

	Resource<DX11Mesh>* meshResource = new Resource<DX11Mesh>(cubeBuilder);

	return meshResource;
}
