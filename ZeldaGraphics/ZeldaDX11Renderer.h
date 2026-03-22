#pragma once

#include "IZeldaRenderer.h"

#include "ZeldaGraphicsDefine.h"
#include "ConstantBuffer.h"
#include "RenderInfo.h"

#include "Color.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include <d2d1.h>
#include <dwrite.h>

#include <SpriteBatch.h>
#include <CommonStates.h>

#include <unordered_map>
#include <map>
#include <unordered_set>

#ifdef _DEBUG
#define USE_BEGIN_FLAG
#define USE_INIT_FLAG
#endif

class ZeldaCamera;
class ZeldaShader;
class ZeldaModel;
class ZeldaLight;
class ZeldaTexture;
class ZeldaMaterial;
class IRenderable;

struct MeshRenderInfo
{
	std::vector<MeshInstancingInfo> instancingInfo;
	MeshID meshId;
	TextureID textureID;
	bool wireFrame;
	Color color;
};

struct ModelRenderInfo
{
	std::vector<ModelInstancingInfo> instancingInfo;
	ModelID modelID;
	std::wstring animationName;
	bool wireFrame;
};

struct BlendingAnimationRenderInfo
{
	DirectX::XMMATRIX worldMatrix;
	ModelID modelID;
	std::wstring firstAnimationName;
	std::wstring secondAnimationName;
	float firstAnimationTime;
	float secondAnimationTime;
	float ratio;
	bool wireFrame;
};

struct SpriteRenderInfo
{
	std::vector<SpriteInstancingInfo> instancingInfo;
	TextureID textureID;
};

struct StringRenderInfo
{
	std::wstring str;
	float x;
	float y;
	float width;
	float height;
	float fontSize;
	float r;
	float g;
	float b;
	float a;
};

namespace std {
	template <>
	struct hash<std::pair<std::pair<MeshID, TextureID>, std::pair<bool, Color>>> {
		size_t operator()(const std::pair<std::pair<MeshID, TextureID>, std::pair<bool, Color>>& obj) const {
			return std::hash<std::pair<MeshID, TextureID>>{}(obj.first) ^ (std::hash<float>{}(obj.second.second.r) ^ std::hash<float>{}(obj.second.second.y) ^ std::hash<float>{}(obj.second.second.z) ^ std::hash<float>{}(obj.second.second.w));
		}
	};
}

namespace std {
	template <>
	struct hash<std::pair<std::pair<ModelID, std::wstring>, bool>> {
		size_t operator()(const std::pair<std::pair<ModelID, std::wstring>, bool>& obj) const {
			return std::hash<ModelID>{}(obj.first.first) ^ std::hash<std::wstring>{}(obj.first.second) ^ std::hash<bool>{}(obj.second);
		}
	};
}

class ZeldaDX11Renderer : public IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen) override;
	virtual void Finalize() override;

	virtual void Resize(unsigned int screenWidth, unsigned int screenHeight) override;

	virtual void SetExtraInitOption(float shadowAreaRange, float shadowAreaOffset, unsigned int shadowMapSize) override;
	virtual void SetExtraOption(float shadowMapDepthBias, float pointLightDepthBias) override;

	virtual void SetDebugMode(DebugMode mode) override;
	virtual void SetRendererMode(RendererMode mode) override;

	virtual void BeginDraw(float deltaTime) override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color color, Color outLineColor) override;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color outLineColor) override;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color outLineColor) override;
	virtual void DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, const std::wstring& firstAnimationName, const std::wstring& secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame, bool drawDirectionalShadow, bool drawPointShadow, bool fastOutLine, bool outLine, Color outLineColor) override;

	virtual void DrawLight(LightID lightID) override;

	virtual void DrawImage(const Eigen::Vector2f& position, TextureID texture, Color color, unsigned int layer) override;
	virtual void DrawImage(const Eigen::Vector2f& position, const Eigen::Vector2f& size, TextureID texture, Color color, unsigned int layer) override;

	virtual void DrawBillBoardParticle(const Eigen::Matrix4f& worldMatrix, const std::vector<Eigen::Matrix4f>& particleMatrix, unsigned int layer, TextureID texture, float ccwRadianAngle, bool keepOriginSize, bool useAlphaTexture, const std::vector<Color>& colors) override;

	virtual void DrawBillBoard(const Eigen::Matrix4f& worldMatrix, TextureID texture, float ccwRadianAngle, bool keepOriginSize, bool useAlphaTexture, Color color) override;

	virtual void DrawSprite(const Eigen::Matrix4f& worldMatrix, TextureID texture, Color color, bool keepOriginSize) override;

	virtual void DrawCubeMap(TextureID texture) override;

	virtual void DrawString(const std::wstring& string, float x, float y, float width, float height, float fontSize, Color color) override;


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

	virtual LightID CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction) override;
	virtual LightID CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& position, float range, float atten0, float atten1, float atten2) override;
	virtual LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) override;
	virtual void ReleaseLight(LightID lightID) override;

	virtual void UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle, float atten0, float atten1, float atten2) override;

private:
	void CreateBasicResources();
	void ReleaseBasicResources();

	void DrawDeferred();
	void DrawForward();
	void DrawDebugInfo();
	void DrawSpriteRenderInfo();
	void DrawImage();
	void DrawBillBoardRenderInfo();
	void DrawBillBoardParticleRenderInfo();

	void DrawDeferredRenderInfo();
	void DrawForwardRenderInfo();
	void DrawFastOutLine();
	void DrawOutLine();

	void DrawMeshRenderInfo(const std::vector<RenderInfo*>& renderInfo, ZeldaShader* shader);
	void DrawModelRenderInfo(const std::vector<RenderInfo*>& renderInfo, ZeldaShader* shader);
	void DrawBlendingAnimationRenderInfo(RenderInfo* renderInfo, ZeldaShader* shader);

	void DrawMeshShadow(const std::vector<RenderInfo*>& renderInfo, ZeldaLight* light, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjMatrix);
	void DrawModelShadow(const std::vector<RenderInfo*>& renderInfo, ZeldaLight* light, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjMatrix);
	void DrawBlendingAnimationShadow(RenderInfo* renderInfo, ZeldaLight* light, DirectX::XMMATRIX lightViewMatrix, DirectX::XMMATRIX lightProjMatrix);

	void DrawImageRenderInfo(RenderInfo* renderInfo);

	void CreateShadowMap(ZeldaLight* light);
	void DrawDeferredLight(ZeldaLight* light, unsigned int lightIndex);

	void DrawCubeMapRenderInfo();
	void DrawStringRenderInfo();

	void UpdateMode();

	void UseWireFrameRasterState(bool use);
	bool CheckDebugMode(DebugMode mode);
	bool CheckRendererMode(RendererMode mode);

private:
	bool bVsyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11DepthStencilState* mDepthStencilState = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;

	ID3D11RasterizerState* defaultRasterState = nullptr;
	ID3D11RasterizerState* wireFrameRasterState = nullptr;
	ID3D11RasterizerState* directionalLightRasterState = nullptr;
	ID3D11RasterizerState* pointLightRasterState = nullptr;
	ID3D11RasterizerState* cubeMapRasterState = nullptr;
	ID3D11RasterizerState* shadowRasterState = nullptr;
	ID3D11RasterizerState* spriteRasterState = nullptr;
	ID3D11RasterizerState* currentRasterState = nullptr;

	ID3D11BlendState* alphaBlendState = nullptr;
	ID3D11BlendState* spriteBlendState = nullptr;

	ID3D11DepthStencilState* particleDepthStencilState = nullptr;
	ID3D11DepthStencilState* cubeMapDepthStencilState = nullptr;

	D3D11_VIEWPORT defaultViewPort;

	// Deferred Rendering
	ID3D11RenderTargetView* deferredRenderTargets[Deferred::BufferCount];
	ID3D11ShaderResourceView* deferredShaderResources[Deferred::BufferCount];

	// Shadow Mapping
	ID3D11DepthStencilView* shadowDepthStencilView[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	ID3D11ShaderResourceView* shadowShaderResource[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	ID3D11DepthStencilState* directionalShadowDepthStencilState = nullptr;

	ZeldaShader* deferredBlendingObjectShader = nullptr;
	ZeldaShader* deferredObjectShader = nullptr;
	ZeldaShader* deferredDirectionalLightShader = nullptr;
	ZeldaShader* deferredPointLightShader = nullptr;
	ZeldaShader* deferredSpotLightShader = nullptr;
	ZeldaShader* deferredFinalShader = nullptr;
	ZeldaShader* forwardShader = nullptr;
	ZeldaShader* forwardBlendingShader = nullptr;
	ZeldaShader* cubeMapShader = nullptr;
	ZeldaShader* shadowMapShader = nullptr;
	ZeldaShader* blendingAnimationShadowMapShader = nullptr;
	ZeldaShader* spriteShader = nullptr;
	ZeldaShader* alphaSpriteShader = nullptr;
	ZeldaShader* fastOutLineShader = nullptr;
	ZeldaShader* outLineShader = nullptr;
	ZeldaShader* outLineObjectShader = nullptr;
	ZeldaShader* outLineBlendingAnimationObjectShader = nullptr;

	DebugMode debugMode;
	DebugMode debugModeBuffer;

	RendererMode rendererMode;
	RendererMode rendererModeBuffer;

	DirectX::SpriteBatch* spriteBatch = nullptr;
	DirectX::DX11::CommonStates* commonStates = nullptr;

	HWND hWnd;
	unsigned int screenWidth;
	unsigned int screenHeight;

private:
	unsigned int particleCount;

private:
	// ID Map
	ID3D11RenderTargetView* idMapRenderTarget = nullptr;
	ID3D11ShaderResourceView* idMapShaderResource = nullptr;

	// Outline Map
	ID3D11RenderTargetView* outlineMapRenderTarget = nullptr;
	ID3D11ShaderResourceView* outlineMapShaderResource = nullptr;

	// ID Constant Buffer
	ConstantBuffer<ObjectIDBufferType, ShaderType::PixelShader>* objectIDPSConstBuffer = nullptr;

private:
	ID2D1Factory* d2dFactory = nullptr;
	IDWriteFactory* writeFactory = nullptr;
	IDXGISurface* surface = nullptr;
	ID2D1RenderTarget* d2dRenderTarget = nullptr;

	HRESULT InitializeD2D();
	void FinalizeD2D();

private:
	// Constant Buffer
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixVsConstBuffer = nullptr;
	ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>* animationConstBuffer = nullptr;
	ConstantBuffer<InstancingMatrixBufferType, ShaderType::VertexShader>* instancingMatrixVsConstBuffer = nullptr;
	ConstantBuffer<BlendingAnimationBufferType, ShaderType::VertexShader>* blendingAnimationVsConstBuffer = nullptr;

	ConstantBuffer<MatrixBufferType, ShaderType::PixelShader>* matrixPsConstBuffer = nullptr;
	ConstantBuffer<LightInfoBufferType, ShaderType::PixelShader>* lightInfoConstBuffer = nullptr;
	ConstantBuffer<LightIndexBufferType, ShaderType::PixelShader>* lightIndexConstBuffer = nullptr;
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer = nullptr;
	
	ConstantBuffer<ScreenBufferType, ShaderType::VertexShaderAndPixelShader>* screenConstBuffer = nullptr;
	ConstantBuffer<LightMatrixBufferType, ShaderType::VertexShaderAndPixelShader>* lightMatrixConstBuffer = nullptr;
	ConstantBuffer<InstancingDataBufferType, ShaderType::VertexShaderAndPixelShader>* instancingDataConstBuffer = nullptr;
	ConstantBuffer<DataBufferType, ShaderType::VertexShaderAndPixelShader>* dataConstBuffer = nullptr;

#ifdef USE_BEGIN_FLAG
	bool beginflag = false;
#endif

#ifdef USE_INIT_FLAG
	bool initflag = false;
#endif
};