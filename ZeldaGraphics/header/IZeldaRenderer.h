#pragma once

#include "GraphicsResourceID.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <Eigen\Dense>

enum class DebugMode : unsigned int
{
	Normal = 0x00000000u,
	DeferredDebugAll = 0x00000001u,
	DeferredDebug0 = 0x00000002u,
	DeferredDebug1 = 0x00000003u,
	DeferredDebug2 = 0x00000004u,
	DeferredDebug3 = 0x00000005u,
	DeferredDebug4 = 0x00000006u,
	DeferredDebug5 = 0x00000007u
};

enum class RendererMode : unsigned int
{
	None = 0x00000000u,
	Clear = 0xffffffffu,
	OnWireFrameMode = 0x00000001u,
	OffWireFrameMode = 0x80000001u
};

class IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear) abstract;
	virtual void Finalize() abstract;

	virtual void SetDebugMode(DebugMode mode) abstract;
	virtual void SetRendererMode(RendererMode mode) abstract;

	// deltaTime의 단위는 second
	virtual void BeginDraw(float deltaTime) abstract;
	virtual void EndDraw() abstract;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) abstract;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) abstract;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) abstract;
	virtual void DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring firstAnimationName, std::wstring secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame) abstract;

	// Light를 그린다. 같은 프레임에 동일한 라이트를 2개 이상 그린다면 한번만 적용한다.
	virtual void DrawLight(LightID lightID) abstract;

	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) abstract;

	virtual TextureID CreateTexture(const std::wstring& texturePath) abstract;
	virtual void ReleaseTexture(TextureID textureID) abstract;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) abstract;
	virtual void ReleaseModel(ModelID modelID) abstract;
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) abstract;
	virtual std::vector<float> GetAnimationPlayTime(ModelID model) abstract;

	virtual CameraID CreateCamera() abstract;
	virtual void ReleaseCamera(CameraID cameraID) abstract;

	virtual bool SetMainCamera(CameraID cameraID) abstract;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) abstract;

	virtual LightID CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction) abstract;
	virtual LightID CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& position, float range) abstract;
	virtual LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
	virtual void ReleaseLight(LightID lightID) abstract;

	virtual void UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
};