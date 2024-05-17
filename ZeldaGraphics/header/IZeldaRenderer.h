#pragma once

#include "GraphicsResourceID.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <Eigen\Dense>

#include "Color.h"

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

	// 화면 크기 재설정
	virtual void Resize(unsigned int screenWidth, unsigned int screenHeight) abstract;

	/// <summary>
	/// 추가 옵션을 설정한다. Initialize가 호출되기 이전에 호출되어야 설정된 옵션을 적용할 수 있다.
	/// </summary>
	/// <param name="[shadowAreaRange] :"> 그림자영역의 범위, 이 영역안에 있는 오브젝트만 그림자가 그려지며 메인카메라위치에서 메인카메라가 바라보는 방향으로 shadowAreaRange만큼 떨어진 위치에서 shadowAreaRange를 반지름으로 가지는 구의 바깥에 접하는 정육면체 공간이 그림자 영역이된다. (이 값이 커질수록 그림자의 품질이 낮아진다.) </param>
	/// <param name="[shadowAreaOffset] :"> 그림자영역의 범위를 카메라 방향으로 shadowAreaOffset의 크기만큼 당긴다.</param>
	/// <param name="[shadowMapSize] :"> shadowMap의 크기, width, height를 모두 이 값으로 사용한다. (이 값이 커질수록 그림자의 품질이 높아진다.) </param>
	virtual void SetExtraInitOption(float shadowAreaRange, float shadowAreaOffset, unsigned int shadowMapSize) abstract;

	/// <summary>
	/// 추가 옵션을 설정한다. BeginDraw 이전에 호출되어야 정상적으로 작동한다.
	/// </summary>
	/// <param name="[shadowMapDepthBias] :"> shadowMap의 DepthBias를 설정한다. (DepthBias는 ShadowMap과 ViewSpace의 좌표계 차이로 인한 텍스쳐의 계단현상을 완화시켜주며 빛과 그림자가 그려지는 오브젝트의 각도에 따라 변경이 필요할 수 있다.) </param>
	virtual void SetExtraOption(float shadowMapDepthBias) abstract;


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

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color color, Color outLineColor) abstract;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color outLineColor) abstract;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color outLineColor) abstract;
	virtual void DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, const std::wstring& firstAnimationName, const std::wstring& secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color outLineColor) abstract;

	// Light를 그린다. 같은 프레임에 동일한 라이트를 2개 이상 그린다면 한번만 적용한다. (여기서 동일한 라이트는 LightID가 같은 라이트를 말한다.)
	virtual void DrawLight(LightID lightID) abstract;

	// sprite를 원본 크기 그대로 position 위치에 그린다.
	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture, int layer) abstract;

	// sprite를 size크기로 position 위치에 그린다. (size를 (0, 0)으로 설정하면 원본 크기로 그려진다.)
	virtual void DrawSprite(const Eigen::Vector2f& position, const Eigen::Vector2f& size, TextureID texture, int layer) abstract;

	// 빌보드를 그린다.
	virtual void DrawBillBoard(const Eigen::Matrix4f& worldMatrix, TextureID texture, float ccwRadianAngle, bool keepOriginSize) abstract;

	// 큐브맵을 그린다. 마지막으로 그린 단 하나만 그려진다.
	virtual void DrawCubeMap(TextureID texture) abstract;

	// 문자열을 그린다.
	virtual void DrawString(const std::wstring& string, float x, float y, float width, float height, float fontSize, Color color) abstract;


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
	virtual LightID CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction) abstract;
	// Point Light 생성
	virtual LightID CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& position, float range) abstract;
	// Spot Light 생성
	virtual LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
	// Light 해제
	virtual void ReleaseLight(LightID lightID) abstract;

	// Light 정보 수정
	virtual void UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
};