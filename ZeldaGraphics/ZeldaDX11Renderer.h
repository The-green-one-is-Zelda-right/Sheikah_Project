#pragma once

#include "IZeldaRenderer.h"

#include "ConstantBuffer.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>

class ZeldaCamera;
class ZeldaShader;
class ZeldaModel;
class ZeldaLight;
class ZeldaTexture;
class ZeldaMaterial;
class IRenderable;

class ZeldaDX11Renderer : public IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear) override;
	virtual void Finalize() override;

	virtual void BeginDraw(float deltaTime) override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) override;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) override;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) override;

	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) override;

	virtual void CreateBasicResources() override;
	virtual void ReleaseBasicResources() override;

	virtual TextureID CreateTexture(const std::wstring& texturePath) override;
	virtual bool ReleaseTexture(TextureID textureID) override;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) override;
	virtual bool ReleaseModel(ModelID modelID) override;
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) override;
	virtual std::vector<float> GetAnimationPlayTime(ModelID modelID) override;

	virtual CameraID CreateCamera() override;
	virtual bool ReleaseCamera(CameraID cameraID) override;

	virtual bool SetMainCamera(CameraID cameraID) override;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) override;

private:
	bool bVsyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;

	ID3D11RasterizerState* defaultRasterState;
	ID3D11RasterizerState* wireFrameRasterState;

	ZeldaLight* light;

	DirectX::SpriteBatch* spriteBatch;

	HWND hWnd;
	unsigned int screenWidth;
	unsigned int screenHeight;

	// hwnd
	// fullScreenMode

	// Constant Buffer
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer;
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer;

	ConstantBuffer<LightBufferType, ShaderType::PixelShader>* lightConstBuffer;
	ConstantBuffer<UseBufferType, ShaderType::PixelShader>* useConstBuffer;
	ConstantBuffer<ColorBufferType, ShaderType::PixelShader>* colorConstBuffer;
};