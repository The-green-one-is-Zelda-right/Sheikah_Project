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

bool ZeldaDX11Renderer::Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear)
{
	hWnd = hwnd;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, numerator, denominator;
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
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// vsync(��������ȭ) ���� ����.
	bVsyncEnabled = vsync;

	// DirectX �׷��Ƚ� �������̽��� IDXGIFactory ����.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) return false;

	// Factory ��ü�� ����Ͽ� �⺻���� ������ �׷��Ƚ� �������̽��� ���� ����� ����(���� ī�带 ����).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	// �⺻���� ������ ����� ���(�����)�� �����Ѵ�.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	// ����� ���(�����)�� DXGI_FORMAT_R8G8B8A8_UNORM ȭ�� ���˿� �´� ����� ������ ���Ѵ�. 
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result)) return false;

	// �ش� �����/���� ī�� �������� ������ ��� ���÷��� ����� ����Ʈ ����.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// ȭ�� ��� ����Ʈ�� ä���.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList);
	if (FAILED(result)) return false;

	// ���� ��� ���÷��� ��带 ���캸�� ȭ�� �ʺ� �� ���̿� ��ġ�ϴ� ��带 ã�´�.
	// ��ġ�ϴ� �׸��� ã���� �ش� ����� �ֻ����� �и𰪰� ���ڰ��� �����Ѵ�.
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
	// ������� description�� ���´�.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	// ���� ī�� �޸𸮸� MB������ ����.
	// ������ ����� ������� �ʴ´ٸ� �ּ��� ���� ���� �Լ��� ����� �� ��.
	mVideoCardMemory = (unsigned int)adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	//mVideoCardMemory = (unsigned int)adapterDesc.DedicatedSystemMemory / 1024 / 1024;

	// ���� ī���� �̸��� �迭�� ����.
	error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) return false;

	// ���÷��� ��� ����Ʈ�� ����.
	delete[] displayModeList;
	displayModeList = 0;
	// adapterOutput ��ü ����.
	adapterOutput->Release();
	adapterOutput = 0;
	// adapter ��ü ����.
	adapter->Release();
	adapter = 0;
	// Factory ��ü ����.
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

	
	ResourceManager::GetInstance().Initialize(mDevice);

	ConstantBufferManager::GetInstance().Initialize(mDevice, mDeviceContext);
	matrixConstBuffer = new ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>(mDevice);

	lightConstBuffer = new ConstantBuffer<LightBufferType, ShaderType::PixelShader>(mDevice);
	useConstBuffer = new ConstantBuffer<UseBufferType, ShaderType::PixelShader>(mDevice);
	colorConstBuffer = new ConstantBuffer<ColorBufferType, ShaderType::PixelShader>(mDevice);

	ConstantBufferManager::GetInstance().RegisterVSBuffer(matrixConstBuffer);

	ConstantBufferManager::GetInstance().RegisterPSBuffer(lightConstBuffer);
	ConstantBufferManager::GetInstance().RegisterPSBuffer(useConstBuffer);
	ConstantBufferManager::GetInstance().RegisterPSBuffer(colorConstBuffer);

	return true;
}

void ZeldaDX11Renderer::Finalize()
{
	if (matrixConstBuffer)
	{
		delete matrixConstBuffer;
		matrixConstBuffer = nullptr;
	}
	if (lightConstBuffer)
	{
		delete lightConstBuffer;
		lightConstBuffer = nullptr;
	}
	if (useConstBuffer)
	{
		delete useConstBuffer;
		useConstBuffer = nullptr;
	}
	if (colorConstBuffer)
	{
		delete colorConstBuffer;
		colorConstBuffer = nullptr;
	}
	if (mSwapChain)
	{
		mSwapChain->SetFullscreenState(false, nullptr);
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

	// ResourceManager Finalize �߰� �ʿ�
}

void ZeldaDX11Renderer::BeginDraw(float deltaTime)
{
	float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

	// Clear the back buffer.
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);
	// Clear the depth buffer.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ZeldaDX11Renderer::EndDraw()
{
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
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ZeldaMesh* cubeMesh = ResourceManager::GetInstance().GetCubeMesh();
	cubeMesh->Render(mDeviceContext);

	ZeldaTexture* cubeTexture = ResourceManager::GetInstance().GetTexture(texture);

	if (wireFrame)
	{
		mDeviceContext->RSSetState(wireFrameRasterState);
	}
	else
	{
		mDeviceContext->RSSetState(defaultRasterState);
	}

	// ZeldaMatrix to XMMATRIX
	DirectX::XMMATRIX dxworldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);

	// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
	matrixConstBuffer->SetData({ XMMatrixTranspose(MathConverter::EigenMatrixToXMMatrix(worldMatrix)), XMMatrixTranspose(currentcamera ->GetViewMatrix()), XMMatrixTranspose(currentcamera ->GetProjMatrix()) });
	lightConstBuffer->SetData({ light->GetAmbient(), light->GetDiffuseColor(), light->GetSpecular(), light->GetDirection() });
	useConstBuffer->SetData({ false, (cubeTexture != nullptr), true, (cubeTexture != nullptr) && cubeTexture->UseSRGB()});
	colorConstBuffer->SetData({ { r, g, b, a } });

	ConstantBufferManager::GetInstance().SetBuffer();

	ZeldaShader* shader = ResourceManager::GetInstance().GetDefaultShader();
	shader->Render(mDeviceContext, cubeMesh, cubeTexture);
}

void ZeldaDX11Renderer::DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame)
{
	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	ZeldaModel* modelData = ResourceManager::GetInstance().GetModel(model);

	// ZeldaMatrix to XMMATRIX
	DirectX::XMMATRIX dxworldMatrix = MathConverter::EigenMatrixToXMMatrix(worldMatrix);

	std::vector<ZNode*> nodes = modelData->GetAllNode();

	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i]->zeldaMeshes.size(); j++)
		{
			ZNode* currentNode = nodes[i];
			ZeldaMesh* currentMesh = currentNode->zeldaMeshes[j];
			ZeldaTexture* currentTexture = currentNode->zeldaTextures[j];

			currentMesh->Render(mDeviceContext);

			if (wireFrame)
			{
				mDeviceContext->RSSetState(wireFrameRasterState);
			}
			else
			{
				mDeviceContext->RSSetState(defaultRasterState);
			}

			// ���̴��� �ѱ�� ����� ��ġ�� �� �� �Ѱܾ� �Ѵ�.
			matrixConstBuffer->SetData({ XMMatrixTranspose(currentNode->GetWorldTransformMatrix() * MathConverter::EigenMatrixToXMMatrix(worldMatrix)), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
			lightConstBuffer->SetData({ light->GetAmbient(), light->GetDiffuseColor(), light->GetSpecular(), light->GetDirection() });
			useConstBuffer->SetData({ false, (currentTexture != nullptr), true, (currentTexture != nullptr && currentTexture->UseSRGB()) });
			colorConstBuffer->SetData({ { 1, 0, 0, 1 } });

			ConstantBufferManager::GetInstance().SetBuffer();

			ZeldaShader* shader = ResourceManager::GetInstance().GetDefaultShader();
			shader->Render(mDeviceContext, currentMesh, currentTexture);
		}
	}
}

void ZeldaDX11Renderer::CreateBasicResources()
{
	ResourceManager::GetInstance().CreateDefaultShader();
	ResourceManager::GetInstance().CreateCubeMesh();

	light = new ZeldaLight();
	light->SetAmbient(0.2f, 0.2f, 0.2f, 1.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, -0.6f, 0.8f);
}

void ZeldaDX11Renderer::ReleaseBasicResources()
{
	// �̱���
	assert(0);
}

TextureID ZeldaDX11Renderer::CreateTexture(const std::wstring& texturePath)
{
	return ResourceManager::GetInstance().CreateTexture(texturePath);
}

bool ZeldaDX11Renderer::ReleaseTexture(TextureID textureID)
{
	// �̱���
	assert(0);
	return false;
}

ModelID ZeldaDX11Renderer::CreateModel(const std::wstring& modelingFilePath)
{
	return ResourceManager::GetInstance().CreateModelFromModelingFile(modelingFilePath);
}

bool ZeldaDX11Renderer::ReleaseModel(ModelID modelID)
{
	// �̱���
	assert(0);
	return false;
}

CameraID ZeldaDX11Renderer::CreateCamera()
{
	return ResourceManager::GetInstance().CreateCamera(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
}

bool ZeldaDX11Renderer::ReleaseCamera(CameraID cameraID)
{
	if (ZeldaCamera::GetMainCamera() == cameraID)
	{
		ZeldaCamera::SetMainCamera(CameraID::ID_NULL);
	}

	return ResourceManager::GetInstance().ReleaseCamera(cameraID);
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
