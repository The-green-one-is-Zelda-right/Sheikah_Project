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
#include "RenderInfoManager.h"

using namespace DirectX;

bool ZeldaDX11Renderer::Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen)
{
#ifdef USE_INIT_FLAG
	assert(initflag == false);
	initflag = true;
#endif

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
	D3D11_RASTERIZER_DESC shadowRasterDesc;

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
	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, // D3D11_CREATE_DEVICE_BGRA_SUPPORT: Direct3D 리소스와의 Direct2D 상호 운용성에 필요합니다. by MSDN
		&featureLevel,
		1,
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
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// Stencil opertations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
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
	defaultRasterDesc.AntialiasedLineEnable = false;
	defaultRasterDesc.CullMode = D3D11_CULL_FRONT;
	defaultRasterDesc.DepthBias = 0;
	defaultRasterDesc.DepthBiasClamp = 0.0f;
	defaultRasterDesc.DepthClipEnable = true;
	defaultRasterDesc.FillMode = D3D11_FILL_SOLID;
	defaultRasterDesc.FrontCounterClockwise = false;
	defaultRasterDesc.MultisampleEnable = false;
	defaultRasterDesc.ScissorEnable = false;
	defaultRasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&defaultRasterDesc, &directionalLightRasterState);
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

	// Setup the raster description which will determine how and what polygons will be drawn.
	shadowRasterDesc.AntialiasedLineEnable = false;
	shadowRasterDesc.CullMode = D3D11_CULL_BACK;
	shadowRasterDesc.DepthBias = 0;
	shadowRasterDesc.DepthBiasClamp = 0.0f;
	shadowRasterDesc.DepthClipEnable = true;
	shadowRasterDesc.FillMode = D3D11_FILL_SOLID;
	shadowRasterDesc.FrontCounterClockwise = false;
	shadowRasterDesc.MultisampleEnable = false;
	shadowRasterDesc.ScissorEnable = false;
	shadowRasterDesc.SlopeScaledDepthBias = 1.0f;
	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&shadowRasterDesc, &shadowRasterState);
	if (FAILED(result)) return false;

	// Setup the raster description which will determine how and what polygons will be drawn.
	shadowRasterDesc.AntialiasedLineEnable = false;
	shadowRasterDesc.CullMode = D3D11_CULL_BACK;
	shadowRasterDesc.DepthBias = 0;
	shadowRasterDesc.DepthBiasClamp = 0.0f;
	shadowRasterDesc.DepthClipEnable = false;
	shadowRasterDesc.FillMode = D3D11_FILL_SOLID;
	shadowRasterDesc.FrontCounterClockwise = false;
	shadowRasterDesc.MultisampleEnable = false;
	shadowRasterDesc.ScissorEnable = false;
	shadowRasterDesc.SlopeScaledDepthBias = 1.0f;
	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&shadowRasterDesc, &spriteRasterState);
	if (FAILED(result)) return false;

	// Set up the viewport for rendering.
	defaultViewPort.Width = (float)screenWidth;
	defaultViewPort.Height = (float)screenHeight;
	defaultViewPort.MinDepth = 0.0f;
	defaultViewPort.MaxDepth = 1.0f;
	defaultViewPort.TopLeftX = 0.0f;
	defaultViewPort.TopLeftY = 0.0f;
	// Create the viewport.
	mDeviceContext->RSSetViewports(1, &defaultViewPort);

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr)) return false;

	spriteBatch = new SpriteBatch(mDeviceContext);
	commonStates = new CommonStates(mDevice);

	ResourceManager::GetInstance().Initialize(mDevice);

	ConstantBufferManager::GetInstance().Initialize(mDevice, mDeviceContext);
	matrixVsConstBuffer = new ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>(mDevice);
	animationConstBuffer = new ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>(mDevice);
	instancingMatrixVsConstBuffer = new ConstantBuffer<InstancingMatrixBufferType, ShaderType::VertexShader>(mDevice);
	blendingAnimationVsConstBuffer = new ConstantBuffer<BlendingAnimationBufferType, ShaderType::VertexShader>(mDevice);

	matrixPsConstBuffer = new ConstantBuffer<MatrixBufferType, ShaderType::PixelShader>(mDevice);
	lightInfoConstBuffer = new ConstantBuffer<LightInfoBufferType, ShaderType::PixelShader>(mDevice);
	lightIndexConstBuffer = new ConstantBuffer<LightIndexBufferType, ShaderType::PixelShader>(mDevice);
	materialConstBuffer = new ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>(mDevice);
	objectIDPSConstBuffer = new ConstantBuffer<ObjectIDBufferType, ShaderType::PixelShader>(mDevice);

	screenConstBuffer = new ConstantBuffer<ScreenBufferType, ShaderType::VertexShaderAndPixelShader>(mDevice);
	lightMatrixConstBuffer = new ConstantBuffer<LightMatrixBufferType, ShaderType::VertexShaderAndPixelShader>(mDevice);
	instancingDataConstBuffer = new ConstantBuffer<InstancingDataBufferType, ShaderType::VertexShaderAndPixelShader>(mDevice);
	dataConstBuffer = new ConstantBuffer<DataBufferType, ShaderType::VertexShaderAndPixelShader>(mDevice);

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

	deferredBlendingObjectShader = new ZeldaShader(mDevice, L"DeferredBlendingAnimationObjectVS.cso", L"DeferredObjectPS.cso", L"");
	deferredObjectShader = new ZeldaShader(mDevice, L"DeferredObjectVS.cso", L"DeferredObjectPS.cso", L"DeferredInstancingObjectVS.cso");
	deferredDirectionalLightShader = new ZeldaShader(mDevice, L"DeferredDirectionalLightVS.cso", L"DeferredDirectionalLightPS.cso", L"", Shader::Type::Shadow);
	deferredPointLightShader = new ZeldaShader(mDevice, L"DeferredPointLightVS.cso", L"DeferredPointLightPS.cso");
	deferredSpotLightShader = new ZeldaShader(mDevice, L"DeferredSpotLightVS.cso", L"DeferredSpotLightPS.cso");
	deferredFinalShader = new ZeldaShader(mDevice, L"DeferredFinalVS.cso", L"DeferredFinalPS.cso");
	forwardBlendingShader = new ZeldaShader(mDevice, L"ForwardBlendingAnimationVS.cso", L"ForwardPS.cso", L"");
	forwardShader = new ZeldaShader(mDevice, L"ForwardVS.cso", L"ForwardPS.cso", L"ForwardInstancingVS.cso");
	cubeMapShader = new ZeldaShader(mDevice, L"CubeMapVS.cso", L"CubeMapPS.cso", L"");
	shadowMapShader = new ZeldaShader(mDevice, L"DirectionalShadowMapVS.cso", L"DirectionalShadowMapPS.cso", L"DirectionalShadowMapInstancingVS.cso");
	blendingAnimationShadowMapShader = new ZeldaShader(mDevice, L"DirectionalShadowMapBlendingAnimationVS.cso", L"DirectionalShadowMapPS.cso", L"");
	spriteShader = new ZeldaShader(mDevice, L"SpriteVS.cso", L"SpritePS.cso", L"SpriteInstancingVS.cso");
	alphaSpriteShader = new ZeldaShader(mDevice, L"SpriteVS.cso", L"AlphaSpritePS.cso", L"SpriteInstancingVS.cso");
	fastOutLineShader = new ZeldaShader(mDevice, L"FastOutLineVS.cso", L"FastOutLinePS.cso", L"FastOutLineVS.cso");
	outLineShader = new ZeldaShader(mDevice, L"OutLineVS.cso", L"OutLinePS.cso", L"");
	outLineObjectShader = new ZeldaShader(mDevice, L"OutLineObjectVS.cso", L"OutLineObjectPS.cso", L"");
	outLineBlendingAnimationObjectShader = new ZeldaShader(mDevice, L"OutLineBlendingAnimationObjectVS.cso", L"OutLineObjectPS.cso", L"");

#pragma endregion Deferred Rendering


#pragma region Shadow Mapping

	// Texture 생성
	D3D11_TEXTURE2D_DESC shadowTextureDesc;
	ZeroMemory(&shadowTextureDesc, sizeof(shadowTextureDesc));

	shadowTextureDesc.Width = ShadowMap::Size;
	shadowTextureDesc.Height = ShadowMap::Size;
	shadowTextureDesc.MipLevels = 1u;
	shadowTextureDesc.ArraySize = 1u;
	shadowTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowTextureDesc.SampleDesc.Count = 1u;
	shadowTextureDesc.SampleDesc.Quality = 0u;
	shadowTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	shadowTextureDesc.CPUAccessFlags = 0;
	shadowTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* shadowTexture[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	for (int i = 0; i < 6; i++)
	{
		result = mDevice->CreateTexture2D(&shadowTextureDesc, NULL, &shadowTexture[i]);
		if (FAILED(result)) return false;
	}

	// Depth Stencil View 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDepthStencilViewDesc;
	shadowDepthStencilViewDesc.Flags = 0;
	shadowDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	shadowDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDepthStencilViewDesc.Texture2D.MipSlice = 0;

	for (int i = 0; i < 6; i++)
	{
		result = mDevice->CreateDepthStencilView(shadowTexture[i], &shadowDepthStencilViewDesc, &shadowDepthStencilView[i]);
		if (FAILED(result)) return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shadowShaderResourceViewDesc;
	shadowShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shadowShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadowShaderResourceViewDesc.Texture2D.MipLevels = shadowTextureDesc.MipLevels;
	shadowShaderResourceViewDesc.Texture2D.MostDetailedMip = 0u;

	for (int i = 0; i < 6; i++)
	{
		result = mDevice->CreateShaderResourceView(shadowTexture[i], &shadowShaderResourceViewDesc, &shadowShaderResource[i]);
		if (FAILED(result)) return false;
	}

	for (int i = 0; i < 6; i++)
	{
		if (shadowTexture[i])
		{
			shadowTexture[i]->Release();
			shadowTexture[i] = nullptr;
		}
	}

	{
		// 깊이 스텐실 스테이트 생성
		D3D11_DEPTH_STENCIL_DESC directionalShadowDepthStencilDesc = {};
		directionalShadowDepthStencilDesc.DepthEnable = TRUE;
		directionalShadowDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		directionalShadowDepthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
		directionalShadowDepthStencilDesc.StencilEnable = FALSE; // 스텐실 테스트를 비활성화합니다.

		mDevice->CreateDepthStencilState(&directionalShadowDepthStencilDesc, &directionalShadowDepthStencilState);
	}

#pragma endregion Shadow Mapping


#pragma region outline

	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = screenWidth;
		textureDesc.Height = screenHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_UINT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		ID3D11Texture2D* renderTargetTextures;

		// Texture2D 생성
		result = mDevice->CreateTexture2D(&textureDesc, NULL, &renderTargetTextures);
		if (FAILED(result)) return false;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Render Target 생성
		result = mDevice->CreateRenderTargetView(renderTargetTextures, &renderTargetViewDesc, &idMapRenderTarget);
		if (FAILED(result)) return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Shader Resource View 생성
		result = mDevice->CreateShaderResourceView(renderTargetTextures, &shaderResourceViewDesc, &idMapShaderResource);
		if (FAILED(result)) return false;

		renderTargetTextures->Release();
	}

	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = screenWidth;
		textureDesc.Height = screenHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_UINT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		ID3D11Texture2D* renderTargetTextures;

		// Texture2D 생성
		result = mDevice->CreateTexture2D(&textureDesc, NULL, &renderTargetTextures);
		if (FAILED(result)) return false;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Render Target 생성
		result = mDevice->CreateRenderTargetView(renderTargetTextures, &renderTargetViewDesc, &outlineMapRenderTarget);
		if (FAILED(result)) return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Shader Resource View 생성
		result = mDevice->CreateShaderResourceView(renderTargetTextures, &shaderResourceViewDesc, &outlineMapShaderResource);
		if (FAILED(result)) return false;

		renderTargetTextures->Release();
	}

#pragma endregion outline

	{
		// 깊이 스텐실 스테이트 생성
		D3D11_DEPTH_STENCIL_DESC cubeMapDepthStencilDesc = {};
		cubeMapDepthStencilDesc.DepthEnable = TRUE; // 깊이 테스트를 활성화합니다.
		cubeMapDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 깊이 버퍼에 쓰기를 끕니다.
		cubeMapDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // 깊이 테스트 함수를 설정합니다. 여기서는 깊이가 작은 값일 때만 통과하도록 설정했습니다.
		cubeMapDepthStencilDesc.StencilEnable = FALSE; // 스텐실 테스트를 비활성화합니다.

		mDevice->CreateDepthStencilState(&cubeMapDepthStencilDesc, &particleDepthStencilState);
	}

	{
		// 깊이 스텐실 스테이트 생성
		D3D11_DEPTH_STENCIL_DESC cubeMapDepthStencilDesc = {};
		cubeMapDepthStencilDesc.DepthEnable = TRUE; // 깊이 테스트를 활성화합니다.
		cubeMapDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 버퍼에 쓰기를 허용합니다.
		cubeMapDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // 깊이 테스트 함수를 설정합니다. 여기서는 깊이가 작은 값일 때만 통과하도록 설정했습니다.
		cubeMapDepthStencilDesc.StencilEnable = FALSE; // 스텐실 테스트를 비활성화합니다.

		mDevice->CreateDepthStencilState(&cubeMapDepthStencilDesc, &cubeMapDepthStencilState);
	}

	// 블렌드 상태 정의
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
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
	}

	{
		// 블렌드 상태 정의
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = TRUE; // 블렌딩을 활성화
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 소스 색상의 알파 값 사용
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 대상 색상의 알파 값 사용
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 블렌딩 연산: 소스 색상 * 소스 알파 + (1 - 소스 알파) * 대상 색상
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		// 블렌드 스테이트 생성
		result = mDevice->CreateBlendState(&blendDesc, &spriteBlendState);
		if (FAILED(result)) return false;
	}

	debugMode = DebugMode::Normal;
	debugModeBuffer = DebugMode::Normal;

	rendererMode = RendererMode::None;
	rendererModeBuffer = RendererMode::None;

	// D2D 초기화
	result = InitializeD2D();
	if (FAILED(result)) return false;

	CreateBasicResources();

	return true;
}

void ZeldaDX11Renderer::Finalize()
{
#ifdef USE_INIT_FLAG
	assert(initflag == true);
	initflag = false;
#endif

	ReleaseBasicResources();

	FinalizeD2D();

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

	if (directionalShadowDepthStencilState)
	{
		directionalShadowDepthStencilState->Release();
		directionalShadowDepthStencilState = nullptr;
	}
	if (alphaBlendState)
	{
		alphaBlendState->Release();
		alphaBlendState = nullptr;
	}
	if (spriteBlendState)
	{
		spriteBlendState->Release();
		spriteBlendState = nullptr;
	}
	if (particleDepthStencilState)
	{
		particleDepthStencilState->Release();
		particleDepthStencilState = nullptr;
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
	if (blendingAnimationVsConstBuffer)
	{
		delete blendingAnimationVsConstBuffer;
		blendingAnimationVsConstBuffer = nullptr;
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
	if (lightMatrixConstBuffer)
	{
		delete lightMatrixConstBuffer;
		lightMatrixConstBuffer = nullptr;
	}
	if (instancingDataConstBuffer)
	{
		delete instancingDataConstBuffer;
		instancingDataConstBuffer = nullptr;
	}
	if (dataConstBuffer)
	{
		delete dataConstBuffer;
		dataConstBuffer = nullptr;
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
	if (deferredBlendingObjectShader)
	{
		delete deferredBlendingObjectShader;
		deferredBlendingObjectShader = nullptr;
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
	if (forwardBlendingShader)
	{
		delete forwardBlendingShader;
		forwardBlendingShader = nullptr;
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
	if (shadowMapShader)
	{
		delete shadowMapShader;
		shadowMapShader = nullptr;
	}
	if (blendingAnimationShadowMapShader)
	{
		delete blendingAnimationShadowMapShader;
		blendingAnimationShadowMapShader = nullptr;
	}
	if (spriteShader)
	{
		delete spriteShader;
		spriteShader = nullptr;
	}
	if (alphaSpriteShader)
	{
		delete alphaSpriteShader;
		alphaSpriteShader = nullptr;
	}
	if (fastOutLineShader)
	{
		delete fastOutLineShader;
		fastOutLineShader = nullptr;
	}
	if (outLineShader)
	{
		delete outLineShader;
		outLineShader = nullptr;
	}
	if (outLineObjectShader)
	{
		delete outLineObjectShader;
		outLineObjectShader = nullptr;
	}
	if (outLineBlendingAnimationObjectShader)
	{
		delete outLineBlendingAnimationObjectShader;
		outLineBlendingAnimationObjectShader = nullptr;
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
	if (directionalLightRasterState)
	{
		directionalLightRasterState->Release();
		directionalLightRasterState = nullptr;
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
	if (shadowRasterState)
	{
		shadowRasterState->Release();
		shadowRasterState = nullptr;
	}
	if (spriteRasterState)
	{
		spriteRasterState->Release();
		spriteRasterState = nullptr;
	}
	for (int i = 0; i < 6; i++)
	{
		if (shadowDepthStencilView[i])
		{
			shadowDepthStencilView[i]->Release();
			shadowDepthStencilView[i] = nullptr;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (shadowShaderResource[i])
		{
			shadowShaderResource[i]->Release();
			shadowShaderResource[i] = nullptr;
		}
	}
	if (idMapRenderTarget)
	{
		idMapRenderTarget->Release();
		idMapRenderTarget = nullptr;
	}
	if (idMapShaderResource)
	{
		idMapShaderResource->Release();
		idMapShaderResource = nullptr;
	}
	if (outlineMapRenderTarget)
	{
		outlineMapRenderTarget->Release();
		outlineMapRenderTarget = nullptr;
	}
	if (outlineMapShaderResource)
	{
		outlineMapShaderResource->Release();
		outlineMapShaderResource = nullptr;
	}
	if (objectIDPSConstBuffer)
	{
		delete objectIDPSConstBuffer;
		objectIDPSConstBuffer = nullptr;
	}
	if (currentRasterState)
	{
		// 얘는 defaultRasterState, wireFrameRasterState를 받아두기만 하기 때문에 해제 필요없음
		currentRasterState = nullptr;
	}

	ResourceManager::GetInstance().Finalize();
}

void ZeldaDX11Renderer::Resize(unsigned int screenWidth, unsigned int screenHeight)
{
	// D2D관련된 것들을 다시 만들어야 할 수도 있다.

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	HRESULT hr = 0;
	HRESULT result = 0;

	// 1. Clear the existing references to the backbuffer
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	mDeviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		if (deferredRenderTargets[i] != nullptr)		deferredRenderTargets[i]->Release();
		if (deferredShaderResources[i] != nullptr)		deferredShaderResources[i]->Release();
	}

	if (mRenderTargetView != nullptr)						mRenderTargetView->Release();
	if (mDepthStencilView != nullptr)						mDepthStencilView->Release();

	mDeviceContext->Flush();

	// 2. Resize the existing swapchain
	hr = mSwapChain->ResizeBuffers(2, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		// You have to destroy the device, swapchain, and all resources and
		// recreate them to recover from this case. The device was hardware reset,
		// physically removed, or the driver was updated and/or restarted
	}

	// 3. Get the new backbuffer texture to use as a render target
	ID3D11Texture2D* backBufferPtr;

	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

	hr = mDevice->CreateRenderTargetView(backBufferPtr, nullptr, &mRenderTargetView);

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	ID3D11Texture2D* depthStencil;

	// 4. Create a depth/stencil buffer and create the depth stencil view
	CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, screenWidth, screenHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
	hr = mDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	hr = mDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &mDepthStencilView);

	// 5. Reset the rendering viewport to the new size
	CD3D11_VIEWPORT viewPort(0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight));
	mDeviceContext->RSSetViewports(1, &viewPort);

	// 6. Reset your camera's aspect ratio based on backBufferWidth/backBufferHeight






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
	}

	//Release render target texture array
	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		renderTargetTextures[i]->Release();
	}




	// 7. Set your render target view/depth stencil view for rendering
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void ZeldaDX11Renderer::SetExtraInitOption(float shadowAreaRange, float shadowAreaOffset, unsigned int shadowMapSize)
{
#ifdef USE_INIT_FLAG
	// 이 함수는 Initialize 이후에 호출하면 예상치 못한 오류를 일으킬 수 있다.
	assert(initflag == false);
#endif

	ShadowMap::Range = shadowAreaRange;
	ShadowMap::Offset = shadowAreaOffset;
	ShadowMap::Size = shadowMapSize;
}

void ZeldaDX11Renderer::SetExtraOption(float shadowMapDepthBias, float pointLightDepthBias)
{
	ShadowMap::DepthBias = shadowMapDepthBias;
	ShadowMap::PointLightDepthBias = pointLightDepthBias;
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

	RenderInfoManager::GetInstance().ClearRenderInfo();

	particleCount = 0u;
}

void ZeldaDX11Renderer::EndDraw()
{
#ifdef USE_BEGIN_FLAG
	// 이미 EndDraw를 호출함
	assert(beginflag == true);
	beginflag = false;
#endif

	RenderInfoManager::GetInstance().SortRenderInfo(ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera())->GetViewMatrix());

	float gray[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float idInit[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < Deferred::BufferCount; i++)
	{
		mDeviceContext->ClearRenderTargetView(deferredRenderTargets[i], black);
	}

	// Clear the back buffer.
	bool existLight = RenderInfoManager::GetInstance().GetLightRenderInfo().size() != 0;
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, (existLight) ? (gray) : (black));

	// Clear the depth buffer.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	// outline
	mDeviceContext->ClearRenderTargetView(idMapRenderTarget, idInit);

	DrawDeferred();

	DrawForward();

	DrawCubeMapRenderInfo();

	DrawBillBoardRenderInfo();
	DrawBillBoardParticleRenderInfo();

	DrawSpriteRenderInfo();
	DrawImage();

	DrawDebugInfo();

	DrawStringRenderInfo();

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

void ZeldaDX11Renderer::DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color color, Color outLineColor)
{
	MeshID cubeID = ResourceManager::GetInstance().GetCubeID();

	// Render Type
	RenderType renderType = RenderType::Deferred_Mesh;
	if ((wireFrame || CheckRendererMode(RendererMode::OnWireFrameMode)))
	{
		renderType = RenderType::Forward_Mesh;
	}

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;
	if (outLine)				renderOption |= RenderInfoOption::OutLine;
	if (fastOutLine)			renderOption |= RenderInfoOption::FastOutLine;
	if (drawDirectionalShadow)	renderOption |= RenderInfoOption::DirectionalLightShadow;
	if (drawPointShadow)		renderOption |= RenderInfoOption::PointLightShadow;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.meshID = cubeID;
	instancingKey.textureID = texture;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	instancingValue.color = color;
	instancingValue.outLineColor = outLineColor;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color outLineColor)
{
	// Render Type
	RenderType renderType = RenderType::Deferred_Model;
	if ((wireFrame || CheckRendererMode(RendererMode::OnWireFrameMode)))
	{
		renderType = RenderType::Forward_Model;
	}

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;
	if (outLine)				renderOption |= RenderInfoOption::OutLine;
	if (fastOutLine)			renderOption |= RenderInfoOption::FastOutLine;
	if (drawDirectionalShadow)	renderOption |= RenderInfoOption::DirectionalLightShadow;
	if (drawPointShadow)		renderOption |= RenderInfoOption::PointLightShadow;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.modelID = model;
	instancingKey.animationID = 0;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	instancingValue.animationTime = 0.0f;
	instancingValue.outLineColor = outLineColor;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color outLineColor)
{
	ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(model);

	// Render Type
	RenderType renderType = RenderType::Deferred_Model;
	if ((wireFrame || CheckRendererMode(RendererMode::OnWireFrameMode)))
	{
		renderType = RenderType::Forward_Model;
	}

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;
	if (outLine)				renderOption |= RenderInfoOption::OutLine;
	if (fastOutLine)			renderOption |= RenderInfoOption::FastOutLine;
	if (drawDirectionalShadow)	renderOption |= RenderInfoOption::DirectionalLightShadow;
	if (drawPointShadow)		renderOption |= RenderInfoOption::PointLightShadow;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.modelID = model;
	instancingKey.animationID = modelInstance->GetAnimationID(animationName);

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	instancingValue.animationTime = animationTime;
	instancingValue.outLineColor = outLineColor;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, const std::wstring& firstAnimationName, const std::wstring& secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color outLineColor)
{
	ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(model);

	// Render Type
	RenderType renderType = RenderType::Deferred_BlendingAnimation;
	if ((wireFrame || CheckRendererMode(RendererMode::OnWireFrameMode)))
	{
		renderType = RenderType::Forward_BlendingAnimation;
	}

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;
	if (outLine)				renderOption |= RenderInfoOption::OutLine;
	if (fastOutLine)			renderOption |= RenderInfoOption::FastOutLine;
	if (drawDirectionalShadow)	renderOption |= RenderInfoOption::DirectionalLightShadow;
	if (drawPointShadow)		renderOption |= RenderInfoOption::PointLightShadow;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.modelID = model;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	instancingValue.blendAnimationID1 = modelInstance->GetAnimationID(firstAnimationName);
	instancingValue.blendAnimationID2 = modelInstance->GetAnimationID(secondAnimationName);
	instancingValue.blendAnimationTime1 = firstAnimationTime;
	instancingValue.blendAnimationTime2 = secondAnimationTime;
	instancingValue.ratio = ratio;
	instancingValue.outLineColor = outLineColor;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawLight(LightID lightID)
{
	// Render Type
	RenderType renderType = RenderType::Light;

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.lightID = lightID;

	// Instancing Value
	InstancingValue instancingValue;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawImage(const Eigen::Vector2f& position, TextureID texture, Color color, unsigned int layer)
{
	DrawImage(position, { 0.0f, 0.0f }, texture, color, layer);
}

void ZeldaDX11Renderer::DrawImage(const Eigen::Vector2f& position, const Eigen::Vector2f& size, TextureID texture, Color color, unsigned int layer)
{
	// Render Type
	RenderType renderType = RenderType::Image;

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.textureID = texture;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.color = color;
	instancingValue.position = { position.x(), position.y() };
	instancingValue.size = { size.x(), size.y() };
	instancingValue.layer = layer;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawBillBoardParticle(const Eigen::Matrix4f& worldMatrix, const std::vector<Eigen::Matrix4f>& particleMatrix, unsigned int layer, TextureID texture, float ccwRadianAngle, bool keepOriginSize, bool useAlphaTexture, const std::vector<Color>& colors)
{
	MeshID meshID = ResourceManager::GetInstance().GetSquareID();
	ZeldaTexture* textureInstance = ResourceManager::GetInstance().GetTexture(texture);

	// Render Type
	RenderType renderType = RenderType::BillBoardParticle;

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.meshID = meshID;
	instancingKey.textureID = texture;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	instancingValue.layer = layer;
	instancingValue.ccwRadianAngle = ccwRadianAngle;
	instancingValue.useAlphaTexture = useAlphaTexture;
	instancingValue.particleID = particleCount;

	assert(particleMatrix.size() == colors.size());

	for (int i = 0; i < particleMatrix.size(); i++)
	{
		instancingValue.color = colors[i];

		if (keepOriginSize)
		{
			DirectX::XMMATRIX sizeMatrix = DirectX::XMMatrixScaling(static_cast<float>(textureInstance->GetWidth()) / Texture::UnitSize, static_cast<float>(textureInstance->GetHeight()) / Texture::UnitSize, 1.0f);
			DirectX::XMMATRIX matrixSRT = sizeMatrix * MathConverter::EigenMatrixToXMMatrix(particleMatrix[i]);

			DirectX::XMVECTOR s;
			DirectX::XMVECTOR r;
			DirectX::XMVECTOR t;
			DirectX::XMMatrixDecompose(&s, &r, &t, matrixSRT);

			// 크기를 조정하고 회전하지 않은 상태의 행렬로 가공하여 저장한다.
			instancingValue.particleMatrix = DirectX::XMMatrixScalingFromVector(s) * DirectX::XMMatrixTranslationFromVector(t);
		}
		else
		{
			DirectX::XMMATRIX matrixSRT = MathConverter::EigenMatrixToXMMatrix(particleMatrix[i]);

			DirectX::XMVECTOR s;
			DirectX::XMVECTOR r;
			DirectX::XMVECTOR t;
			DirectX::XMMatrixDecompose(&s, &r, &t, matrixSRT);

			// 회전하지 않은 상태의 행렬로 가공하여 저장한다.
			instancingValue.particleMatrix = DirectX::XMMatrixScalingFromVector(s) * DirectX::XMMatrixTranslationFromVector(t);
		}


		// Register
		RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
	}

	particleCount += 1u;
}

void ZeldaDX11Renderer::DrawBillBoard(const Eigen::Matrix4f& worldMatrix, TextureID texture, float ccwRadianAngle, bool keepOriginSize, bool useAlphaTexture, Color color)
{
	MeshID meshID = ResourceManager::GetInstance().GetSquareID();
	ZeldaTexture* textureInstance = ResourceManager::GetInstance().GetTexture(texture);

	// Render Type
	RenderType renderType = RenderType::BillBoard;

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.meshID = meshID;
	instancingKey.textureID = texture;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.ccwRadianAngle = ccwRadianAngle;
	instancingValue.useAlphaTexture = useAlphaTexture;
	instancingValue.color = color;

	if (keepOriginSize)
	{
		DirectX::XMMATRIX sizeMatrix = DirectX::XMMatrixScaling(static_cast<float>(textureInstance->GetWidth()) / Texture::UnitSize, static_cast<float>(textureInstance->GetHeight()) / Texture::UnitSize, 1.0f);
		DirectX::XMMATRIX matrixSRT = sizeMatrix * MathConverter::EigenMatrixToXMMatrix(worldMatrix);

		DirectX::XMVECTOR s;
		DirectX::XMVECTOR r;
		DirectX::XMVECTOR t;
		DirectX::XMMatrixDecompose(&s, &r, &t, matrixSRT);

		// 크기를 조정하고 회전하지 않은 상태의 행렬로 가공하여 저장한다.
		instancingValue.worldMatrix = DirectX::XMMatrixScalingFromVector(s) * DirectX::XMMatrixTranslationFromVector(t);
	}
	else
	{
		DirectX::XMMATRIX matrixSRT = MathConverter::EigenMatrixToXMMatrix(worldMatrix);

		DirectX::XMVECTOR s;
		DirectX::XMVECTOR r;
		DirectX::XMVECTOR t;
		DirectX::XMMatrixDecompose(&s, &r, &t, matrixSRT);

		// 회전하지 않은 상태의 행렬로 가공하여 저장한다.
		instancingValue.worldMatrix = DirectX::XMMatrixScalingFromVector(s) * DirectX::XMMatrixTranslationFromVector(t);
	}

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawSprite(const Eigen::Matrix4f& worldMatrix, TextureID texture, Color color, bool keepOriginSize)
{
	MeshID meshID = ResourceManager::GetInstance().GetSquareID();
	ZeldaTexture* textureInstance = ResourceManager::GetInstance().GetTexture(texture);

	// Render Type
	RenderType renderType = RenderType::Sprite;

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.meshID = meshID;
	instancingKey.textureID = texture;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.color = color;

	if (keepOriginSize)
	{
		DirectX::XMMATRIX sizeMatrix = DirectX::XMMatrixScaling(static_cast<float>(textureInstance->GetWidth()) / Texture::UnitSize, static_cast<float>(textureInstance->GetHeight()) / Texture::UnitSize, 1.0f);
		DirectX::XMMATRIX matrixSRT = sizeMatrix * MathConverter::EigenMatrixToXMMatrix(worldMatrix);

		// 크기를 조정하여 저장한다.
		instancingValue.worldMatrix = matrixSRT;
	}
	else
	{
		instancingValue.worldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);
	}

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawCubeMap(TextureID texture)
{
	// Render Type
	RenderType renderType = RenderType::CubeMap;

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;

	// Instancing Key
	InstancingKey instancingKey;
	instancingKey.textureID = texture;

	// Instancing Value
	InstancingValue instancingValue;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
}

void ZeldaDX11Renderer::DrawString(const std::wstring& string, float x, float y, float width, float height, float fontSize, Color color)
{
	// Render Type
	RenderType renderType = RenderType::String;

	// Render Option
	RenderOption renderOption = RenderInfoOption::None;

	// Instancing Key
	InstancingKey instancingKey;

	// Instancing Value
	InstancingValue instancingValue;
	instancingValue.str = string;
	instancingValue.position = { x, y };
	instancingValue.size = { width, height };
	instancingValue.fontSize = fontSize;
	instancingValue.color = color;

	// Register
	RenderInfoManager::GetInstance().RegisterRenderInfo(renderType, renderOption, instancingKey, instancingValue);
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
	ID3D11RenderTargetView* renderTargets[Deferred::Object::Count + 1] =
	{
		deferredRenderTargets[Deferred::Object::Begin + 0],
		deferredRenderTargets[Deferred::Object::Begin + 1],
		deferredRenderTargets[Deferred::Object::Begin + 2],
		deferredRenderTargets[Deferred::Object::Begin + 3],
		idMapRenderTarget
	};
	mDeviceContext->OMSetRenderTargets(Deferred::Object::Count + 1, renderTargets, mDepthStencilView);
	//mDeviceContext->OMSetRenderTargets(Deferred::Object::Count, deferredRenderTargets + Deferred::Object::Begin, mDepthStencilView);




	// Default Rasterizer State 사용
	mDeviceContext->RSSetState(defaultRasterState);

	DrawDeferredRenderInfo();

#pragma endregion


#pragma region Draw Deferred Light

	LightInfoBufferType lightInfoData;
	unsigned int lightCount = 0;
	for (auto& [key, value] : RenderInfoManager::GetInstance().GetLightRenderInfo())
	{
		lightInfoData.lights[lightCount] = ResourceManager::GetInstance().GetLight(key.lightID)->GetLightInfo();
		lightCount++;
	}

	lightInfoConstBuffer->SetData(lightInfoData);

	lightCount = 0;
	for (auto& [key, value] : RenderInfoManager::GetInstance().GetLightRenderInfo())
	{
		ZeldaLight* light = ResourceManager::GetInstance().GetLight(key.lightID);

		CreateShadowMap(light);
		DrawDeferredLight(light, lightCount);

		lightCount++;
	}

#pragma endregion


#pragma region Draw Deferred Final
	// 다음 그리기에 영향을 주지 않기 위해 뎁스스텐실을 비워놓음
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin + 0, 1, deferredShaderResources + Deferred::Object::Albedo);
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin + 1, 2, deferredShaderResources + Deferred::Light::Begin);

	ZeldaMesh* squareMesh = ResourceManager::GetInstance().GetSquareMesh();
	squareMesh->Render(mDeviceContext);
	deferredFinalShader->Render(mDeviceContext, squareMesh->GetIndexCount());

	mDeviceContext->PSSetShaderResources(Texture::Slot::IDMap, 1, &nullSRV[0]);
#pragma endregion

	DrawFastOutLine();
	DrawOutLine();
}

void ZeldaDX11Renderer::DrawForward()
{
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr, nullptr, nullptr };
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin, 3, nullSRV);
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetState(wireFrameRasterState);

	DrawForwardRenderInfo();

	mDeviceContext->RSSetState(defaultRasterState);
}

void ZeldaDX11Renderer::DrawDebugInfo()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);
	spriteBatch->Begin(SpriteSortMode_Immediate, commonStates->NonPremultiplied(), nullptr, mDepthStencilState, spriteRasterState);

	const static long wsize = 240;
	const static long hsize = 135;
	const static long space = 20;

	const static unsigned int DebugInfoCount = Deferred::BufferCount + 1;
	long fullsize = wsize * DebugInfoCount + max(DebugInfoCount - 1, 0) * space;

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

		// 임시 디버깅, 이 코드를 없앨 때 위의 DebugInfoCount에 +1을 한 것도 지울 것
		{
			int i = Deferred::BufferCount;
			spriteBatch->Draw(shadowShaderResource[0], RECT{((1920l - fullsize) / 2) + (i) * (wsize + space), 100, ((1920l - fullsize) / 2) + (i) * (wsize + space) + wsize, 100 + wsize});
		}
	}
#pragma endregion

	spriteBatch->End();

	ID3D11ShaderResourceView* nullSRV = nullptr;
	// Sprite Batch에서 사용하는 0번 슬롯의 리소스를 해제한다.
	mDeviceContext->PSSetShaderResources(0, 1, &nullSRV);
	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	mDeviceContext->RSSetState(defaultRasterState);
}

void ZeldaDX11Renderer::DrawSpriteRenderInfo()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetState(defaultRasterState);
	float alphaBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(spriteBlendState, alphaBlendFactor, 0xFFFFFFFF);

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	const auto& renderInfo = RenderInfoManager::GetInstance().GetSpriteRenderInfo();

	for (auto& [key, value] : renderInfo)
	{
		ZeldaMesh* meshInstance = ResourceManager::GetInstance().GetMesh(value->instancingKey.meshID);
		meshInstance->Render(mDeviceContext);

		ZeldaTexture* textureInstance = ResourceManager::GetInstance().GetTexture(value->instancingKey.textureID);
		textureInstance->SetDiffuseMapShaderResource(mDeviceContext);

		MatrixBufferType matrixBuffer;
		matrixBuffer.world = XMMatrixTranspose(value->instancingValue.worldMatrix);
		matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
		matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());
		matrixBuffer.cameraFar = currentcamera->GetFar();
		matrixVsConstBuffer->SetData(matrixBuffer);

		MaterialBufferType materialBufferType;
		materialBufferType.useSRGB = textureInstance->UseSRGB();
		materialBufferType.baseColor = DirectX::XMFLOAT4{ value->instancingValue.color.r, value->instancingValue.color.g, value->instancingValue.color.b, value->instancingValue.color.a };
		materialConstBuffer->SetData(materialBufferType);

		ConstantBufferManager::GetInstance().SetBuffer();

		spriteShader->Render(mDeviceContext, meshInstance->GetIndexCount());
	}

	ID3D11ShaderResourceView* nullSRV = nullptr;
	// Sprite Batch에서 사용하는 0번 슬롯의 리소스를 해제한다.
	mDeviceContext->PSSetShaderResources(0, 1, &nullSRV);
	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	mDeviceContext->RSSetState(defaultRasterState);
}

void ZeldaDX11Renderer::DrawImage()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetState(spriteRasterState);
	float alphaBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(spriteBlendState, alphaBlendFactor, 0xFFFFFFFF);

	const auto& imageRenderInfo = RenderInfoManager::GetInstance().GetImageRenderInfo();

	for (auto& [key, value] : imageRenderInfo)
	{
		DrawImageRenderInfo(value);
	}

	ID3D11ShaderResourceView* nullSRV = nullptr;
	mDeviceContext->PSSetShaderResources(0, 1, &nullSRV);
	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	mDeviceContext->RSSetState(defaultRasterState);
}

void ZeldaDX11Renderer::DrawBillBoardRenderInfo()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetState(defaultRasterState);
	float alphaBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(spriteBlendState, alphaBlendFactor, 0xFFFFFFFF);

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	const auto& renderInfo = RenderInfoManager::GetInstance().GetBillBoardRenderInfo();

	DirectX::XMVECTOR cameraS;
	DirectX::XMVECTOR cameraR;
	DirectX::XMVECTOR cameraT;
	DirectX::XMMatrixDecompose(&cameraS, &cameraR, &cameraT, currentcamera->GetTransformMatrix());

	for (auto& [depth, value] : renderInfo)
	{
		ZeldaMesh* meshInstance = ResourceManager::GetInstance().GetMesh(value->instancingKey.meshID);
		meshInstance->Render(mDeviceContext);

		ZeldaTexture* textureInstance = ResourceManager::GetInstance().GetTexture(value->instancingKey.textureID);
		textureInstance->SetDiffuseMapShaderResource(mDeviceContext);

		DirectX::XMVECTOR worldS;
		DirectX::XMVECTOR worldR;
		DirectX::XMVECTOR worldT;
		DirectX::XMMatrixDecompose(&worldS, &worldR, &worldT, value->instancingValue.worldMatrix);
		DirectX::XMMATRIX worldMS = XMMatrixScalingFromVector(worldS);
		DirectX::XMMATRIX worldMR = XMMatrixRotationQuaternion(worldR);
		DirectX::XMMATRIX worldMT = XMMatrixTranslationFromVector(worldT);

		MatrixBufferType matrixBuffer;
		// 2D회전을 한 후, 카메라가 바라보는 방향과 같은 방향으로 회전한 상태로 가공하여 저장한다.
		matrixBuffer.world = XMMatrixTranspose(worldMS * XMMatrixRotationZ(value->instancingValue.ccwRadianAngle) * XMMatrixRotationQuaternion(cameraR) * worldMT);
		matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
		matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());
		matrixBuffer.cameraFar = currentcamera->GetFar();
		matrixVsConstBuffer->SetData(matrixBuffer);

		MaterialBufferType materialBufferType;
		materialBufferType.baseColor = DirectX::XMFLOAT4{ value->instancingValue.color.r, value->instancingValue.color.g, value->instancingValue.color.b, value->instancingValue.color.a };
		materialBufferType.useSRGB = textureInstance->UseSRGB();
		materialConstBuffer->SetData(materialBufferType);

		ConstantBufferManager::GetInstance().SetBuffer();

		if (value->instancingValue.useAlphaTexture)
		{
			alphaSpriteShader->Render(mDeviceContext, meshInstance->GetIndexCount());
		}
		else
		{
			spriteShader->Render(mDeviceContext, meshInstance->GetIndexCount());
		}
	}

	ID3D11ShaderResourceView* nullSRV = nullptr;
	// Sprite Batch에서 사용하는 0번 슬롯의 리소스를 해제한다.
	mDeviceContext->PSSetShaderResources(0, 1, &nullSRV);
	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	mDeviceContext->RSSetState(defaultRasterState);
}

void ZeldaDX11Renderer::DrawBillBoardParticleRenderInfo()
{
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetState(defaultRasterState);
	float alphaBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(spriteBlendState, alphaBlendFactor, 0xFFFFFFFF);
	mDeviceContext->OMSetDepthStencilState(particleDepthStencilState, 1);

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	const auto& renderInfo = RenderInfoManager::GetInstance().GetBillBoardParticleRenderInfo();

	MatrixBufferType matrixBuffer;
	matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
	matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());
	matrixBuffer.cameraFar = currentcamera->GetFar();
	matrixVsConstBuffer->SetData(matrixBuffer);

	DirectX::XMVECTOR cameraS;
	DirectX::XMVECTOR cameraR;
	DirectX::XMVECTOR cameraT;
	DirectX::XMMatrixDecompose(&cameraS, &cameraR, &cameraT, currentcamera->GetTransformMatrix());

	InstancingMatrixBufferType* instancingMatrix = new InstancingMatrixBufferType();
	InstancingDataBufferType* instancingData = new InstancingDataBufferType();

	int instanceCount = 0;
	for (auto iter = renderInfo.begin(); iter != renderInfo.end(); iter++)
	{
		auto& key = iter->first;
		auto& value = iter->second;

		unsigned int particleID = key.second.second.second;

		ZeldaMesh* meshInstance = ResourceManager::GetInstance().GetMesh(value->instancingKey.meshID);
		meshInstance->RenderInstanced(mDeviceContext);

		ZeldaTexture* textureInstance = ResourceManager::GetInstance().GetTexture(value->instancingKey.textureID);
		textureInstance->SetDiffuseMapShaderResource(mDeviceContext);

		DirectX::XMVECTOR worldS;
		DirectX::XMVECTOR worldR;
		DirectX::XMVECTOR worldT;
		DirectX::XMMatrixDecompose(&worldS, &worldR, &worldT, value->instancingValue.particleMatrix);
		DirectX::XMMATRIX worldMS = XMMatrixScalingFromVector(worldS);
		DirectX::XMMATRIX worldMR = XMMatrixRotationQuaternion(worldR);
		DirectX::XMMATRIX worldMT = XMMatrixTranslationFromVector(worldT);

		// 2D회전을 한 후, 카메라가 바라보는 방향과 같은 방향으로 회전한 상태로 가공하여 저장한다.
		instancingMatrix->instancingWorldMatrix[instanceCount % INSTANCING_MAX] = XMMatrixTranspose(worldMS * XMMatrixRotationZ(value->instancingValue.ccwRadianAngle) * XMMatrixRotationQuaternion(cameraR) * worldMT);
		instancingData->instancingValue0[instanceCount % INSTANCING_MAX].x = value->instancingValue.color.r;
		instancingData->instancingValue0[instanceCount % INSTANCING_MAX].y = value->instancingValue.color.g;
		instancingData->instancingValue0[instanceCount % INSTANCING_MAX].z = value->instancingValue.color.b;
		instancingData->instancingValue0[instanceCount % INSTANCING_MAX].w = value->instancingValue.color.a;

		if (((instanceCount % INSTANCING_MAX) + 1 == INSTANCING_MAX) || (std::next(iter) == renderInfo.end()) || (std::next(iter)->first.second.second.second != particleID))
		{
			MaterialBufferType materialBufferType;
			if (value->instancingValue.useAlphaTexture)
			{
				materialBufferType.baseColor = DirectX::XMFLOAT4{ value->instancingValue.color.r, value->instancingValue.color.g, value->instancingValue.color.b, value->instancingValue.color.a };
			}
			else
			{
				materialBufferType.baseColor = DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
			}
			materialBufferType.useSRGB = textureInstance->UseSRGB();
			materialConstBuffer->SetData(materialBufferType);

			instancingDataConstBuffer->SetData(*instancingData);

			instancingMatrixVsConstBuffer->SetData(*instancingMatrix);
			ConstantBufferManager::GetInstance().SetBuffer();

			if (value->instancingValue.useAlphaTexture)
			{
				alphaSpriteShader->RenderInstanced(mDeviceContext, meshInstance->GetIndexCount(), (instanceCount % INSTANCING_MAX) + 1, 0);
			}
			else
			{
				spriteShader->RenderInstanced(mDeviceContext, meshInstance->GetIndexCount(), (instanceCount % INSTANCING_MAX) + 1, 0);
			}

			instanceCount = 0;
		}
		else
		{
			instanceCount += 1;
		}
	}

	delete instancingMatrix;
	delete instancingData;

	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
	ID3D11ShaderResourceView* nullSRV = nullptr;
	// Sprite Batch에서 사용하는 0번 슬롯의 리소스를 해제한다.
	mDeviceContext->PSSetShaderResources(0, 1, &nullSRV);
	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	mDeviceContext->RSSetState(defaultRasterState);
}

void ZeldaDX11Renderer::DrawDeferredRenderInfo()
{
	auto& renderInfo = RenderInfoManager::GetInstance().GetDeferredRenderInfo();

	for (auto& [key, value] : renderInfo)
	{
		assert(value.size() != 0u);
		RenderType renderType = value[0]->renderType;

		switch (renderType)
		{
			case RenderType::Deferred_Mesh:
			{
				DrawMeshRenderInfo(value, deferredObjectShader);
				break;
			}
			case RenderType::Deferred_Model:
			{
				DrawModelRenderInfo(value, deferredObjectShader);
				break;
			}
			case RenderType::Deferred_BlendingAnimation:
			{
				for (int i = 0; i < value.size(); i++)
				{
					DrawBlendingAnimationRenderInfo(value[i], deferredBlendingObjectShader);
				}
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}
}

void ZeldaDX11Renderer::DrawForwardRenderInfo()
{
	auto& renderInfo = RenderInfoManager::GetInstance().GetForwardRenderInfo();

	for (auto& [key, value] : renderInfo)
	{
		assert(value.size() != 0u);
		RenderType renderType = value[0]->renderType;

		switch (renderType)
		{
			case RenderType::Forward_Mesh:
			{
				DrawMeshRenderInfo(value, forwardShader);
				break;
			}
			case RenderType::Forward_Model:
			{
				DrawModelRenderInfo(value, forwardShader);
				break;
			}
			case RenderType::Forward_BlendingAnimation:
			{
				for (int i = 0; i < value.size(); i++)
				{
					DrawBlendingAnimationRenderInfo(value[i], forwardBlendingShader);
				}
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}
}

void ZeldaDX11Renderer::DrawFastOutLine()
{
	ID3D11ShaderResourceView* nullSRV = nullptr;

	// 다음 그리기에 영향을 주지 않기 위해 뎁스스텐실을 비워놓음
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);

	mDeviceContext->PSSetShaderResources(Texture::Slot::IDMap, 1, &idMapShaderResource);

	ZeldaMesh* squareMesh = ResourceManager::GetInstance().GetSquareMesh();
	squareMesh->RenderInstanced(mDeviceContext);

	const auto& fastOutLineRenderInfo = RenderInfoManager::GetInstance().GetFastOutLineRenderInfo();


	InstancingDataBufferType* instancingData = new InstancingDataBufferType();

	int instanceCount = 0;
	for (auto iter = fastOutLineRenderInfo.begin(); iter != fastOutLineRenderInfo.end(); iter++)
	{
		for (int i = 0; i < iter->second.size(); i++)
		{
			instancingData->instancingValue0[instanceCount % INSTANCING_MAX].x = iter->second[i]->instancingValue.outLineColor.r;
			instancingData->instancingValue0[instanceCount % INSTANCING_MAX].y = iter->second[i]->instancingValue.outLineColor.g;
			instancingData->instancingValue0[instanceCount % INSTANCING_MAX].z = iter->second[i]->instancingValue.outLineColor.b;
			instancingData->instancingValue0[instanceCount % INSTANCING_MAX].w = iter->second[i]->instancingValue.outLineColor.a;

			instancingData->instancingValue2[instanceCount % INSTANCING_MAX].x = iter->second[i]->drawID;

			// 인스턴싱 가능한 최대 갯수로 끊어서 그리기
			if (((instanceCount % INSTANCING_MAX) + 1 == INSTANCING_MAX) || ((std::next(iter) == fastOutLineRenderInfo.end()) && (instanceCount == iter->second.size() - 1)))
			{
				instancingDataConstBuffer->SetData(*instancingData);
				ConstantBufferManager::GetInstance().SetBuffer();
				fastOutLineShader->RenderInstanced(mDeviceContext, squareMesh->GetIndexCount(), (instanceCount % INSTANCING_MAX) + 1, 0);
			}

			instanceCount += 1;
		}
	}

	delete instancingData;

	mDeviceContext->PSSetShaderResources(Texture::Slot::IDMap, 1, &nullSRV);
}

void ZeldaDX11Renderer::DrawOutLine()
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ID3D11ShaderResourceView* nullSRV = nullptr;
	float initColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ZeldaMesh* squareMesh = ResourceManager::GetInstance().GetSquareMesh();
	squareMesh->RenderInstanced(mDeviceContext);

	const auto& outLineRenderInfo = RenderInfoManager::GetInstance().GetOutLineRenderInfo();

	for (int i = 0; i < outLineRenderInfo.size(); i++)
	{
		// 렌더타겟 초기화
		mDeviceContext->ClearRenderTargetView(outlineMapRenderTarget, initColor);

		// 단일 오브젝트를 그린다.
		mDeviceContext->OMSetRenderTargets(1, &outlineMapRenderTarget, nullptr);

		RenderType renderType = outLineRenderInfo[i]->renderType;

		switch (renderType)
		{
			case RenderType::Deferred_Mesh:
			{
				ZeldaMesh* meshInstance = ResourceManager::GetInstance().GetMesh(outLineRenderInfo[i]->instancingKey.meshID);
				meshInstance->Render(mDeviceContext);

				MatrixBufferType matrixBuffer;
				matrixBuffer.world = XMMatrixTranspose(outLineRenderInfo[i]->instancingValue.worldMatrix);
				matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
				matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());
				matrixBuffer.cameraFar = currentcamera->GetFar();
				matrixVsConstBuffer->SetData(matrixBuffer);

				ObjectIDBufferType objectIDBufferType;
				objectIDBufferType.objectID = outLineRenderInfo[i]->drawID;
				objectIDPSConstBuffer->SetData(objectIDBufferType);

				ConstantBufferManager::GetInstance().SetBuffer();

				outLineObjectShader->Render(mDeviceContext, meshInstance->GetIndexCount());

				break;
			}
			case RenderType::Deferred_Model:
			{
				ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(outLineRenderInfo[i]->instancingKey.modelID);

				modelInstance->Render(
					mDeviceContext,
					matrixVsConstBuffer,
					animationConstBuffer,
					materialConstBuffer,
					objectIDPSConstBuffer,
					outLineRenderInfo[i],
					outLineObjectShader);

				break;
			}
			case RenderType::Deferred_BlendingAnimation:
			{
				ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(outLineRenderInfo[i]->instancingKey.modelID);

				modelInstance->RenderBlendingAnimation(
					mDeviceContext,
					matrixVsConstBuffer,
					blendingAnimationVsConstBuffer,
					materialConstBuffer,
					objectIDPSConstBuffer,
					outLineRenderInfo[i],
					outLineBlendingAnimationObjectShader);

				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}

		// OutLine을 그린다.

		// 렌더타겟을 변경
		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);

		mDeviceContext->PSSetShaderResources(Texture::Slot::IDMap, 1, &idMapShaderResource);
		mDeviceContext->PSSetShaderResources(Texture::Slot::OutLineMap, 1, &outlineMapShaderResource);

		ZeldaMesh* squareMesh = ResourceManager::GetInstance().GetSquareMesh();
		squareMesh->Render(mDeviceContext);

		ObjectIDBufferType objectIDBufferType;
		objectIDBufferType.objectID = outLineRenderInfo[i]->drawID;
		objectIDPSConstBuffer->SetData(objectIDBufferType);

		DataBufferType dataBufferType;
		dataBufferType.float4Value[0].x = outLineRenderInfo[i]->instancingValue.outLineColor.r;
		dataBufferType.float4Value[0].y = outLineRenderInfo[i]->instancingValue.outLineColor.g;
		dataBufferType.float4Value[0].z = outLineRenderInfo[i]->instancingValue.outLineColor.b;
		dataBufferType.float4Value[0].w = outLineRenderInfo[i]->instancingValue.outLineColor.a;
		dataConstBuffer->SetData(dataBufferType);

		ConstantBufferManager::GetInstance().SetBuffer();

		outLineShader->Render(mDeviceContext, squareMesh->GetIndexCount());

		mDeviceContext->PSSetShaderResources(Texture::Slot::IDMap, 1, &nullSRV);
		mDeviceContext->PSSetShaderResources(Texture::Slot::OutLineMap, 1, &nullSRV);
	}
}

void ZeldaDX11Renderer::DrawMeshRenderInfo(const std::vector<RenderInfo*>& renderInfo, ZeldaShader* shader)
{
	assert(renderInfo.size() != 0);

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// Mesh
	ZeldaMesh* meshInstance = ResourceManager::GetInstance().GetMesh(renderInfo[0]->instancingKey.meshID);

	// Texture
	ZeldaTexture* textureInstance = ResourceManager::GetInstance().GetTexture(renderInfo[0]->instancingKey.textureID);

	// Constant Buffer
	MatrixBufferType matrixBuffer;
	matrixBuffer.world = XMMatrixTranspose(renderInfo[0]->instancingValue.worldMatrix);
	matrixBuffer.view = XMMatrixTranspose(currentcamera->GetViewMatrix());
	matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetProjMatrix());
	matrixBuffer.cameraFar = currentcamera->GetFar();
	matrixVsConstBuffer->SetData(matrixBuffer);

	// Set Texture
	if (textureInstance != nullptr)
	{
		textureInstance->SetDiffuseMapShaderResource(mDeviceContext);
	}

	bool instancing = (renderInfo.size() > 1u);

	if (instancing)
	{
		MaterialBufferType materialBufferType;
		materialBufferType.baseColor =
		{
			renderInfo[0]->instancingValue.color.r,
			renderInfo[0]->instancingValue.color.g,
			renderInfo[0]->instancingValue.color.b,
			renderInfo[0]->instancingValue.color.a
		};
		materialBufferType.useBaseColor = (textureInstance == nullptr);
		materialBufferType.useSRGB = (textureInstance != nullptr) && textureInstance->UseSRGB();
		materialBufferType.useDiffuse = (textureInstance != nullptr);
		materialBufferType.useNormal = false;
		materialBufferType.useInstancingColor = true;
		materialConstBuffer->SetData(materialBufferType);

		meshInstance->RenderInstanced(mDeviceContext);

		InstancingMatrixBufferType* instancingMatrix = new InstancingMatrixBufferType();
		InstancingDataBufferType* instancingData = new InstancingDataBufferType();

		for (size_t i = 0ull; i < renderInfo.size(); i++)
		{
			// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
			instancingMatrix->instancingWorldMatrix[i % INSTANCING_MAX] = XMMatrixTranspose(renderInfo[i]->instancingValue.worldMatrix);
			instancingData->instancingValue1[i % INSTANCING_MAX] =
			{
				renderInfo[i]->instancingValue.color.r,
				renderInfo[i]->instancingValue.color.g,
				renderInfo[i]->instancingValue.color.b,
				renderInfo[i]->instancingValue.color.a
			};

			// 인스턴싱 가능한 최대 갯수로 끊어서 그리기
			if (((i % INSTANCING_MAX) + 1 == INSTANCING_MAX) || (i == renderInfo.size() - 1))
			{
				instancingMatrixVsConstBuffer->SetData(*instancingMatrix);
				instancingDataConstBuffer->SetData(*instancingData);

				ObjectIDBufferType objectIDBufferType;
				objectIDBufferType.objectID = renderInfo[i]->drawID - (i % INSTANCING_MAX);
				objectIDPSConstBuffer->SetData(objectIDBufferType);

				ConstantBufferManager::GetInstance().SetBuffer();

				shader->RenderInstanced(mDeviceContext, meshInstance->GetIndexCount(), (i % INSTANCING_MAX) + 1, 0);
			}
		}

		delete instancingMatrix;
		delete instancingData;
	}
	else
	{
		MaterialBufferType materialBufferType;
		materialBufferType.baseColor =
		{
			renderInfo[0]->instancingValue.color.r,
			renderInfo[0]->instancingValue.color.g,
			renderInfo[0]->instancingValue.color.b,
			renderInfo[0]->instancingValue.color.a
		};
		materialBufferType.useBaseColor = (textureInstance == nullptr);
		materialBufferType.useSRGB = (textureInstance != nullptr) && textureInstance->UseSRGB();
		materialBufferType.useDiffuse = (textureInstance != nullptr);
		materialBufferType.useNormal = false;
		materialBufferType.useInstancingColor = false;
		materialConstBuffer->SetData(materialBufferType);

		meshInstance->Render(mDeviceContext);

		ObjectIDBufferType objectIDBufferType;
		objectIDBufferType.objectID = renderInfo[0]->drawID;
		objectIDPSConstBuffer->SetData(objectIDBufferType);

		ConstantBufferManager::GetInstance().SetBuffer();

		shader->Render(mDeviceContext, meshInstance->GetIndexCount());
	}
}

void ZeldaDX11Renderer::DrawModelRenderInfo(const std::vector<RenderInfo*>& renderInfo, ZeldaShader* shader)
{
	assert(renderInfo.size() != 0);

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// Model
	ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(renderInfo[0]->instancingKey.modelID);

	bool instancing = (renderInfo.size() > 1u);

	if (instancing)
	{
		modelInstance->RenderInstanced(
			mDeviceContext,
			matrixVsConstBuffer,
			instancingMatrixVsConstBuffer,
			instancingDataConstBuffer,
			materialConstBuffer,
			objectIDPSConstBuffer,
			renderInfo,
			shader);
	}
	else
	{
		modelInstance->Render(
			mDeviceContext,
			matrixVsConstBuffer,
			animationConstBuffer,
			materialConstBuffer,
			objectIDPSConstBuffer,
			renderInfo[0],
			shader);
	}
}

void ZeldaDX11Renderer::DrawBlendingAnimationRenderInfo(RenderInfo* renderInfo, ZeldaShader* shader)
{
	assert(renderInfo != nullptr);

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// Model
	ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(renderInfo->instancingKey.modelID);

	modelInstance->RenderBlendingAnimation(
		mDeviceContext,
		matrixVsConstBuffer,
		blendingAnimationVsConstBuffer,
		materialConstBuffer,
		objectIDPSConstBuffer,
		renderInfo,
		shader);
}

void ZeldaDX11Renderer::DrawMeshShadow(const std::vector<RenderInfo*>& renderInfo, ZeldaLight* light, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjMatrix)
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// Mesh
	ZeldaMesh* meshInstance = ResourceManager::GetInstance().GetMesh(renderInfo[0]->instancingKey.meshID);

	// Constant Buffer
	LightMatrixBufferType lightMatrixBuffer;
	lightMatrixBuffer.view[0] = XMMatrixTranspose(lightViewMatrix);
	lightMatrixBuffer.projection = XMMatrixTranspose(lightProjMatrix);
	lightMatrixBuffer.shadowMapSize = static_cast<float>(ShadowMap::Size);
	lightMatrixBuffer.shadowMapDepthBias = ShadowMap::DepthBias;
	lightMatrixBuffer.pointLightDepthBias = ShadowMap::PointLightDepthBias;
	lightMatrixConstBuffer->SetData(lightMatrixBuffer);

	bool instancing = (renderInfo.size() > 1u);

	if (instancing)
	{
		meshInstance->RenderInstanced(mDeviceContext);

		InstancingMatrixBufferType* instancingMatrix = new InstancingMatrixBufferType();

		for (size_t i = 0; i < renderInfo.size(); i++)
		{
			auto& instancingInfo = renderInfo[i];

			// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
			instancingMatrix->instancingWorldMatrix[i % INSTANCING_MAX] = XMMatrixTranspose(renderInfo[i]->instancingValue.worldMatrix);

			// 인스턴싱 가능한 최대 갯수로 끊어서 그리기
			if (((i % INSTANCING_MAX) + 1 == INSTANCING_MAX) || (i == renderInfo.size() - 1))
			{
				instancingMatrixVsConstBuffer->SetData(*instancingMatrix);
				ConstantBufferManager::GetInstance().SetBuffer();

				shadowMapShader->RenderInstanced(mDeviceContext, meshInstance->GetIndexCount(), (i % INSTANCING_MAX) + 1, 0);
			}
		}

		delete instancingMatrix;
	}
	else
	{
		meshInstance->Render(mDeviceContext);

		// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
		matrixVsConstBuffer->SetData({ XMMatrixTranspose(renderInfo[0]->instancingValue.worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });

		ConstantBufferManager::GetInstance().SetBuffer();

		shadowMapShader->Render(mDeviceContext, meshInstance->GetIndexCount());
	}
}

void ZeldaDX11Renderer::DrawModelShadow(const std::vector<RenderInfo*>& renderInfo, ZeldaLight* light, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjMatrix)
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(renderInfo[0]->instancingKey.modelID);

	LightMatrixBufferType lightMatrixBuffer;
	lightMatrixBuffer.view[0] = XMMatrixTranspose(lightViewMatrix);
	lightMatrixBuffer.projection = XMMatrixTranspose(lightProjMatrix);
	lightMatrixBuffer.shadowMapSize = static_cast<float>(ShadowMap::Size);
	lightMatrixBuffer.shadowMapDepthBias = ShadowMap::DepthBias;
	lightMatrixBuffer.pointLightDepthBias = ShadowMap::PointLightDepthBias;
	lightMatrixConstBuffer->SetData(lightMatrixBuffer);

	bool instancing = (renderInfo.size() > 1u);

	if (instancing)
	{
		modelInstance->RenderInstanced(
			mDeviceContext,
			matrixVsConstBuffer,
			instancingMatrixVsConstBuffer,
			instancingDataConstBuffer,
			materialConstBuffer,
			objectIDPSConstBuffer,
			renderInfo,
			shadowMapShader);
	}
	else
	{
		modelInstance->Render(
			mDeviceContext,
			matrixVsConstBuffer,
			animationConstBuffer,
			materialConstBuffer,
			objectIDPSConstBuffer,
			renderInfo[0],
			shadowMapShader);
	}
}

void ZeldaDX11Renderer::DrawBlendingAnimationShadow(RenderInfo* renderInfo, ZeldaLight* light, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjMatrix)
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ZeldaModel* modelInstance = ResourceManager::GetInstance().GetModel(renderInfo->instancingKey.modelID);

	LightMatrixBufferType lightMatrixBuffer;
	lightMatrixBuffer.view[0] = XMMatrixTranspose(lightViewMatrix);
	lightMatrixBuffer.projection = XMMatrixTranspose(lightProjMatrix);
	lightMatrixBuffer.shadowMapSize = static_cast<float>(ShadowMap::Size);
	lightMatrixBuffer.shadowMapDepthBias = ShadowMap::DepthBias;
	lightMatrixBuffer.pointLightDepthBias = ShadowMap::PointLightDepthBias;
	lightMatrixConstBuffer->SetData(lightMatrixBuffer);

	modelInstance->RenderBlendingAnimation(
		mDeviceContext,
		matrixVsConstBuffer,
		blendingAnimationVsConstBuffer,
		materialConstBuffer,
		objectIDPSConstBuffer,
		renderInfo,
		blendingAnimationShadowMapShader);
}

void ZeldaDX11Renderer::DrawImageRenderInfo(RenderInfo* renderInfo)
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());
	ZeldaTexture* texture = ResourceManager::GetInstance().GetTexture(renderInfo->instancingKey.textureID);
	ZeldaMesh* meshInstance = ResourceManager::GetInstance().GetSquareMesh();

	if (renderInfo->instancingValue.size.x <= 0.0f)
	{
		renderInfo->instancingValue.size.x = texture->GetWidth();
	}

	if (renderInfo->instancingValue.size.y <= 0.0f)
	{
		renderInfo->instancingValue.size.y = texture->GetHeight();
	}

	float layer = static_cast<float>(renderInfo->instancingValue.layer);

	XMMATRIX scale = XMMatrixScaling(renderInfo->instancingValue.size.x, renderInfo->instancingValue.size.y, 1.0f);
	XMMATRIX translation = XMMatrixTranslation(renderInfo->instancingValue.size.x / 2.0f + renderInfo->instancingValue.position.x - static_cast<float>(screenWidth) / 2.0f, -renderInfo->instancingValue.size.y / 2.0f - renderInfo->instancingValue.position.y + static_cast<float>(screenHeight) / 2.0f, layer);

	MatrixBufferType matrixBuffer;
	matrixBuffer.world = XMMatrixTranspose(scale * translation);
	matrixBuffer.view = XMMatrixTranspose(XMMatrixIdentity());
	matrixBuffer.projection = XMMatrixTranspose(currentcamera->GetOrthoMatrix());
	matrixBuffer.cameraFar = currentcamera->GetFar();
	matrixVsConstBuffer->SetData(matrixBuffer);

	MaterialBufferType materialBufferType;
	materialBufferType.useSRGB = texture->UseSRGB();
	materialBufferType.baseColor = DirectX::XMFLOAT4{ renderInfo->instancingValue.color.r, renderInfo->instancingValue.color.g, renderInfo->instancingValue.color.b, renderInfo->instancingValue.color.a };
	materialConstBuffer->SetData(materialBufferType);

	ConstantBufferManager::GetInstance().SetBuffer();

	meshInstance->Render(mDeviceContext);
	texture->SetDiffuseMapShaderResource(mDeviceContext);

	spriteShader->Render(mDeviceContext, meshInstance->GetIndexCount());
}

void ZeldaDX11Renderer::CreateShadowMap(ZeldaLight* light)
{
	LightType lightType = light->GetLightType();

	switch (lightType)
	{
		case LightType::Directional:
		{
			// Rasterizer State 설정 - (지금은 셰이더에서 bias처리를 하기 때문에 defaultRasterState를 사용한다)
			mDeviceContext->RSSetState(directionalLightRasterState);
			// Shadow DepthStencilView 비우기
			mDeviceContext->ClearDepthStencilView(shadowDepthStencilView[0], D3D11_CLEAR_DEPTH, 1.0f, 0);
			// RenderTarget을 비우고 DepthStencilView를 설정
			mDeviceContext->OMSetRenderTargets(0, nullptr, shadowDepthStencilView[0]);
			// ViewPort 설정
			D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(ShadowMap::Size), static_cast<float>(ShadowMap::Size), 0.0f, 1.0f };
			mDeviceContext->RSSetViewports(1, &viewport);

			const auto& shadowRenderInfo = RenderInfoManager::GetInstance().GetDirectionalShadowRenderInfo();

			ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());
			DirectX::XMMATRIX lightViewMatrix = light->GetViewMatrix(currentcamera);
			DirectX::XMMATRIX lightProjMatrix = light->GetOrthoMatrix();

			for (auto& [key, value] : shadowRenderInfo)
			{
				assert(value.size() != 0);

				RenderType renderType = value[0]->renderType;


				// 그림자의 깨짐을 줄이기 위해 쉐도우맵을 만들 때 그리는 크기를 조정한다.
				constexpr float shadowSclae = 0.9f;
				constexpr float shadowSclaeInverse = 1.0f / shadowSclae;

				for (auto& renderInfo : value)
				{
					renderInfo->instancingValue.worldMatrix = DirectX::XMMatrixScaling(shadowSclae, shadowSclae, shadowSclae) * renderInfo->instancingValue.worldMatrix;
				}

				switch (renderType)
				{
					case RenderType::Deferred_Mesh:
					{
						DrawMeshShadow(value, light, lightViewMatrix, lightProjMatrix);
						break;
					}
					case RenderType::Deferred_Model:
					{
						DrawModelShadow(value, light, lightViewMatrix, lightProjMatrix);
						break;
					}
					case RenderType::Deferred_BlendingAnimation:
					{
						for (int i = 0; i < value.size(); i++)
						{
							DrawBlendingAnimationShadow(value[i], light, lightViewMatrix, lightProjMatrix);
						}
						break;
					}
					default:
					{
						assert(0);
						break;
					}
				}

				// Render Info를 원래대로 돌려놓는다. 이 작업을 하지 않으면 외곽선 그리기 등에서 크기가 바뀌어 나올 수 있다.
				for (auto& renderInfo : value)
				{
					renderInfo->instancingValue.worldMatrix = DirectX::XMMatrixScaling(shadowSclaeInverse, shadowSclaeInverse, shadowSclaeInverse) * renderInfo->instancingValue.worldMatrix;
				}
			}

			// 종료 후 ViewPort를 되돌려 놓는다.
			mDeviceContext->RSSetViewports(1, &defaultViewPort);
			// Rasterizer State를 되돌려 놓음
			mDeviceContext->RSSetState(defaultRasterState);

			break;
		}
		case LightType::Point:
		{
			DirectX::XMMATRIX lightViewMatrix[6];
			DirectX::XMMATRIX lightProjMatrix;

			light->CreatePointLightViewAndProjMatrices(lightViewMatrix, lightProjMatrix);

			// 6방향의 텍스쳐를 만들어야 한다.
			for (int dirNum = 0; dirNum < 6; dirNum++)
			{
				// Rasterizer State 설정 - (지금은 셰이더에서 bias처리를 하기 때문에 defaultRasterState를 사용한다)
				mDeviceContext->RSSetState(defaultRasterState);
				// Shadow DepthStencilView 비우기
				mDeviceContext->ClearDepthStencilView(shadowDepthStencilView[dirNum], D3D11_CLEAR_DEPTH, 1.0f, 0);
				// RenderTarget을 비우고 DepthStencilView를 설정
				mDeviceContext->OMSetRenderTargets(0, nullptr, shadowDepthStencilView[dirNum]);
				// ViewPort 설정
				D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(ShadowMap::Size), static_cast<float>(ShadowMap::Size), 0.0f, 1.0f };
				mDeviceContext->RSSetViewports(1, &viewport);

				const auto& shadowRenderInfo = RenderInfoManager::GetInstance().GetPointShadowRenderInfo();

				for (auto& [key, value] : shadowRenderInfo)
				{
					assert(value.size() != 0);

					RenderType renderType = value[0]->renderType;

					switch (renderType)
					{
						case RenderType::Deferred_Mesh:
						{
							DrawMeshShadow(value, light, lightViewMatrix[dirNum], lightProjMatrix);
							break;
						}
						case RenderType::Deferred_Model:
						{
							DrawModelShadow(value, light, lightViewMatrix[dirNum], lightProjMatrix);
							break;
						}
						case RenderType::Deferred_BlendingAnimation:
						{
							for (int i = 0; i < value.size(); i++)
							{
								DrawBlendingAnimationShadow(value[i], light, lightViewMatrix[dirNum], lightProjMatrix);
							}
							break;
						}
						default:
						{
							assert(0);
							break;
						}
					}
				}

				// 종료 후 ViewPort를 되돌려 놓는다.
				mDeviceContext->RSSetViewports(1, &defaultViewPort);
				// Rasterizer State를 되돌려 놓음
				mDeviceContext->RSSetState(defaultRasterState);
			}

			break;
		}
		case LightType::Spot:
		{
			assert(0);
			break;
		}
		default:
		{
			assert(0);
			break;
		}
	}
}

void ZeldaDX11Renderer::DrawDeferredLight(ZeldaLight* light, unsigned int lightIndex)
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// 다음 그리기에 영향을 주지 않기 위해 뎁스스텐실을 비워놓음
	mDeviceContext->OMSetRenderTargets(Deferred::Light::Count, deferredRenderTargets + Deferred::Light::Begin, nullptr);
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin, 2, deferredShaderResources + Deferred::Object::Begin);
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin + 2u, 1, &shadowShaderResource[0]);
	mDeviceContext->PSSetShaderResources(0u, 5, &shadowShaderResource[1]);
	float alphaBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	mDeviceContext->OMSetBlendState(alphaBlendState, alphaBlendFactor, 0xFFFFFFFF);

	ZeldaMesh* squareMesh = ResourceManager::GetInstance().GetSquareMesh();

	ZeldaCamera* mainCamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	MatrixBufferType matrixBuffer;
	matrixBuffer.view = XMMatrixTranspose(mainCamera->GetViewMatrix());
	matrixBuffer.projection = XMMatrixTranspose(mainCamera->GetProjMatrix());
	matrixBuffer.cameraFar = mainCamera->GetFar();

	matrixPsConstBuffer->SetData(matrixBuffer);

	LightMatrixBufferType lightMatrixBuffer;
	lightMatrixBuffer.shadowMapSize = static_cast<float>(ShadowMap::Size);
	lightMatrixBuffer.shadowMapDepthBias = ShadowMap::DepthBias;
	lightMatrixBuffer.pointLightDepthBias = ShadowMap::PointLightDepthBias;

	LightType lightType = light->GetLightType();
	ZeldaMesh* lightMesh = nullptr;
	ZeldaShader* lightShader = nullptr;

	if (lightType == LightType::Directional)
	{
		mDeviceContext->RSSetState(defaultRasterState);

		lightMatrixBuffer.view[0] = XMMatrixTranspose(light->GetViewMatrix(currentcamera));
		lightMatrixBuffer.projection = XMMatrixTranspose(light->GetOrthoMatrix());

		lightMesh = squareMesh;
		lightShader = deferredDirectionalLightShader;
	}
	else if (lightType == LightType::Point)
	{
		mDeviceContext->RSSetState(pointLightRasterState);

		DirectX::XMMATRIX view[6];
		DirectX::XMMATRIX proj;
		light->CreatePointLightViewAndProjMatrices(view, proj);
		lightMatrixBuffer.view[0] = XMMatrixTranspose(view[0]);
		lightMatrixBuffer.view[1] = XMMatrixTranspose(view[1]);
		lightMatrixBuffer.view[2] = XMMatrixTranspose(view[2]);
		lightMatrixBuffer.view[3] = XMMatrixTranspose(view[3]);
		lightMatrixBuffer.view[4] = XMMatrixTranspose(view[4]);
		lightMatrixBuffer.view[5] = XMMatrixTranspose(view[5]);
		lightMatrixBuffer.projection = XMMatrixTranspose(proj);

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

	matrixBuffer.world = XMMatrixTranspose(light->GetWorldMatrix());
	matrixVsConstBuffer->SetData(matrixBuffer);

	LightIndexBufferType lightIndexData;
	lightIndexData.lightIndex = lightIndex;
	lightIndexConstBuffer->SetData(lightIndexData);

	lightMatrixConstBuffer->SetData(lightMatrixBuffer);

	ConstantBufferManager::GetInstance().SetBuffer();

	lightShader->Render(mDeviceContext, lightMesh->GetIndexCount());


	mDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	ID3D11ShaderResourceView* nullSRV[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
	mDeviceContext->PSSetShaderResources(Deferred::SlotBegin, 3, nullSRV);
	mDeviceContext->PSSetShaderResources(0u, 5, nullSRV);
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
	return ResourceManager::GetInstance().CreateCamera(screenWidth, screenHeight);
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

LightID ZeldaDX11Renderer::CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction)
{
	LightID lightID = ResourceManager::GetInstance().CreateDirectionalLight();

	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);

	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());
	light->SetShadowColor(shadowColor.x(), shadowColor.y(), shadowColor.z());

	light->SetDirection(direction.x(), direction.y(), direction.z());

	return lightID;
}

LightID ZeldaDX11Renderer::CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& position, float range, float atten0, float atten1, float atten2)
{
	LightID lightID = ResourceManager::GetInstance().CreatePointLight();

	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);

	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());
	light->SetShadowColor(shadowColor.x(), shadowColor.y(), shadowColor.z());

	light->SetPosition(position.x(), position.y(), position.z());
	light->SetRange(range);

	light->SetAttenuation(atten0, atten1, atten2);

	return lightID;
}

LightID ZeldaDX11Renderer::CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle)
{
	LightID lightID = ResourceManager::GetInstance().CreateSpotLight();

	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);

	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());
	light->SetShadowColor(shadowColor.x(), shadowColor.y(), shadowColor.z());

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

void ZeldaDX11Renderer::UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle, float atten0, float atten1, float atten2)
{
	ZeldaLight* light = ResourceManager::GetInstance().GetLight(lightID);
	light->SetAmbient(ambient.x(), ambient.y(), ambient.z());
	light->SetDiffuseColor(diffuse.x(), diffuse.y(), diffuse.z());
	light->SetSpecular(specular.x(), specular.y(), specular.z());
	light->SetShadowColor(shadowColor.x(), shadowColor.y(), shadowColor.z());

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
			light->SetAttenuation(atten0, atten1, atten2);
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

void ZeldaDX11Renderer::DrawCubeMapRenderInfo()
{
	const auto& cubeMapRenderInfo = RenderInfoManager::GetInstance().GetCubeMapRenderInfo();

	if (cubeMapRenderInfo == nullptr || cubeMapRenderInfo->instancingKey.textureID == TextureID::ID_NULL)
	{
		// CubeMap을 그리지 않음
		return;
	}

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->OMSetDepthStencilState(cubeMapDepthStencilState, 1);
	mDeviceContext->RSSetState(cubeMapRasterState);


	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ZeldaMesh* mesh = ResourceManager::GetInstance().GetSphereMesh();
	mesh->Render(mDeviceContext);

	ZeldaTexture* texture = ResourceManager::GetInstance().GetTexture(cubeMapRenderInfo->instancingKey.textureID);
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
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
}

void ZeldaDX11Renderer::DrawStringRenderInfo()
{
	d2dRenderTarget->BeginDraw();

	const auto& stringRenderInfo = RenderInfoManager::GetInstance().GetStringRenderInfo();

	for (auto& renderInfo : stringRenderInfo)
	{
		std::wstring str = renderInfo->instancingValue.str;
		float x = renderInfo->instancingValue.position.x;
		float y = renderInfo->instancingValue.position.y;
		float width = renderInfo->instancingValue.size.x;
		float height = renderInfo->instancingValue.size.y;
		float fontSize = renderInfo->instancingValue.fontSize;
		float r = renderInfo->instancingValue.color.r;
		float g = renderInfo->instancingValue.color.g;
		float b = renderInfo->instancingValue.color.b;
		float a = renderInfo->instancingValue.color.a;

		// 텍스트 렌더링 준비
		ID2D1SolidColorBrush* textBrush = nullptr;
		d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &textBrush);

		// 텍스트 포맷 생성
		IDWriteTextFormat* textFormat = nullptr;
		writeFactory->CreateTextFormat(
			L"Consolas", // 폰트 이름
			nullptr, // 폰트 컬렉션
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize, // 글자 크기
			L"", // 로케일
			&textFormat
		);


		// 텍스트 렌더링
		d2dRenderTarget->DrawTextW(
			str.c_str(), // 텍스트 내용
			wcslen(str.c_str()), // 텍스트 길이
			textFormat, // 텍스트 포맷
			D2D1::RectF(x, y, x + width, y + height), // 텍스트가 그려질 사각형
			textBrush // 브러시
		);

		textFormat->Release();
		textBrush->Release();
	}

	d2dRenderTarget->EndDraw();
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

HRESULT ZeldaDX11Renderer::InitializeD2D()
{
	HRESULT result;
	D2D1_FACTORY_OPTIONS options = D2D1_FACTORY_OPTIONS();
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &d2dFactory);
	if (FAILED(result))
		return result;

	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	if (FAILED(result))
		return result;

	result = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&surface));
	if (FAILED(result))
		return result;

	D2D1_RENDER_TARGET_PROPERTIES d2dRenderTargetProperties =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
			0.0f, 0.0f);
	result = d2dFactory->CreateDxgiSurfaceRenderTarget(surface, &d2dRenderTargetProperties, &d2dRenderTarget);
	if (FAILED(result))
		return result;

	return result;
}

void ZeldaDX11Renderer::FinalizeD2D()
{
	if (d2dRenderTarget)
	{
		d2dRenderTarget->Release();
	}

	if (surface)
	{
		surface->Release();
	}

	if (writeFactory)
	{
		writeFactory->Release();
	}

	if (d2dFactory)
	{
		d2dFactory->Release();
	}
}
