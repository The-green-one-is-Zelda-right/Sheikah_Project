#pragma once

#include "ZeldaGraphicsAPI.h"
#include "GraphicsResourceID.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <Eigen\Dense>

class IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear) abstract;
	virtual void Finalize() abstract;

	// deltaTime의 단위는 second
	virtual void BeginDraw(float deltaTime) abstract;
	virtual void EndDraw() abstract;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) abstract;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) abstract;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) abstract;
	
	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) abstract;

	virtual void CreateBasicResources() abstract;
	virtual void ReleaseBasicResources() abstract;

	virtual TextureID CreateTexture(const std::wstring& texturePath) abstract;
	virtual bool ReleaseTexture(TextureID textureID) abstract;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) abstract;
	virtual bool ReleaseModel(ModelID modelID) abstract;
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) abstract;
	virtual std::vector<float> GetAnimationPlayTime(ModelID model) abstract;

	virtual CameraID CreateCamera() abstract;
	virtual bool ReleaseCamera(CameraID cameraID) abstract;

	virtual bool SetMainCamera(CameraID cameraID) abstract;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) abstract;
};