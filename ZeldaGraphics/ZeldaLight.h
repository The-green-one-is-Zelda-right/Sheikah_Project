#pragma once

#include "ZeldaGraphicsDefine.h"

class ZeldaCamera;

class ZeldaLight
{
public:
	ZeldaLight(LightType type);
	ZeldaLight(const ZeldaLight& zeldaLight) = delete;
	~ZeldaLight();

	void SetAmbient(float red, float green, float blue);
	void SetDiffuseColor(float red, float green, float blue);
	void SetSpecular(float red, float green, float blue);
	void SetShadowColor(float red, float green, float blue);

	DirectX::XMFLOAT4 GetAmbient();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT4 GetSpecular();
	DirectX::XMFLOAT4 GetShadowColor();

	DirectX::XMMATRIX GetViewMatrix(ZeldaCamera* maincamera);
	DirectX::XMMATRIX GetOrthoMatrix();

	void CreatePointLightViewAndProjMatrices(DirectX::XMMATRIX viewMatrices[6], DirectX::XMMATRIX& projMatrix);

	// Directional Light
	void SetDirection(float x, float y, float z);

	DirectX::XMFLOAT3 GetDirection();

	// Point Light
	void SetRange(float range);
	void SetPosition(float x, float y, float z);
	// 1 / ( att0i + att1i * d + att2i * d²)
	void SetAttenuation(float atten0, float atten1, float atten2);

	float GetRange();
	DirectX::XMFLOAT3 GetPosition();

	// Spot Light
	void SetAngle(float angle);
	
	float GetAngle();

	LightType GetLightType();
	LightInfo GetLightInfo();

	DirectX::XMMATRIX GetWorldMatrix();

private:
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT4 shadowColor;

	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 direction;
	LightType type;
	float range;
	float angle;

	// Attenuation 이란...
	// https://learn.microsoft.com/en-us/windows/win32/direct3d9/attenuation-and-spotlight-factor

	// Unity에서는 1 / (25 * (d / r)^2 + 1) 과 유사한 공식을 사용한다고 한다.
	// https://geom.io/bakery/wiki/index.php?title=Point_Light_Attenuation

	float atten0;	// 0 ~ INF
	float atten1;	// 0 ~ INF
	float atten2;	// 0 ~ INF
};

