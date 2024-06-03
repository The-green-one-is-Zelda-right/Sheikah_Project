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
	/// Renderer�� �ʱ�ȭ
	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen) abstract;
	/// Renderer�� ����
	virtual void Finalize() abstract;

	// ȭ�� ũ�� �缳��
	virtual void Resize(unsigned int screenWidth, unsigned int screenHeight) abstract;

	/// <summary>
	/// �߰� �ɼ��� �����Ѵ�. Initialize�� ȣ��Ǳ� ������ ȣ��Ǿ�� ������ �ɼ��� ������ �� �ִ�.
	/// </summary>
	/// <param name="[shadowAreaRange] :"> �׸��ڿ����� ����, �� �����ȿ� �ִ� ������Ʈ�� �׸��ڰ� �׷����� ����ī�޶���ġ���� ����ī�޶� �ٶ󺸴� �������� shadowAreaRange��ŭ ������ ��ġ���� shadowAreaRange�� ���������� ������ ���� �ٱ��� ���ϴ� ������ü ������ �׸��� �����̵ȴ�. (�� ���� Ŀ������ �׸����� ǰ���� ��������.) </param>
	/// <param name="[shadowAreaOffset] :"> �׸��ڿ����� ������ ī�޶� �������� shadowAreaOffset�� ũ�⸸ŭ ����.</param>
	/// <param name="[shadowMapSize] :"> shadowMap�� ũ��, width, height�� ��� �� ������ ����Ѵ�. (�� ���� Ŀ������ �׸����� ǰ���� ��������.) </param>
	virtual void SetExtraInitOption(float shadowAreaRange, float shadowAreaOffset, unsigned int shadowMapSize) abstract;

	/// <summary>
	/// �߰� �ɼ��� �����Ѵ�. BeginDraw ������ ȣ��Ǿ�� ���������� �۵��Ѵ�.
	/// </summary>
	/// <param name="[shadowMapDepthBias] :"> shadowMap�� DepthBias�� �����Ѵ�. (DepthBias�� ShadowMap�� ViewSpace�� ��ǥ�� ���̷� ���� �ؽ����� ��������� ��ȭ�����ָ� ���� �׸��ڰ� �׷����� ������Ʈ�� ������ ���� ������ �ʿ��� �� �ִ�.) </param>
	virtual void SetExtraOption(float shadowMapDepthBias) abstract;


	// ����� ��带 �����Ѵ�.
	virtual void SetDebugMode(DebugMode mode) abstract;
	// ������ ��带 �����Ѵ�.
	virtual void SetRendererMode(RendererMode mode) abstract;

	// �׸��⸦ �غ��Ѵ�. Draw��� ������ �ݵ�� �ѹ� ȣ��Ǿ�� �Ѵ�. (deltaTime�� ������ second)
	virtual void BeginDraw(float deltaTime) abstract;
	// �׸��⸦ �����Ѵ�. Draw��� ���Ŀ� �ݵ�� �ѹ� ȣ��Ǿ�� �Ѵ�.
	virtual void EndDraw() abstract;



	// ### DrawXXXX() 
	// ���𰡸� �׸��� ���, ������ ID�� ����Ұ�� ����ȭ�ȴ�.

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color color, Color outLineColor) abstract;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color outLineColor) abstract;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color outLineColor) abstract;
	virtual void DrawChangingAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, const std::wstring& firstAnimationName, const std::wstring& secondAnimationName, float firstAnimationTime, float secondAnimationTime, float ratio, bool wireFrame, bool drawShadow, bool fastOutLine, bool outLine, Color outLineColor) abstract;

	// Light�� �׸���. ���� �����ӿ� ������ ����Ʈ�� 2�� �̻� �׸��ٸ� �ѹ��� �����Ѵ�. (���⼭ ������ ����Ʈ�� LightID�� ���� ����Ʈ�� ���Ѵ�.)
	virtual void DrawLight(LightID lightID) abstract;

	// sprite�� ���� ũ�� �״�� position ��ġ�� �׸���.
	virtual void DrawImage(const Eigen::Vector2f& position, TextureID texture, unsigned int layer) abstract;

	// sprite�� sizeũ��� position ��ġ�� �׸���. (size�� (0, 0)���� �����ϸ� ���� ũ��� �׷�����.)
	virtual void DrawImage(const Eigen::Vector2f& position, const Eigen::Vector2f& size, TextureID texture, unsigned int layer) abstract;

	/// <summary>
	/// �����带 �׸���.
	/// </summary>
	/// <param name="worldMatrix">: �߽� ��ġ </param>
	/// <param name="texture">: ����� �̹��� </param>
	/// <param name="ccwRadianAngle">: �������� �ݽð� ���� ȸ�� ��(Radian) </param>
	/// <param name="keepOriginSize">: 1x1ũ���� ���簢�� ��� ���� �̹����� ũ��(�ȼ� �� / 100)�� ����Ѵ�. </param>
	virtual void DrawBillBoard(const Eigen::Matrix4f& worldMatrix, TextureID texture, float ccwRadianAngle, bool keepOriginSize) abstract;

	/// <summary>
	/// Sprite�� �׸���.
	/// </summary>
	/// <param name="worldMatrix">: ���� ��ȯ ��� </param>
	/// <param name="texture">: ����� �̹��� </param>
	/// <param name="keepOriginSize">: 1x1ũ���� ���簢�� ��� ���� �̹����� ũ��(�ȼ� �� / 100)�� ����Ѵ�. </param>
	virtual void DrawSprite(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool keepOriginSize) abstract;

	// ť����� �׸���. ���������� �׸� �� �ϳ��� �׷�����.
	virtual void DrawCubeMap(TextureID texture) abstract;

	// ���ڿ��� �׸���.
	virtual void DrawString(const std::wstring& string, float x, float y, float width, float height, float fontSize, Color color) abstract;


	// ### CreateXXXX()
	// Resource�� �����ϴ� ���, ������ �ص� Draw����� �ϱ� ������ ȭ�鿡 �׷����� �ʴ´�.
	// ��, ī�޶�� Draw��� SetMainCamera ���

	// �ؽ��� ���� (������ ��η� �������� ID�� ���� �� ������ �׸��⿡ ������ ID�� ����� ��쿡�� ����ȭ��)
	virtual TextureID CreateTexture(const std::wstring& texturePath) abstract;
	// �ؽ��� ����
	virtual void ReleaseTexture(TextureID textureID) abstract;

	// �� ���� (������ ��η� �������� ID�� ���� �� ������ �׸��⿡ ������ ID�� ����� ��쿡�� ����ȭ��)
	virtual ModelID CreateModel(const std::wstring& modelingFilePath) abstract;
	// �� ����
	virtual void ReleaseModel(ModelID modelID) abstract;
	// �ִϸ��̼� �̸� ��� (ModelID�� ����ִ� ���� ���� ��� �ִϸ��̼��� �̸��� std::vector�� ��´�. �̸��� ������������ ���ĵȴ�.)
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) abstract;
	// �ִϸ��̼� ��� �ð� ��� (ModelID�� ����ִ� ���� ���� ��� �ִϸ��̼��� ����ð��� ��´�. �ش� �ִϸ��̼��� �̸��� ������������ ���ĵȴ�.)
	virtual std::vector<float> GetAnimationPlayTime(ModelID model) abstract;


	// ī�޶� ����
	virtual CameraID CreateCamera() abstract;
	// ī�޶� ����
	virtual void ReleaseCamera(CameraID cameraID) abstract;

	// ���� ī�޶� ����
	virtual bool SetMainCamera(CameraID cameraID) abstract;
	// ī�޶� ���� ����
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) abstract;


	// Directional Light ����
	virtual LightID CreateDirectionalLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction) abstract;
	// Point Light ����
	virtual LightID CreatePointLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& position, float range) abstract;
	// Spot Light ����
	virtual LightID CreateSpotLight(const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
	// Light ����
	virtual void ReleaseLight(LightID lightID) abstract;

	// Light ���� ����
	virtual void UpdateLight(LightID lightID, const Eigen::Vector3f& ambient, const Eigen::Vector3f& diffuse, const Eigen::Vector3f& specular, const Eigen::Vector3f& shadowColor, const Eigen::Vector3f& direction, const Eigen::Vector3f& position, float range, float angle) abstract;
};