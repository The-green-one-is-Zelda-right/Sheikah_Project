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

	DirectX::XMFLOAT4 GetAmbient();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT4 GetSpecular();

	// Directional Light
	void SetDirection(float x, float y, float z);

	DirectX::XMFLOAT3 GetDirection();

	// Point Light
	void SetRange(float range);
	void SetPosition(float x, float y, float z);

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

	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 direction;
	LightType type;
	float range;
	float angle;
};

