#pragma once
#include <windows.h>
#include <Eigen/core>
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
	class GraphicsManager
	{
	public:
		void Initialize(HWND hWnd);
		void Run();

	private:
		HMODULE zeldaGraphicsDLL;
		IZeldaRenderer* renderer;

	private:
		GraphicsManager();
		~GraphicsManager();
		GraphicsManager(const GraphicsManager& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		GraphicsManager& operator=(const GraphicsManager& ref) = delete;

	public:
		static GraphicsManager& GetInstance();
	};

}

