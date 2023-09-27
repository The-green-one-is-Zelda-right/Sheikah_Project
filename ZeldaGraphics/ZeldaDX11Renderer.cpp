#include "ZeldaDX11Renderer.h"

#include <cassert>

bool ZeldaDX11Renderer::Initialize(HWND hwnd)
{
	hWnd = hwnd;

	// 1. D3D11CreateDevice�Լ��� �̿��ؼ� Device�� DeviceContext�� ����

	UINT createDeviceFlags = 0;

	HRESULT hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext
	);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}


	// 2. ID3D11Device::CheckMultisampleQualityLevels�� �̿��ؼ� 4XMSAA ǰ�� ���� ���� ���θ� ����

	UINT m4xMsaaQuality;

	hr = md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	if (FAILED(hr))
	{
		MessageBox(0, L"CheckMultisampleQualityLevels Failed", 0, 0);
		return false;
	}


	// 3. ������ SwapChain�� Ư���� �����ϴ� DXGI_SWAP_CHAIN_DESC ����ü�� ä���.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = clientWidth;
	sd.BufferDesc.Height = clientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;

		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = !fullScreenMode;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// DXGI_SWAP_EFFECT_FLIP_DISCARD
	sd.Flags = 0;


	// 4. Device�� �����ϴµ� ����ߴ� IDXGIFactory �������̽��� �����ؼ� IDXGISwapChain�� �����Ѵ�.

	IDXGIDevice* dxgiDevice = 0;
	hr = md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(hr))
	{
		MessageBox(0, L"IDXGIDevice Failed", 0, 0);
		return false;
	}

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"IDXGIAdapter Failed", 0, 0);
		return false;
	}

	IDXGIFactory* dxgiFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"IDXGIFactory Failed", 0, 0);
		return false;
	}

	hr = dxgiFactory->CreateSwapChain(md3dDevice, &sd, &swapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"IDXGISwapChain Failed", 0, 0);
		return false;
	}

	dxgiDevice->Release();
	dxgiDevice = nullptr;

	dxgiAdapter->Release();
	dxgiAdapter = nullptr;

	dxgiFactory->Release();
	dxgiFactory = nullptr;


	// 5. SwapChain�� �ĸ���ۿ� ���� ����Ÿ�� �並 �����Ѵ�.

	ID3D11Texture2D* backBuffer;

	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"ID3D11Texture2D Failed", 0, 0);
		return false;
	}
	
	hr = md3dDevice->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	if (FAILED(hr))
	{
		MessageBox(0, L"ID3D11RenderTargetView Failed", 0, 0);
		return false;
	}

	backBuffer->Release();
	backBuffer = nullptr;


	// 6. ����-���ٽ� ���ۿ� �׿� ����Ǵ� ����-���ٽ� �並 �����Ѵ�.

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = clientWidth;
	depthStencilDesc.Height = clientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"ID3D11Texture2D Failed", 0, 0);
		return false;
	}

	hr = md3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"ID3D11DepthStencilView Failed", 0, 0);
		return false;
	}


	// 7. ����Ÿ�� ��, ����-���ٽ� �並 direct3D�� ����� �� �ֵ��� ������ ������������ ��� ���ձ� �ܰ迡 ���´�.

	md3dImmediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);


	// 8. ����Ʈ�� �����Ѵ�.

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(clientWidth);
	vp.Height = static_cast<float>(clientHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &vp);

	// ȭ���� �����
	//ID3D11DeviceContext::ClearRenderTargetView();

	return true;
}

void ZeldaDX11Renderer::BeginDraw()
{
	float clearColor[4];
	clearColor[0] = 0.0f;
	clearColor[1] = 1.0f;
	clearColor[2] = 0.0f;
	clearColor[3] = 1.0f;

	md3dImmediateContext->ClearRenderTargetView(renderTargetView, clearColor);
}

void ZeldaDX11Renderer::EndDraw()
{
	swapChain->Present(1, 0);
}
