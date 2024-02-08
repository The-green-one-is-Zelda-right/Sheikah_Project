#pragma once

#include <Windows.h>

#include "IZeldaRenderer.h"

namespace GraphicsEngine
{
	class Sprite;
	class SpriteSheet;
	class Graphics;
}

class CoreSystem
{
public:
	friend class GraphicsSystem;

	// Initialize Coresystem and Subsystem
	void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, unsigned int width, unsigned int height, bool screenresizeable = true, bool notitlebar = true, bool minimizable = true, bool maximizable = true);

	void Finalize();

	void Run(_In_ int nCmdShow);

	void ChangeToFullScreenMode();
	void ChangeToWindowMode();
	void ChangeWindowSize(unsigned int width, unsigned int height);

	void SetResizable(bool value);

	unsigned int GetScreenWidth();
	unsigned int GetScreenHeight();
	bool IsFullScreenMode();

	bool IsRun();

private:
	enum class RunningState
	{
		INITIALIZATION,
		DESTROY,
		PHYSICS,
		PHYSICSWAIT,
		INPUTEVENT,
		GAMELOGIC,
		GUIEVENT,
		RENDER
	};

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void run();

	HWND hWnd;

	HMODULE zeldaGraphicsDLL;
	IZeldaRenderer* renderer;

	bool isFullScreenMode;
	bool resizable;

	bool firstRun;
	bool isRun;
	RunningState runningState;

	// ÇØ»óµµ
	unsigned int resolutionX;
	unsigned int resolutionY;
	unsigned int lastWidth;
	unsigned int lastHeight;

	HCURSOR hCursor;


	// graphics
	CameraID mainCameraID;
	TextureID scdTextureID;
	TextureID hnsTextureID;
	TextureID msTextureID;
	TextureID cubeMapID;
	ModelID fbxID;
	ModelID fbxID2;
	LightID dirLightID;
	LightID pointLightID;


	//singleton
public:
	static CoreSystem& GetInstance();

private:
	CoreSystem();
	~CoreSystem();
	CoreSystem(const CoreSystem& ref);
	CoreSystem& operator=(const CoreSystem& ref) = delete;
};