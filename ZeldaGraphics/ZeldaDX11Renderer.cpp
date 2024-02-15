#include "ZeldaDX11Renderer.h"

#include <cassert>

#include <ppltasks.h>
#include <cmath>
#include <wrl/client.h>

#include "ResourceManager.h"
#include "ZeldaCamera.h"
#include "ZeldaModel.h"
#include "ZeldaLight.h"
#include "ZeldaShader.h"
#include "ZeldaMesh.h"
#include "ZeldaTexture.h"

#include "MathConverter.h"

#include "ConstantBufferManager.h"

using namespace DirectX;

bool ZeldaDX11Renderer::Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen)
{
	hWnd = hwnd;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes = 0u;
	unsigned int numerator = 0u;
	unsigned int denominator = 0u;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC defaultRasterDesc;
	D3D11_RASTERIZER_DESC wireFrameRasterDesc;
	D3D11_RASTERIZER_DESC lightRasterDesc;
	D3D11_RASTERIZER_DESC cubeMapRasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// vsync(수직동기화) 설정 저장.
	bVsyncEnabled = vsync;

	// DirectX 그래픽스 인터페이스인 IDXGIFactory 생성.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) return false;

	// Factory 객체를 사용하여 기본으로 설정된 그래픽스 인터페이스를 위한 어댑터 생성(비디오 카드를 말함).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	// 기본으로 설정된 어댑터 출력(모니터)를 열거한다.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	// 어댑터 출력(모니터)의 DXGI_FORMAT_R8G8B8A8_UNORM 화면 포맷에 맞는 모드의 개수를 구한다. 
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result)) return false;

	// 해당 모니터/비디오 카드 조합으로 가능한 모든 디스플레이 모드의 리스트 생성.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// 화면 모드 리스트를 채운다.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList);
	if (FAILED(result)) return false;

	// 이제 모든 디스플레이 모드를 살펴보고 화면 너비 및 높이와 일치하는 모드를 찾는다.
	// 일치하는 항목을 찾으면 해당 모니터 주사율의 분모값과 분자값을 저장한다.
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == screenWidth)
		{
			if (displayModeList[i].Height == screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	// 어댑터의 description을 얻어온다.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	// 비디오 카드 메모리를 MB단위로 저장.
	// 정보가 제대로 저장되지 않는다면 주석한 다음 줄의 함수를 사용해 볼 것.
	mVideoCardMemory = (unsigned int)adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	//mVideoCardMemory = (unsigned int)adapterDesc.DedicatedSystemMemory / 1024 / 1024;

	// 비디오 카드의 이름을 배열에 저장.
	error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) return false;

	// 디스플레이 모드 리스트를 해제.
	delete[] displayModeList;
	displayModeList = 0;
	// adapterOutput 객체 해제.
	adapterOutput->Release();
	adapterOutput = 0;
	// adapter 객체 해제.
	adapter->Release();
	adapter = 0;
	// Factory 객체 해제.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;
	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate of the back buffer.
	if (bVsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle of the window to render to.
	swapChainDesc.OutputWindow = hwnd;
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Set to full screen or windowed mode.
	if (fullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}
	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext);
	if (FAILED(result)) return false;

	// Get the pointer to the back buffer.
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result)) return false;

	// Create the render target view with the back buffer pointer.
	result = mDevice->CreateRenderTargetView(backBufferPtr, nullptr, &mRenderTargetView);
	if (FAILED(result)) return false;

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	// Create the texture for the depth buffer using the filled out description.
	result = mDevice->CreateTexture2D(&depthBufferDesc, nullptr, &mDepthStencilBuffer);
	if (FAILED(result)) return false;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil opertations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Create the depth stencil state.
	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(result)) return false;

	// Set the depth stencil state.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// Create the depth stencil view.
	result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(result)) return false;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


	// Setup the raster description which will determine how and what polygons will be drawn.
	defaultRasterDesc.AntialiasedLineEnable = false;
	defaultRasterDesc.CullMode = D3D11_CULL_BACK;
	defaultRasterDesc.DepthBias = 0;
	defaultRasterDesc.DepthBiasClamp = 0.0f;
	defaultRasterDesc.DepthClipEnable = true;
	defaultRasterDesc.FillMode = D3D11_FILL_SOLID;
	defaultRasterDesc.FrontCounterClockwise = false;
	defaultRasterDesc.MultisampleEnable = false;
	defaultRasterDesc.ScissorEnable = false;
	defaultRasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&defaultRasterDesc, &defaultRasterState);
	if (FAILED(result)) return false;

	// Setup the raster description which will determine how and what polygons will be drawn.
	wireFrameRasterDesc.AntialiasedLineEnable = false;
	wireFrameRasterDesc.CullMode = D3D11_CULL_NONE;
	wireFrameRasterDesc.DepthBias = 0;
	wireFrameRasterDesc.DepthBiasClamp = 0.0f;
	wireFrameRasterDesc.DepthClipEnable = true;
	wireFrameRasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireFrameRasterDesc.FrontCounterClockwise = false;
	wireFrameRasterDesc.MultisampleEnable = false;
	wireFrameRasterDesc.ScissorEnable = false;
	wireFrameRasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&wireFrameRasterDesc, &wireFrameRasterState);
	if (FAILED(result)) return false;

	// Setup the raster description which will determine how and what polygons will be drawn.
	lightRasterDesc.AntialiasedLineEnable = false;
	lightRasterDesc.CullMode = D3D11_CULL_FRONT;
	lightRasterDesc.DepthBias = 0;
	lightRasterDesc.DepthBiasClamp = 0.0f;
	lightRasterDesc.DepthClipEnable = false;
	lightRasterDesc.FillMode = D3D11_FILL_SOLID;
	lightRasterDesc.FrontCounterClockwise = false;
	lightRasterDesc.MultisampleEnable = false;
	lightRasterDesc.ScissorEnable = false;
	lightRasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&lightRasterDesc, &pointLightRasterState);
	if (FAILED(result)) return false;

	// Setup the raster description which will determine how and what polygons will be drawn.
	cubeMapRasterDesc.AntialiasedLineEnable = false;
	cubeMapRasterDesc.CullMode = D3D11_CULL_FRONT;
	cubeMapRasterDesc.DepthBias = 0;
	cubeMapRasterDesc.DepthBiasClamp = 0.0f;
	cubeMapRasterDesc.DepthClipEnable = false;
	cubeMapRasterDesc.FillMode = D3D11_FILL_SOLID;
	cubeMapRasterDesc.FrontCounterClockwise = false;
	cubeMapRasterDesc.MultisampleEnable = false;
	cubeMapRasterDesc.ScissorEnable = false;
	cubeMapRasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&cubeMapRasterDesc, &cubeMapRasterState);
	if (FAILED(result)) return false;

	// Set up the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	// Create the viewport.
	mDeviceContext->RSSetViewports(1, &viewport);

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr)) return false;

	spriteBatch = new SpriteBatch(mDeviceContext);
	commonStates = new CommonStates(mDevice);

	ResourceManager::GetInstance().Initialize(mDevice);

	ConstantBufferManager::GetInstance().Initialize(mDevice, mDeviceContext);
	matrixVsConstBuffer = new ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>(mDevice);
	animationConstBuffer = new ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>(mDevice);
	instancingMatrixVsConstBuffer = new ConstantBuffer<InstancingMatrixBufferType, ShaderType::VertexShader>(mDevice);
	instancingAnimationVsConstBuffer = new ConstantBuffer<InstancingAnimationBufferType, ShaderType::VertexShader>(mDevice);
	animationHierarchyVsConstBuffer = new ConstantBuffer<AnimationHierarchyBufferType, ShaderType::VertexShader>(mDevice);

	matrixPsConstBuffer = new ConstantBuffer<MatrixBufferType, ShaderType::PixelShader>(mDevice);
	lightInfoConstBuffer = new ConstantBuffer<LightInfoBufferType, ShaderType::PixelShader>(mDevice);
	lightIndexConstBuffer = new ConstantBuffer<LightIndexBufferType, ShaderType::PixelShader>(mDevice);
	materialConstBuffer = new ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>(mDevice);

	screenConstBuffer = new ConstantBuffer<ScreenBufferType, ShaderType::VertexShaderAndPixelShader>(mDevice);

	screenConstBuffer->SetData({ { static_cast<float>(screenWidth), static_cast<float>(screenHeight) } });
	ConstantBufferManager::GetInstance().SetBuffer();

#pragma region Deferred Rendering

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = screenWidth;
	textureDesc.Height = screenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* renderTargetTextures[Deferred::BufferCount];

	// Texture2D 생성
	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		result = mDevice->CreateTexture2D(&textureDesc, NULL, &renderTargetTextures[i]);
		if (FAILED(result)) return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// RenderTargetView 생성
	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		result = mDevice->CreateRenderTargetView(renderTargetTextures[i], &renderTargetViewDesc, &deferredRenderTargets[i]);
		if (FAILED(result)) return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Shader Resource View 생성
	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		result = mDevice->CreateShaderResourceView(renderTargetTextures[i], &shaderResourceViewDesc, &deferredShaderResources[i]);
		if (FAILED(result)) return false;
	}

	//Release render target texture array
	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		renderTargetTextures[i]->Release();
	}

	deferredObjectShader = new ZeldaShader(mDevice, L"DeferredObjectVS.cso", L"DeferredObjectPS.cso", L"DeferredInstancingObjectVS.cso");
	deferredDirectionalLightShader = new ZeldaShader(mDevice, L"DeferredDirectionalLightVS.cso", L"DeferredDirectionalLightPS.cso");
	deferredPointLightShader = new ZeldaShader(mDevice, L"DeferredPointLightVS.cso", L"DeferredPointLightPS.cso");
	deferredSpotLightShader = new ZeldaShader(mDevice, L"DeferredSpotLightVS.cso", L"DeferredSpotLightPS.cso");
	deferredFinalShader = new ZeldaShader(mDevice, L"DeferredFinalVS.cso", L"DeferredFinalPS.cso");
	forwardShader = new ZeldaShader(mDevice, L"ForwardVS.cso", L"ForwardPS.cso", L"ForwardInstancingVS.cso");
	cubeMapShader = new ZeldaShader(mDevice, L"CubeMapVS.cso", L"CubeMapPS.cso", L"");

#pragma endregion Deferred Rendering

	// 깊이 스텐실 스테이트 생성
	D3D11_DEPTH_STENCIL_DESC cubeMapDepthStencilDesc = {};
	cubeMapDepthStencilDesc.DepthEnable = TRUE; // 깊이 테스트를 활성화합니다.
	cubeMapDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 버퍼에 쓰기를 허용합니다.
	cubeMapDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // 깊이 테스트 함수를 설정합니다. 여기서는 깊이가 작은 값일 때만 통과하도록 설정했습니다.
	cubeMapDepthStencilDesc.StencilEnable = FALSE; // 스텐실 테스트를 비활성화합니다.

	mDevice->CreateDepthStencilState(&cubeMapDepthStencilDesc, &cubeMapDepthStencilState);


	// 블렌드 상태 정의
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 블렌드 스테이트 생성
	result = mDevice->CreateBlendState(&blendDesc, &alphaBlendState);
	if (FAILED(result)) return false;

	debugMode = DebugMode::Normal;
	debugModeBuffer = DebugMode::Normal;

	rendererMode = RendererMode::None;
	rendererModeBuffer = RendererMode::None;

	CreateBasicResources();

	return true;
}

void ZeldaDX11Renderer::Finalize()
{
	ReleaseBasicResources();

	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		if (deferredRenderTargets[i])
		{
			deferredRenderTargets[i]->Release();
			deferredRenderTargets[i] = nullptr;
		}
	}

	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		if (deferredShaderResources[i])
		{
			deferredShaderResources[i]->Release();
			deferredShaderResources[i] = nullptr;
		}
	}

	if (alphaBlendState)
	{
		alphaBlendState->Release();
		alphaBlendState = nullptr;
	}
	if (cubeMapDepthStencilState)
	{
		cubeMapDepthStencilState->Release();
		cubeMapDepthStencilState = nullptr;
	}
	if (matrixVsConstBuffer)
	{
		delete matrixVsConstBuffer;
		matrixVsConstBuffer = nullptr;
	}
	if (animationConstBuffer)
	{
		delete animationConstBuffer;
		animationConstBuffer = nullptr;
	}
	if (instancingMatrixVsConstBuffer)
	{
		delete instancingMatrixVsConstBuffer;
		instancingMatrixVsConstBuffer = nullptr;
	}
	if (instancingAnimationVsConstBuffer)
	{
		delete instancingAnimationVsConstBuffer;
		instancingAnimationVsConstBuffer = nullptr;
	}
	if (animationHierarchyVsConstBuffer)
	{
		delete animationHierarchyVsConstBuffer;
		animationHierarchyVsConstBuffer = nullptr;
	}
	if (matrixPsConstBuffer)
	{
		delete matrixPsConstBuffer;
		matrixPsConstBuffer = nullptr;
	}
	if (lightInfoConstBuffer)
	{
		delete lightInfoConstBuffer;
		lightInfoConstBuffer = nullptr;
	}
	if (lightIndexConstBuffer)
	{
		delete lightIndexConstBuffer;
		lightIndexConstBuffer = nullptr;
	}
	if (materialConstBuffer)
	{
		delete materialConstBuffer;
		materialConstBuffer = nullptr;
	}
	if (screenConstBuffer)
	{
		delete screenConstBuffer;
		screenConstBuffer = nullptr;
	}
	if (mRasterState)
	{
		mRasterState->Release();
		mRasterState = 0;
	}
	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = 0;
	}
	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = 0;
	}
	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = 0;
	}
	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = 0;
	}
	if (mDeviceContext)
	{
		mDeviceContext->Release();
		mDeviceContext = 0;
	}
	if (mDevice)
	{
		mDevice->Release();
		mDevice = 0;
	}
	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = 0;
	}
	if (spriteBatch)
	{
		delete spriteBatch;
		spriteBatch = nullptr;
	}
	if (commonStates)
	{
		delete commonStates;
		commonStates = nullptr;
	}
	if (deferredObjectShader)
	{
		delete deferredObjectShader;
		deferredObjectShader = nullptr;
	}
	if (deferredDirectionalLightShader)
	{
		delete deferredDirectionalLightShader;
		deferredDirectionalLightShader = nullptr;
	}
	if (deferredPointLightShader)
	{
		delete deferredPointLightShader;
		deferredPointLightShader = nullptr;
	}
	if (deferredSpotLightShader)
	{
		delete deferredSpotLightShader;
		deferredSpotLightShader = nullptr;
	}
	if (deferredFinalShader)
	{
		delete deferredFinalShader;
		deferredFinalShader = nullptr;
	}
	if (forwardShader)
	{
		delete forwardShader;
		forwardShader = nullptr;
	}
	if (cubeMapShader)
	{
		delete cubeMapShader;
		cubeMapShader = nullptr;
	}
	if (defaultRasterState)
	{
		defaultRasterState->Release();
		defaultRasterState = nullptr;
	}
	if (wireFrameRasterState)
	{
		wireFrameRasterState->Release();
		wireFrameRasterState = nullptr;
	}
	if (pointLightRasterState)
	{
		pointLightRasterState->Release();
		pointLightRasterState = nullptr;
	}
	if (cubeMapRasterState)
	{
		cubeMapRasterState->Release();
		cubeMapRasterState = nullptr;
	}
	if (currentRasterState)
	{
		// 얘는 defaultRasterState, wireFrameRasterState를 받아두기만 하기 때문에 해제 필요없음
		currentRasterState = nullptr;
	}

	ResourceManager::GetInstance().Finalize();
}

void ZeldaDX11Renderer::SetDebugMode(DebugMode mode)
{
	debugModeBuffer = mode;
}

void ZeldaDX11Renderer::SetRendererMode(RendererMode mode)
{
	rendererModeBuffer = mode;
}

void ZeldaDX11Renderer::BeginDraw(float deltaTime)
{
#ifdef USE_BEGIN_FLAG
	// 이미 BeginDraw를 호출함
	assert(beginflag == false);
	beginflag = true;
#endif

	UpdateMode();

	ClearRenderInfo();
}

void ZeldaDX11Renderer::EndDraw()
{
#ifdef USE_BEGIN_FLAG
	// 이미 EndDraw를 호출함
	assert(beginflag == true);
	beginflag = false;
#endif


	float gray[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		mDeviceContext->ClearRenderTargetView(deferredRenderTargets[i], black);
	}

	// Clear the back buffer.
	bool existLight = organizedLightRenderInfo.size() != 0;
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, (existLight) ? (gray) : (black));
		
	// Clear the depth buffer.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	DrawDeferred();

	DrawForward();

	DrawCubeMapRenderInfo();

	BeginDrawSprite();
	DrawSprite();
	EndDrawSprite();


	// Present the back buffer to the screen since rendering is complete.
	if (bVsyncEnabled)
	{
		// Lock to screen refresh rate.
		mSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		mSwapChain->Present(0, 0);
	}
}

void ZeldaDX11Renderer::DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a)
{
	MeshID cubeID = ResourceManager::GetInstance().GetCubeID();

	MeshInstancingInfo instancinInfo;
	instancinInfo.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);

	std::unordered_map<std::pair<std::pair<MeshID, TextureID>, std::pair<bool, Color>>, MeshRenderInfo>& renderInfoContainer = (wireFrame || CheckRendererMode(RendererMode::OnWireFrameMode)) ? (forwardMeshRenderInfo) : (organizedMeshRenderInfo);

	auto iter = renderInfoContainer.find({ { cubeID, texture }, { wireFrame, { r, g, b, a } } });

	// 이미 동일한 것을 그린적이 있음
	if (iter != renderInfoContainer.end())
	{
		iter->second.instancingInfo.push_back(instancinInfo);
	}
	// 처음 그리는 경우
	else
	{
		MeshRenderInfo renderInfo;
		renderInfo.instancingInfo.assign(1, instancinInfo);
		renderInfo.meshId = cubeID;
		renderInfo.textureID = texture;
		renderInfo.wireFrame = wireFrame;
		renderInfo.color = { r, g, b, a };

		renderInfoContainer[{ { cubeID, texture }, { wireFrame, { r, g, b, a } } }] = renderInfo;
	}
}

void ZeldaDX11Renderer::DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame)
{
	ModelInstancingInfo instancinInfo;
	instancinInfo.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	instancinInfo.time = 0.0f;

	std::unordered_map<std::pair<std::pair<ModelID, std::wstring>, bool>, ModelRenderInfo>& renderInfoContainer = (wireFrame || CheckRendererMode(RendererMode::OnWireFrameMode)) ? (forwardModelRenderInfo) : (organizedModelRenderInfo);

	auto iter = renderInfoContainer.find({ { model, L"" }, wireFrame});

	// 이미 동일한 것을 그린적이 있음
	if (iter != renderInfoContainer.end())
	{
		iter->second.instancingInfo.push_back(instancinInfo);
	}
	// 처음 그리는 경우
	else
	{
		ModelRenderInfo renderInfo;
		renderInfo.instancingInfo.assign(1, instancinInfo);
		renderInfo.modelID = model;
		renderInfo.animationName = L"";
		renderInfo.wireFrame = wireFrame;

		renderInfoContainer[{ { model, L"" }, wireFrame }] = renderInfo;
	}
}

void ZeldaDX11Renderer::DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame)
{
	ModelInstancingInfo instancinInfo;
	instancinInfo.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	instancinInfo.time = animationTime;

	std::unordered_map<std::pair<std::pair<ModelID, std::wstring>, bool>, ModelRenderInfo>& renderInfoContainer = (wireFrame || CheckRendererMode(RendererMode::OnWireFrameMode)) ? (forwardModelRenderInfo) : (organizedModelRenderInfo);

	auto iter = renderInfoContainer.find({ { model, animationName }, wireFrame });

	// 이미 동일한 것을 그린적이 있음
	if (iter != renderInfoContainer.end())
	{
		iter->second.instancingInfo.push_back(instancinInfo);
	}
	// 처음 그리는 경우
	else
	{
		ModelRenderInfo renderInfo;
		renderInfo.instancingInfo.assign(1, instancinInfo);
		renderInfo.modelID = model;
		renderInfo.animationName = animationName;
		renderInfo.wireFrame = wireFrame;

		renderInfoContainer[{ { model, animationName }, wireFrame }] = renderInfo;
	}
}

void ZeldaDX11Renderer::DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring firstAnimationName, std::wstring secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame)
{
	assert(0);
}

void ZeldaDX11Renderer::DrawLight(LightID lightID)
{
	auto iter = organizedLightRenderInfo.find(lightID);

	// 처음 그리는 경우
	if (iter == organizedLightRenderInfo.end())
	{
		organizedLightRenderInfo.insert(lightID);
	}
}

void ZeldaDX11Renderer::DrawSprite(const Eigen::Vector2f& position, TextureID texture)
{
	SpriteInstancingInfo instancinInfo;
	instancinInfo.position = { position.x(), position.y() };

	auto iter = organizedSpriteRenderInfo.find(texture);

	// 이미 동일한 것을 그린적이 있음
	if (iter != organizedSpriteRenderInfo.end())
	{
		iter->second.instancingInfo.push_back(instancinInfo);
	}
	// 처음 그리는 경우
	else
	{
		SpriteRenderInfo renderInfo;
		renderInfo.instancingInfo.assign(1, instancinInfo);
		renderInfo.textureID = texture;

		organizedSpriteRenderInfo[texture] = renderInfo;
	}
}

void ZeldaDX11Renderer::DrawCubeMap(TextureID texture)
{
	cubeMapRenderInfo = texture;
}

void ZeldaDX11Renderer::CreateBasicResources()
{
	ResourceManager::GetInstance().CreateCubeMesh();
	ResourceManager::GetInstance().CreateSquareMesh();
	ResourceManager::GetInstance().CreateCircleMesh();
	ResourceManager::GetInstance().CreateSphereMesh();
	ResourceManager::GetInstance().CreateCapsuleMesh();
	ResourceManager::GetInstance().CreateCylinderMesh();
}

void ZeldaDX11Renderer::ReleaseBasicResources()
{
	ResourceManager::GetInstance().ReleaseCubeMesh();
	ResourceManager::GetInstance().ReleaseSquareMesh();
	ResourceManager::GetInstance().ReleaseCircleMesh();
	ResourceManager::GetInstance().ReleaseSphereMesh();
	ResourceManager::GetInstance().ReleaseCapsuleMesh();
	ResourceManager::GetInstance().ReleaseCylinderMesh();
}

void ZeldaDX11Renderer::DrawDeferred()
{
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr, nullptr, nullptr };
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin, 3, nullSRV);

	ZeldaCamera* mainCamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	matrixPsConstBuffer->SetData({ {}, XMMatrixTranspose(mainCamera->GetViewMatrix()), { XMMatrixTranspose(mainCamera->GetProjMatrix()) }, mainCamera->GetFar() });
	ConstantBufferManager::GetInstance().SetBuffer();

#pragma region Draw Deferred Object
	mDeviceContext->OMSetRenderTargets(Deferred::Object::Count, deferredRenderTargets + Deferred::Object::Begin, mDepthStencilView);

	for (auto& iter : organizedMeshRenderInfo)
	{
		DrawMeshRenderInfo(iter.second, deferredObjectShader);
	}
	
	for (auto& iter : organizedModelRenderInfo)
	{
		DrawModelRenderInfo(iter.second, deferredObjectShader);
	}
#pragma endregion


#pragma region Draw Deferred Light
	// 다음 그리기에 영향을 주지 않기 위해 뎁스스텐실을 비워놓음
	mDeviceContext->OMSetRenderTargets(Deferred::Light::Count, deferredRenderTargets + Deferred::Light::Begin, nullptr);
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin, 2, deferredShaderResources + Deferred::Object::Begin);
	float alphaBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(alphaBlendState, alphaBlendFactor, 0xFFFFFFFF);
	
	ZeldaMesh* squareMesh = ResourceManager::GetInstance().GetSquareMesh();

	LightInfoBufferType lightInfoData;
	unsigned int lightCount = 0;
	for (auto& iter : organizedLightRenderInfo)
	{
		lightInfoData.lights[lightCount] = ResourceManager::GetInstance().GetLight(iter)->GetLightInfo();
		lightCount++;
	}

	MatrixBufferType lightMatrixBuffer;
	lightMatrixBuffer.view = XMMatrixTranspose(mainCamera->GetViewMatrix());
	lightMatrixBuffer.projection = XMMatrixTranspose(mainCamera->GetProjMatrix());
	lightMatrixBuffer.cameraFar = mainCamera->GetFar();
	
	matrixPsConstBuffer->SetData(lightMatrixBuffer);
	lightInfoConstBuffer->SetData(lightInfoData);

	lightCount = 0;
	for (auto& iter : organizedLightRenderInfo)
	{
		ZeldaLight* light = ResourceManager::GetInstance().GetLight(iter);
		LightType lightType = light->GetLightType();
		ZeldaMesh* lightMesh = nullptr;
		ZeldaShader* lightShader = nullptr;

		if (lightType == LightType::Directional)
		{
			mDeviceContext->RSSetState(defaultRasterState);
			lightMesh = squareMesh;
			lightShader = deferredDirectionalLightShader;
		}
		else if (lightType == LightType::Point)
		{
			mDeviceContext->RSSetState(pointLightRasterState);
			lightMesh = ResourceManager::GetInstance().GetSphereMesh();
			lightShader = deferredPointLightShader;
		}
		else if (lightType == LightType::Spot)
		{
			// Spot Light Mesh 추가 필요

			mDeviceContext->RSSetState(defaultRasterState);
			
			lightShader = deferredSpotLightShader;
			assert(0);
		}
		else
		{
			assert(0);
		}

		lightMesh->Render(mDeviceContext);

		lightMatrixBuffer.world = XMMatrixTranspose(light->GetWorldMatrix());
		matrixVsConstBuffer->SetData(lightMatrixBuffer);

		LightIndexBufferType lightIndexData;
		lightIndexData.lightIndex = lightCount;
		lightIndexConstBuffer->SetData(lightIndexData);

		ConstantBufferManager::GetInstance().SetBuffer();

		lightShader->Render(mDeviceContext, lightMesh->GetIndexCount());

		lightCount++;
	}

	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	mDeviceContext->RSSetState(defaultRasterState);

#pragma endregion


#pragma region Draw Deferred Final
	// 다음 그리기에 영향을 주지 않기 위해 뎁스스텐실을 비워놓음
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin + 0, 1, deferredShaderResources + Deferred::Object::Albedo);
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin + 1, 2, deferredShaderResources + Deferred::Light::Begin);

	squareMesh->Render(mDeviceContext);
	deferredFinalShader->Render(mDeviceContext, squareMesh->GetIndexCount());

#pragma endregion
}

void ZeldaDX11Renderer::DrawForward()
{
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr, nullptr, nullptr };
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin, 3, nullSRV);
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	for (auto& iter : forwardMeshRenderInfo)
	{
		DrawMeshRenderInfo(iter.second, forwardShader);
	}

	for (auto& iter : forwardModelRenderInfo)
	{
		DrawModelRenderInfo(iter.second, forwardShader);
	}
}

void ZeldaDX11Renderer::BeginDrawSprite()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	spriteBatch->Begin(SpriteSortMode_Immediate, commonStates->NonPremultiplied(), nullptr, mDepthStencilState);
}

void ZeldaDX11Renderer::DrawSprite()
{
	const static long wsize = 240;
	const static long hsize = 135;
	const static long space = 20;

	long fullsize = wsize * Deferred::BufferCount + max(Deferred::BufferCount - 1, 0) * space;

#pragma region Draw Deferred Debug Info
	if (CheckDebugMode(DebugMode::DeferredDebug0))
	{
		spriteBatch->Draw(deferredShaderResources[0], RECT{ 0, 0, static_cast<long>(screenWidth), static_cast<long>(screenHeight) });
	}
	else if (CheckDebugMode(DebugMode::DeferredDebug1))
	{
		spriteBatch->Draw(deferredShaderResources[1], RECT{ 0, 0, static_cast<long>(screenWidth), static_cast<long>(screenHeight) });
	}
	else if (CheckDebugMode(DebugMode::DeferredDebug2))
	{
		spriteBatch->Draw(deferredShaderResources[2], RECT{ 0, 0, static_cast<long>(screenWidth), static_cast<long>(screenHeight) });
	}
	else if (CheckDebugMode(DebugMode::DeferredDebug3))
	{
		spriteBatch->Draw(deferredShaderResources[3], RECT{ 0, 0, static_cast<long>(screenWidth), static_cast<long>(screenHeight) });
	}
	else if (CheckDebugMode(DebugMode::DeferredDebug4))
	{
		spriteBatch->Draw(deferredShaderResources[4], RECT{ 0, 0, static_cast<long>(screenWidth), static_cast<long>(screenHeight) });
	}
	else if (CheckDebugMode(DebugMode::DeferredDebug5))
	{
		spriteBatch->Draw(deferredShaderResources[5], RECT{ 0, 0, static_cast<long>(screenWidth), static_cast<long>(screenHeight) });
	}

	if (CheckDebugMode(DebugMode::DeferredDebugAll))
	{
		for (int i = 0; i < Deferred::BufferCount; i++)
		{
			spriteBatch->Draw(deferredShaderResources[i], RECT{ ((1920l - fullsize) / 2) + (i) * (wsize + space), 100, ((1920l - fullsize) / 2) + (i) * (wsize + space) + wsize, 100 + hsize });
		}
	}
#pragma endregion

	for (auto iter : organizedSpriteRenderInfo)
	{
		DrawSpriteRenderInfo(iter.second);
	}
}

void ZeldaDX11Renderer::EndDrawSprite()
{
	spriteBatch->End();

	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	mDeviceContext->RSSetState(defaultRasterState);
}

TextureID ZeldaDX11Renderer::CreateTexture(const std::wstring& texturePath)
{
	return ResourceManager::GetInstance().CreateTexture(texturePath);
}

void ZeldaDX11Renderer::ReleaseTexture(TextureID textureID)
{
	ResourceManager::GetInstance().ReleaseTexture(textureID);
}

ModelID ZeldaDX11Renderer::CreateModel(const std::wstring& modelingFilePath)
{
	return ResourceManager::GetInstance().CreateModelFromModelingFile(modelingFilePath);
}

void ZeldaDX11Renderer::ReleaseModel(ModelID modelID)
{
	ResourceManager::GetInstance().ReleaseModel(modelID);
}

std::vector<std::wstring> ZeldaDX11Renderer::GetAnimationListByModel(ModelID modelID)
{
	return ResourceManager::GetInstance().GetModel(modelID)->GetAnimationList();
}

std::vector<float> ZeldaDX11Renderer::GetAnimationPlayTime(ModelID modelID)
{
	return ResourceManager::GetInstance().GetModel(modelID)->GetAnimationPlayTime();
}

CameraID ZeldaDX11Renderer::CreateCamera()
{
	return ResourceManager::GetInstance().CreateCamera(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
}

void ZeldaDX11Renderer::ReleaseCamera(CameraID cameraID)
{
	if (ZeldaCamera::GetMainCamera() == cameraID)
	{
		ZeldaCamera::SetMainCamera(CameraID::ID_NULL);
	}

	ResourceManager::GetInstance().ReleaseCamera(cameraID);
}

bool ZeldaDX11Renderer::SetMainCamera(CameraID cameraID)
{
	return ZeldaCamera::SetMainCamera(cameraID);
}

bool ZeldaDX11Renderer::UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar)
{
	ZeldaCamera* targetCamera = ResourceManager::GetInstance().GetCamera(cameraID);

	if (targetCamera == nullptr)
	{
		return false;
	}

	targetCamera->SetTransformMatrix(MathConverter::EigenMatrixToXMMatrix(worldMatrix));
	targetCamera->SetOption(fieldOfView, cameraNear, cameraFar);

	return true;
}

LightID ZeldaDX11Renderer::CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction)
{
	LightID lightID = ResourceManager::GetInstance().CreateDirectionalLight();

	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);

	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());

	light->SetDirection(direction.x(), direction.y(), direction.z());

	return lightID;
}

LightID ZeldaDX11Renderer::CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& position, float range)
{
	LightID lightID = ResourceManager::GetInstance().CreatePointLight();

	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);

	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());

	light->SetPosition(position.x(), position.y(), position.z());
	light->SetRange(range);

	return lightID;
}

LightID ZeldaDX11Renderer::CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle)
{
	LightID lightID = ResourceManager::GetInstance().CreateSpotLight();

	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);

	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());

	light->SetDirection(direction.x(), direction.y(), direction.z());
	light->SetPosition(position.x(), position.y(), position.z());
	light->SetRange(range);
	light->SetAngle(angle);

	return lightID;
}

void ZeldaDX11Renderer::ReleaseLight(LightID lightID)
{
	ResourceManager::GetInstance().ReleaseLight(lightID);
}

void ZeldaDX11Renderer::UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle)
{
	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);
	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());

	switch (light->GetLightType())
	{
		case LightType::Directional:
		{
			light->SetDirection(direction.x(), direction.y(), direction.z());
		}
		break;

		case LightType::Point:
		{
			light->SetPosition(position.x(), position.y(), position.z());
			light->SetRange(range);
		}
		break;

		case LightType::Spot:
		{
			light->SetPosition(position.x(), position.y(), position.z());
			light->SetRange(range);
			light->SetAngle(angle);
		}
		break;

		default:
			assert(0);
			break;
	}
}

void ZeldaDX11Renderer::DrawMeshRenderInfo(MeshRenderInfo renderInfo, ZeldaShader* shader)
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ZeldaMesh* cubeMesh = ResourceManager::GetInstance().GetCubeMesh();
	
	ZeldaTexture* texture = ResourceManager::GetInstance().GetTexture(renderInfo.textureID);

	MatrixBufferType matrixBuffer;
	matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
	matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());
	matrixBuffer.cameraFar = currentcamera->GetFar();
	matrixVsConstBuffer->SetData(matrixBuffer);

	// Instancing 사용
	if (renderInfo.instancingInfo.size() > 1)
	{
		cubeMesh->RenderInstanced(mDeviceContext);

		UseWireFrameRasterState(renderInfo.wireFrame);
		mDeviceContext->RSSetState(currentRasterState);
		materialConstBuffer->SetData({
				{ renderInfo.color.r, renderInfo.color.g, renderInfo.color.b, renderInfo.color.a },
				(texture == nullptr),
				(texture != nullptr) && texture->UseSRGB(),
				(texture != nullptr)
			});

		if (texture != nullptr)
		{
			texture->SetShaderResource(mDeviceContext);
		}

		InstancingMatrixBufferType* instacingMatrix = new InstancingMatrixBufferType();

		for (size_t i = 0; i < renderInfo.instancingInfo.size(); i++)
		{
			auto& instancingInfo = renderInfo.instancingInfo[i];

			// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
			instacingMatrix->instancingWorldMatrix[i % INSTANCING_MAX] = XMMatrixTranspose(instancingInfo.worldMatrix);

			// 인스턴싱 가능한 최대 갯수로 끊어서 그리기
			if (((i % INSTANCING_MAX) + 1 == INSTANCING_MAX) || (i == renderInfo.instancingInfo.size() - 1))
			{
				instancingMatrixVsConstBuffer->SetData(*instacingMatrix);
				ConstantBufferManager::GetInstance().SetBuffer();

				shader->RenderInstanced(mDeviceContext, cubeMesh->GetIndexCount(), (i % INSTANCING_MAX) + 1, 0);
			}
		}

		delete instacingMatrix;
	}
	else
	{
		cubeMesh->Render(mDeviceContext);

		for (size_t i = 0; i < renderInfo.instancingInfo.size(); i++)
		{
			auto& instancingInfo = renderInfo.instancingInfo[i];

			UseWireFrameRasterState(renderInfo.wireFrame);
			mDeviceContext->RSSetState(currentRasterState);
			materialConstBuffer->SetData({
					{ renderInfo.color.r, renderInfo.color.g, renderInfo.color.b, renderInfo.color.a },
					(texture == nullptr),
					(texture != nullptr) && texture->UseSRGB(),
					(texture != nullptr)
				});

			if (texture != nullptr)
			{
				texture->SetShaderResource(mDeviceContext);
			}

			// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
			matrixVsConstBuffer->SetData({ XMMatrixTranspose(instancingInfo.worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });

			ConstantBufferManager::GetInstance().SetBuffer();

			shader->Render(mDeviceContext, cubeMesh->GetIndexCount());
		}
	}
}

void ZeldaDX11Renderer::DrawModelRenderInfo(ModelRenderInfo renderInfo, ZeldaShader* shader)
{
	ZeldaModel* modelData = ResourceManager::GetInstance().GetModel(renderInfo.modelID);

	// Instancing 사용
	if (renderInfo.instancingInfo.size() > 1)
	{
		UseWireFrameRasterState(renderInfo.wireFrame);
		mDeviceContext->RSSetState(currentRasterState);

		modelData->RenderInstanced(mDeviceContext, matrixVsConstBuffer, instancingMatrixVsConstBuffer, animationHierarchyVsConstBuffer, instancingAnimationVsConstBuffer, materialConstBuffer, renderInfo.instancingInfo, shader, renderInfo.animationName);
	}
	else
	{
		for (size_t i = 0; i < renderInfo.instancingInfo.size(); i++)
		{
			auto& instancingInfo = renderInfo.instancingInfo[i];

			UseWireFrameRasterState(renderInfo.wireFrame);
			mDeviceContext->RSSetState(currentRasterState);

			modelData->Render(mDeviceContext, matrixVsConstBuffer, animationHierarchyVsConstBuffer, animationConstBuffer, materialConstBuffer, instancingInfo.worldMatrix, shader, renderInfo.animationName, instancingInfo.time);
		}
	}
}

void ZeldaDX11Renderer::DrawSpriteRenderInfo(SpriteRenderInfo renderInfo)
{
	ZeldaTexture* texture = ResourceManager::GetInstance().GetTexture(renderInfo.textureID);

	for (size_t i = 0; i < renderInfo.instancingInfo.size(); i++)
	{
		spriteBatch->Draw(texture->GetTexture(), renderInfo.instancingInfo[i].position);
	}
}

void ZeldaDX11Renderer::DrawCubeMapRenderInfo()
{
	if (cubeMapRenderInfo == TextureID::ID_NULL)
	{
		// CubeMap을 그리지 않음
		return;
	}

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->OMSetDepthStencilState(cubeMapDepthStencilState, 0);
	mDeviceContext->RSSetState(cubeMapRasterState);


	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ZeldaMesh* mesh = ResourceManager::GetInstance().GetSphereMesh();
	mesh->Render(mDeviceContext);

	ZeldaTexture* texture = ResourceManager::GetInstance().GetTexture(cubeMapRenderInfo);
	texture->SetCubeMapShaderResource(mDeviceContext);
	
	// 전치해야한다.
	MatrixBufferType matrixBuffer;
	matrixBuffer.world = XMMatrixTranspose(currentcamera->GetTransformMatrix());
	matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
	matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());

	matrixVsConstBuffer->SetData(matrixBuffer);
	matrixPsConstBuffer->SetData(matrixBuffer);

	ConstantBufferManager::GetInstance().SetBuffer();

	cubeMapShader->Render(mDeviceContext, mesh->GetIndexCount());

	mDeviceContext->RSSetState(defaultRasterState);
	mDeviceContext->OMSetDepthStencilState(nullptr, 0);
}

void ZeldaDX11Renderer::ClearRenderInfo()
{
	organizedMeshRenderInfo.clear();
	organizedModelRenderInfo.clear();
	organizedSpriteRenderInfo.clear();
	organizedLightRenderInfo.clear();

	forwardMeshRenderInfo.clear();
	forwardModelRenderInfo.clear();

	cubeMapRenderInfo = TextureID::ID_NULL;
}

void ZeldaDX11Renderer::UpdateMode()
{
	if (debugMode != debugModeBuffer)
	{
		debugMode = debugModeBuffer;
	}

	if (rendererModeBuffer != RendererMode::None)
	{
		if (static_cast<unsigned int>(rendererModeBuffer) >= 0x80000000u)
		{
			rendererMode = static_cast<RendererMode>((~static_cast<unsigned int>(rendererModeBuffer)) & static_cast<unsigned int>(rendererMode));
		}
		else
		{
			rendererMode = static_cast<RendererMode>(static_cast<unsigned int>(rendererModeBuffer) | static_cast<unsigned int>(rendererMode));
		}

		rendererModeBuffer = RendererMode::None;
	}
}

void ZeldaDX11Renderer::UseWireFrameRasterState(bool use)
{
	if (CheckRendererMode(RendererMode::OnWireFrameMode) || use)
	{
		currentRasterState = wireFrameRasterState;
	}
	else
	{
		currentRasterState = defaultRasterState;
	}
}

bool ZeldaDX11Renderer::CheckDebugMode(DebugMode mode)
{
	return debugMode == mode;
}

bool ZeldaDX11Renderer::CheckRendererMode(RendererMode mode)
{
	if (mode == RendererMode::None && rendererMode == RendererMode::None) return true;

	if (static_cast<unsigned int>(mode) >= 0x80000000u)
	{
		return (static_cast<unsigned int>(rendererMode) & (static_cast<unsigned int>(mode) - 0x80000000u)) == 0;
	}

	return static_cast<unsigned int>(rendererMode) & static_cast<unsigned int>(mode);
}