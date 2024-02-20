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

struct Color
{
	union
	{
		struct
		{
			float r, g, b, a;
		};

		struct
		{
			float x, y, z, w;
		};
	};

	bool operator==(const Color& right) const
	{
		return r == right.r && g == right.g && b == right.b && a == right.a;
	}
};

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

namespace std {
	template <>
	struct hash<std::pair<std::pair<MeshID, TextureID>, std::pair<bool, Color>>> {
		size_t operator()(const std::pair<std::pair<MeshID, TextureID>, std::pair<bool, Color>>& obj) const {
			return std::hash<std::pair<MeshID, TextureID>>{}(obj.first) ^ (std::hash<unsigned int>{}(obj.second.second.r) ^ std::hash<unsigned int>{}(obj.second.second.y) ^ std::hash<unsigned int>{}(obj.second.second.z) ^ std::hash<unsigned int>{}(obj.second.second.w));
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

	virtual void SetDebugMode(DebugMode mode) override;
	virtual void SetRendererMode(RendererMode mode) override;

	virtual void BeginDraw(float deltaTime) override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) override;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) override;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) override;
	virtual void DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, const std::wstring& firstAnimationName, const std::wstring& secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame) override;

	virtual void DrawLight(LightID lightID) override;

	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) override;

	virtual void DrawCubeMap(TextureID texture) override;

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
	virtual LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) override;
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

	void DrawMeshRenderInfo(MeshRenderInfo renderInfo, ZeldaShader* shader);
	void DrawModelRenderInfo(ModelRenderInfo renderInfo, ZeldaShader* shader);
	void DrawBlendingAnimationRenderInfo(BlendingAnimationRenderInfo renderInfo, ZeldaShader* shader);
	void DrawSpriteRenderInfo(SpriteRenderInfo renderInfo);
	void DrawCubeMapRenderInfo();

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
	ID3D11RasterizerState* pointLightRasterState;
	ID3D11RasterizerState* cubeMapRasterState;
	ID3D11RasterizerState* currentRasterState;

	ID3D11BlendState* alphaBlendState;

	ID3D11DepthStencilState* cubeMapDepthStencilState;

	// Deferred Rendering
	ID3D11RenderTargetView* deferredRenderTargets[Deferred::BufferCount];
	ID3D11ShaderResourceView* deferredShaderResources[Deferred::BufferCount];

	ZeldaShader* deferredBlendingObjectShader;
	ZeldaShader* deferredObjectShader;
	ZeldaShader* deferredDirectionalLightShader;
	ZeldaShader* deferredPointLightShader;
	ZeldaShader* deferredSpotLightShader;
	ZeldaShader* deferredFinalShader;
	ZeldaShader* forwardShader;
	ZeldaShader* forwardBlendingShader;
	ZeldaShader* cubeMapShader;

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
	ConstantBuffer<AnimationBufferType, ShaderType::VertexShader>* animationConstBuffer;
	ConstantBuffer<InstancingMatrixBufferType, ShaderType::VertexShader>* instancingMatrixVsConstBuffer;
	ConstantBuffer<InstancingAnimationBufferType, ShaderType::VertexShader>* instancingAnimationVsConstBuffer;
	ConstantBuffer<BlendingAnimationBufferType, ShaderType::VertexShader>* blendingAnimationVsConstBuffer;

	ConstantBuffer<MatrixBufferType, ShaderType::PixelShader>* matrixPsConstBuffer;
	ConstantBuffer<LightInfoBufferType, ShaderType::PixelShader>* lightInfoConstBuffer;
	ConstantBuffer<LightIndexBufferType, ShaderType::PixelShader>* lightIndexConstBuffer;
	ConstantBuffer<MaterialBufferType, ShaderType::PixelShader>* materialConstBuffer;
	
	ConstantBuffer<ScreenBufferType, ShaderType::VertexShaderAndPixelShader>* screenConstBuffer;


	// Draw함수가 호출되면 채워진다. BeginDraw에서 ClearRenderInfo를 통해 초기화된다.
	std::unordered_map<std::pair<std::pair<MeshID, TextureID>, std::pair<bool, Color>>, MeshRenderInfo> organizedMeshRenderInfo;
	std::unordered_map<std::pair<std::pair<ModelID, std::wstring>, bool>, ModelRenderInfo> organizedModelRenderInfo;
	std::unordered_map<TextureID, SpriteRenderInfo> organizedSpriteRenderInfo;
	std::unordered_set<LightID> organizedLightRenderInfo;

	std::vector<BlendingAnimationRenderInfo> blendingAnimationRenderInfo;

	// 오브젝트들을 실제로 그리는 과정에서 WireFrame으로 그리도록 설정된 오브젝트들을 여기에 저장해두고 deferred render 후에 그린다.
	// 만약 RendererMode가 WireFrameMode라면 사용하지 않는다.
	std::unordered_map<std::pair<std::pair<MeshID, TextureID>, std::pair<bool, Color>>, MeshRenderInfo> forwardMeshRenderInfo;
	std::unordered_map<std::pair<std::pair<ModelID, std::wstring>, bool>, ModelRenderInfo> forwardModelRenderInfo;

	std::vector<BlendingAnimationRenderInfo> forwardBlendingAnimationRenderInfo;

	TextureID cubeMapRenderInfo;

#ifdef USE_BEGIN_FLAG
	bool beginflag = false;
#endif
};