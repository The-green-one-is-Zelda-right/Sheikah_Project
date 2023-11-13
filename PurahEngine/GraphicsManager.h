#pragma once
#include <windows.h>
#include <Eigen/core>
#include <Eigen/dense>

#include "GraphicsResourceID.h"
#include "IZeldaRenderer.h"
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
		// Ŭ������ �����ϰ� �� ���, �⺻������ = �� ���� ������ �����ε��� �����ȴ�.
		// �̱����� ��ü�� �ϳ����� �ϹǷ� �װ��� �����ϱ� ����, ���������� delete�� ����Ͽ� ����� ���´�.
		GraphicsManager& operator=(const GraphicsManager& ref) = delete;

	public:
		static GraphicsManager& GetInstance();
	};

}
