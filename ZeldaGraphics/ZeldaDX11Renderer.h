#pragma once

#include "IZeldaRenderer.h"

#include "ZeldaGraphicsDefine.h"
#include "ConstantBuffer.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include <SpriteBatch.h>
#include <CommonStates.h>

#include <unordered_map>
#include <unordered_set>

#ifdef _DEBUG
#define USE_BEGIN_FLAG
#endif

class ZeldaCamera;
class ZeldaShader;
class ZeldaModel;
class ZeldaLight;
class ZeldaTexture;
class ZeldaMaterial;
class IRenderable;

struct MeshInstancingInfo
{
	DirectX::XMMATRIX worldMatrix;
	bool wireFrame;
	DirectX::XMFLOAT4 color;
};

struct ModelInstancingInfo
{
	DirectX::XMMATRIX worldMatrix;
	std::wstring animationName;
	float animationTime;
	bool wireFrame;
};

struct SpriteInstancingInfo
{
	DirectX::XMFLOAT2 position;
};

struct MeshRenderInfo
{
	std::vector<MeshInstancingInfo> instancingInfo;
	MeshID meshId;
	TextureID textureID;
};

struct ModelRenderInfo
{
	std::vector<ModelInstancingInfo> instancingInfo;
	ModelID modelID;
};

struct SpriteRenderInfo
{
	std::vector<SpriteInstancingInfo> instancingInfo;
	TextureID textureID;
};

class ZeldaDX11Renderer : public IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear) override;
	virtual void Finalize() override;

	virtual void SetDebugMode(DebugMode mode) override;
	virtual void SetRendererMode(RendererMode mode) override;

	virtual void BeginDraw(float deltaTime) override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) override;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) override;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) override;
	virtual void DrawLight(LightID lightID) override;

	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) override;

	virtual TextureID CreateTexture(const std::wstring& texturePath) override;
	virtual void ReleaseTexture(TextureID textureID) override;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) override;
	virtual void ReleaseModel(ModelID modelID) override;
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) override;
	virtual std::vector<float> GetAnimationPlayTime(ModelID modelID) override;

	virtual CameraID CreateCamera() override;
	virtual void ReleaseCamera(CameraID cameraID) override;

	virtual bool SetMainCamera(CameraID cameraID) override;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) override;

	virtual LightID CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction) override;
	virtual LightID CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& position, float range) override;
	virtual LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& position, float range, float angle) override;
	virtual void ReleaseLight(LightID lightID) override;

	virtual void UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) override;

private:
	void CreateBasicResources();
	void ReleaseBasicResources();

	void DrawDeferred();
	void DrawForward();
	void BeginDrawSprite();
	void DrawSprite();
	void EndDrawSprite();

	void DrawMeshRenderInfo(MeshRenderInfo renderInfo);
	void DrawModelRenderInfo(ModelRenderInfo renderInfo);
	void DrawSpriteRenderInfo(SpriteRenderInfo renderInfo);

	void ClearRenderInfo();

	void UpdateMode();

	void UseWireFrameRasterState(bool use);
	bool CheckDebugMode(DebugMode mode);
	bool CheckRendererMode(RendererMode mode);

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
	ID3D11RasterizerState* currentRasterState;

	ID3D11BlendState* alphaBlendState;

	// Deferred Rendering
	ID3D11RenderTargetView* deferredRenderTargets[Deferred::bufferCount];
	ID3D11ShaderResourceView* deferredShaderResources[Deferred::bufferCount];

	ZeldaShader* deferredObjectShader;
	ZeldaShader* deferredLightShader;
	ZeldaShader* deferredFinalShader;

	DebugMode debugMode;
	DebugMode debugModeBuffer;

	RendererMode rendererMode;
	RendererMode rendererModeBuffer;

	DirectX::SpriteBatch* spriteBatch;
	DirectX::DX11::CommonStates* commonStates;

	HWND hWnd;
	unsigned int screenWidth;
	unsigned int screenHeight;

	// hwnd
	// fullScreenMode

	// Constant Buffer
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixVsConstBuffer;
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer;

	ConstantBuffer<MatrixBufferType, ShaderType::PixelShader>* matrixPsConstBuffer;
	ConstantBuffer<LightInfoBufferType, ShaderType::PixelShader>* lightInfoConstBuffer;
	ConstantBuffer<LightIndexBufferType, ShaderType::PixelShader>* lightIndexConstBuffer;
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer;


	// Draw�Լ��� ȣ��Ǹ� ä������. BeginDraw���� ClearRenderInfo�� ���� �ʱ�ȭ�ȴ�.
	std::unordered_map<std::pair<MeshID, TextureID>, MeshRenderInfo> organizedMeshRenderInfo;
	std::unordered_map<ModelID, ModelRenderInfo> organizedModelRenderInfo;
	std::unordered_map<TextureID, SpriteRenderInfo> organizedSpriteRenderInfo;
	std::unordered_set<LightID> organizedLightRenderInfo;

#ifdef USE_BEGIN_FLAG
	bool beginflag = false;
#endif
};