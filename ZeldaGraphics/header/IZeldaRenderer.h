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
	/// Renderer의 초기화
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen) abstract;
	/// Renderer의 종료
	virtual void Finalize() abstract;

	// 디버그 모드를 선택한다.
	virtual void SetDebugMode(DebugMode mode) abstract;
	// 렌더러 모드를 선택한다.
	virtual void SetRendererMode(RendererMode mode) abstract;

	// 그리기를 준비한다. Draw명령 이전에 반드시 한번 호출되어야 한다. (deltaTime의 단위는 second)
	virtual void BeginDraw(float deltaTime) abstract;
	// 그리기를 종료한다. Draw명령 이후에 반드시 한번 호출되어야 한다.
	virtual void EndDraw() abstract;



	// ### DrawXXXX() 
	// 무언가를 그리는 명령, 동일한 ID를 사용할경우 최적화된다.

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) abstract;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) abstract;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) abstract;
	virtual void DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring firstAnimationName, std::wstring secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame) abstract;

	// Light를 그린다. 같은 프레임에 동일한 라이트를 2개 이상 그린다면 한번만 적용한다.
	virtual void DrawLight(LightID lightID) abstract;

	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) abstract;

	// 큐브맵을 그린다. 마지막으로 그린 단 하나만 그려진다.
	virtual void DrawCubeMap(TextureID texture) abstract;



	// ### CreateXXXX()
	// Resource를 생성하는 명령, 생성을 해도 Draw명령을 하기 전에는 화면에 그려지지 않는다.
	// 단, 카메라는 Draw대신 SetMainCamera 사용

	// 텍스쳐 생성 (동일한 경로로 여러개의 ID를 만들 수 있지만 그리기에 동일한 ID를 사용한 경우에만 최적화됨)
	virtual TextureID CreateTexture(const std::wstring& texturePath) abstract;
	// 텍스쳐 해제
	virtual void ReleaseTexture(TextureID textureID) abstract;

	// 모델 생성 (동일한 경로로 여러개의 ID를 만들 수 있지만 그리기에 동일한 ID를 사용한 경우에만 최적화됨)
	virtual ModelID CreateModel(const std::wstring& modelingFilePath) abstract;
	// 모델 해제
	virtual void ReleaseModel(ModelID modelID) abstract;
	// 애니메이션 이름 얻기 (ModelID에 들어있는 모델이 가진 모든 애니메이션의 이름을 std::vector로 얻는다. 이름의 오름차순으로 정렬된다.)
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) abstract;
	// 애니메이션 재생 시간 얻기 (ModelID에 들어있는 모델이 가진 모든 애니메이션의 재생시간을 얻는다. 해당 애니메이션의 이름의 오름차순으로 정렬된다.)
	virtual std::vector<float> GetAnimationPlayTime(ModelID model) abstract;


	// 카메라를 생성
	virtual CameraID CreateCamera() abstract;
	// 카메라 해제
	virtual void ReleaseCamera(CameraID cameraID) abstract;

	// 메인 카메라 설정
	virtual bool SetMainCamera(CameraID cameraID) abstract;
	// 카메라 정보 수정
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) abstract;


	// Directional Light 생성
	virtual LightID CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction) abstract;
	// Point Light 생성
	virtual LightID CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& position, float range) abstract;
	// Spot Light 생성
	virtual LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
	// Light 해제
	virtual void ReleaseLight(LightID lightID) abstract;

	// Light 정보 수정
	virtual void UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
};