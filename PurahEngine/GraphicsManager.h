#pragma once

#include "PurahEngineAPI.h"

#include <windows.h>
#include <Eigen/dense>

#include "GraphicsResourceID.h"

 
#ifdef YH_RENDERER
#include "../Floater/ZeldaGraphicsAdapter/include/IZeldaRendererAdapter.h"
#else
#include "IZeldaRenderer.h"
#endif

#include "ZeldaGraphics.h"
#include "ZeldaGraphicsAPI.h"

namespace PurahEngine
{
	class Camera;
	class Renderer;
	class Light;
	class IRenderer;
	class Animator;
	class GraphicsResourceManager;

	class PURAHENGINE_API GraphicsManager
	{
	public:
		void Initialize(HWND hWnd);
		void Finalize();

		void Run(float deltaTime);

	private:
		void AddRenderer(IRenderer* renderer);
		void RemoveRenderer(IRenderer* renderer);

		void AddLight(IRenderer* renderer);
		void RemoveLight(IRenderer* renderer);

	private:
		HMODULE zeldaGraphicsDLL;
		IZeldaRenderer* graphicsModule;
		
		std::vector<IRenderer*> lightList;
		std::vector<IRenderer*> rendererList;

		GraphicsResourceManager* resourceManager;

	private:
		GraphicsManager();
		~GraphicsManager();
		GraphicsManager(const GraphicsManager& ref) = delete;
		GraphicsManager& operator=(const GraphicsManager& ref) = delete;

	public:
		static GraphicsManager& GetInstance();

		friend Renderer;
		friend Light;
		friend Camera;
		friend Animator;
	};
}

