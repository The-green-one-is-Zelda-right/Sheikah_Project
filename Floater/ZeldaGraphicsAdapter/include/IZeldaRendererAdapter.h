#pragma once
#pragma warning(push)
#pragma warning(disable: 4819 26495)
#include "IZeldaRenderer.h"
#pragma warning (pop)


class RendererObjectMgr;
namespace flt
{
	class IRenderer;
}


class IZeldaRendererAdapter : public IZeldaRenderer
{
public:
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear) override;
	virtual void Finalize() override;

	// deltaTime의 단위는 second
	virtual void BeginDraw(float deltaTime) override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) override;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) override;

	virtual void CreateBasicResources() override;
	virtual void ReleaseBasicResources() override;

	virtual TextureID CreateTexture(const std::wstring& texturePath) override;
	virtual bool ReleaseTexture(TextureID textureID) override;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) override;
	virtual bool ReleaseModel(ModelID modelID) override;

	virtual CameraID CreateCamera() override;
	virtual bool ReleaseCamera(CameraID cameraID) override;

	virtual bool SetMainCamera(CameraID cameraID) override;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) override;

private:
	unsigned int _screenWidth;
	unsigned int _screenHeight;
	bool _vsync;
	HWND _hwnd;
	bool _fullScreen;
	float _screenDepth;
	float _cameraNear;
	float _deltaTime;
	flt::IRenderer* _renderer;
	RendererObjectMgr* _rendererObjectMgr;
};

