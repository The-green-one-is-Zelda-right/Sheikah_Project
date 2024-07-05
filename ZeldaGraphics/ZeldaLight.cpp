#include "ZeldaLight.h"

#include "ZeldaCamera.h"

using namespace DirectX;

ZeldaLight::ZeldaLight(LightType type) :
	ambient({ 0.2f, 0.2f, 0.2f, 1.0f }),
	diffuse({ 1.0f, 1.0f, 1.0f, 1.0f }),
	specular({ 1.0f, 1.0f, 1.0f, 1.0f }),
	shadowColor({ 0.2f, 0.2f, 0.2f, 1.0f }),
	position({ 0.0f, 0.0f, 0.0f, 0.0f }),
	direction({ 0.0f, 0.0f, 1.0f, 0.0f }),
	type(type),
	range(10.0f),
	angle(3.141592f * 0.1666f),
	atten0(1.0f),
	atten1(0.0f),
	atten2(1.0f)
{

}

ZeldaLight::~ZeldaLight()
{

}

void ZeldaLight::SetAmbient(float red, float green, float blue)
{
	ambient = XMFLOAT4(red, green, blue, 1.0f);
}

void ZeldaLight::SetDiffuseColor(float red, float green, float blue)
{
	diffuse = XMFLOAT4(red, green, blue, 1.0f);
}

void ZeldaLight::SetSpecular(float red, float green, float blue)
{
	specular = XMFLOAT4(red, green, blue, 1.0f);
}

void ZeldaLight::SetShadowColor(float red, float green, float blue)
{
	shadowColor = XMFLOAT4(red, green, blue, 1.0f);
}

DirectX::XMFLOAT4 ZeldaLight::GetAmbient()
{
	return ambient;
}

DirectX::XMFLOAT4 ZeldaLight::GetDiffuseColor()
{
	return diffuse;
}

DirectX::XMFLOAT4 ZeldaLight::GetSpecular()
{
	return specular;
}

DirectX::XMFLOAT4 ZeldaLight::GetShadowColor()
{
	return shadowColor;
}

DirectX::XMMATRIX ZeldaLight::GetViewMatrix(ZeldaCamera* maincamera)
{
	DirectX::XMMATRIX cameraTM = maincamera->GetTransformMatrix();
	DirectX::XMMATRIX cameraRotation = cameraTM;
	DirectX::XMMATRIX cameraTranslation = cameraTM;
	
	cameraRotation.r[3].m128_f32[0] = 0.0f;
	cameraRotation.r[3].m128_f32[1] = 0.0f;
	cameraRotation.r[3].m128_f32[2] = 0.0f;

	cameraTranslation.r[0].m128_f32[0] = 1.0f;
	cameraTranslation.r[0].m128_f32[1] = 0.0f;
	cameraTranslation.r[0].m128_f32[2] = 0.0f;
	cameraTranslation.r[1].m128_f32[0] = 0.0f;
	cameraTranslation.r[1].m128_f32[1] = 1.0f;
	cameraTranslation.r[1].m128_f32[2] = 0.0f;
	cameraTranslation.r[2].m128_f32[0] = 0.0f;
	cameraTranslation.r[2].m128_f32[1] = 0.0f;
	cameraTranslation.r[2].m128_f32[2] = 1.0f;
	
	DirectX::XMVECTOR cameraDirection = DirectX::XMVector3Normalize(DirectX::XMVector4Transform({ 0.0f, 0.0f, 1.0f, 1.0f }, cameraRotation));
	DirectX::XMVECTOR cameraPosition = { cameraTM.r[3].m128_f32[0], cameraTM.r[3].m128_f32[1], cameraTM.r[3].m128_f32[2], 0.0f };

	DirectX::XMMATRIX worldMatrix;
	switch (type)
	{
		case LightType::Directional:
		{
			DirectX::XMVECTOR lightDirection = DirectX::XMVectorSet(direction.x, direction.y, direction.z, 0.0f);
			DirectX::XMVECTOR zVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

			if (DirectX::XMVector4Equal(zVector, lightDirection)) {
				worldMatrix = DirectX::XMMatrixIdentity();
			}
			else
			{
				// �� ������ ������ ����Ͽ� �ڻ��� ������ ����
				float cosTheta = DirectX::XMVector4Dot(zVector, lightDirection).m128_f32[0];

				// v1�� targetV�� �����ϰų� �ݴ� ������ ��� �ƹ��� ȸ���� �ʿ����� ����
				if (cosTheta >= 0.999999f || cosTheta <= -0.999999f) {
					worldMatrix = DirectX::XMMatrixIdentity();
				}
				else
				{
					// ������ ���
					float theta = std::acos(cosTheta);

					// ȸ�� ���� ���
					DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zVector, lightDirection));

					// ȸ�� ��� ����
					worldMatrix = DirectX::XMMatrixRotationAxis(rotationAxis, theta);
				}
			}
			
			worldMatrix =
				worldMatrix *
				DirectX::XMMatrixTranslationFromVector(-ShadowMap::Range * DirectX::XMVector3Normalize({ direction.x, direction.y, direction.z, 0.0f })) *
				DirectX::XMMatrixTranslationFromVector((ShadowMap::Range - ShadowMap::Offset) * DirectX::XMVector3Normalize({ cameraDirection.m128_f32[0], cameraDirection.m128_f32[1], cameraDirection.m128_f32[2], 0.0f})) *
				DirectX::XMMatrixTranslationFromVector(cameraPosition);

			break;
		}

		case LightType::Point:
		{
			assert(0);
			break;
		}

		case LightType::Spot:
		{
			assert(0);
			break;
		}

		default:
			assert(0);
			break;
	}

	return DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

DirectX::XMMATRIX ZeldaLight::GetOrthoMatrix()
{
	return DirectX::XMMatrixOrthographicLH(2.0f * ShadowMap::Range, 2.0f * ShadowMap::Range, 0.0f, 2.0f * ShadowMap::Range);
}

void ZeldaLight::CreatePointLightViewAndProjMatrices(DirectX::XMMATRIX viewMatrices[6], DirectX::XMMATRIX& projMatrix)
{
	const float nearPlane = 0.0001f;
	const float farPlane = range;

	// ����Ʈ�� ��ġ
	XMVECTOR lightPosition = XMVectorSet(position.x, position.y, position.z, 1.0f);

	// �� ������ up ���Ϳ� lookAt ����
	XMVECTOR upVectors[6] = {
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),  // Right (positive X)
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),  // Left (negative X)
		XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), // Up (positive Y)
		XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),  // Down (negative Y)
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),  // Forward (positive Z)
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // Backward (negative Z)
	};

	XMVECTOR lookAtVectors[6] = {
		XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),  // Right (positive X)
		XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), // Left (negative X)
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),  // Up (positive Y)
		XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f), // Down (negative Y)
		XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),  // Forward (positive Z)
		XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)  // Backward (negative Z)
	};

	// 6���� �� ��� ����
	for (int i = 0; i < 6; ++i)
	{
		viewMatrices[i] = XMMatrixLookToLH(lightPosition, lookAtVectors[i], upVectors[i]);
	}

	// ���� ��� ���� (90�� �þ߰�, 1:1 ��Ⱦ��, nearPlane, farPlane)
	projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, nearPlane, farPlane);
}

void ZeldaLight::SetDirection(float x, float y, float z)
{
	assert(type == LightType::Directional);
	direction = XMFLOAT4(x, y, z, 0.0f);
}

DirectX::XMFLOAT3 ZeldaLight::GetDirection()
{
	assert(type == LightType::Directional);
	return XMFLOAT3(direction.x, direction.y, direction.z);
}

void ZeldaLight::SetRange(float range)
{
	assert(type == LightType::Point || type == LightType::Spot);
	this->range = range;
}

void ZeldaLight::SetPosition(float x, float y, float z)
{
	assert(type == LightType::Point || type == LightType::Spot);
	this->position = XMFLOAT4(x, y, z, 0.0f);
}

void ZeldaLight::SetAttenuation(float atten0, float atten1, float atten2)
{
	this->atten0 = atten0;
	this->atten1 = atten1;
	this->atten2 = atten2;
}

float ZeldaLight::GetRange()
{
	assert(type == LightType::Point || type == LightType::Spot);
	return range;
}

DirectX::XMFLOAT3 ZeldaLight::GetPosition()
{
	assert(type == LightType::Point || type == LightType::Spot);
	return XMFLOAT3(position.x, position.y, position.z);
}

void ZeldaLight::SetAngle(float angle)
{
	assert(type == LightType::Spot);
	this->angle = angle;
}

float ZeldaLight::GetAngle()
{
	assert(type == LightType::Spot);
	return angle;
}

LightType ZeldaLight::GetLightType()
{
	return type;
}

LightInfo ZeldaLight::GetLightInfo()
{
	LightInfo lightInfo;
	lightInfo.color.ambient = ambient;
	lightInfo.color.diffuse = diffuse;
	lightInfo.color.specular = specular;
	lightInfo.color.shadow = shadowColor;
	lightInfo.position = position;
	lightInfo.direction = direction;
	lightInfo.type = type;
	lightInfo.range = range;
	lightInfo.angle = angle;
	lightInfo.atten0 = atten0;
	lightInfo.atten1 = atten1;
	lightInfo.atten2 = atten2;

	return lightInfo;
}

DirectX::XMMATRIX ZeldaLight::GetWorldMatrix()
{
	switch (type)
	{
		case LightType::Directional:
		{
			// Directional Light�� WorldMatrix�� ����� �ʿ䰡 ���� ������ �׳� ������ �Ѱ��ش�.
			return DirectX::XMMatrixIdentity();
			break;
		}

		case LightType::Point:
		{
			float extra = 0.1f;

			DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(2.0f * range, 2.0f * range, 2.0f * range);
			DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

			return scaleMatrix * translation;
			break;
		}

		case LightType::Spot:
		{
			DirectX::XMVECTOR currentDir = { 0.0f, -1.0f, 0.0f, 0.0f };
			DirectX::XMVECTOR targetDir = XMVector3Normalize({ direction.x, direction.y, direction.z, 0.0f });
			DirectX::XMVECTOR axis = DirectX::XMVector3Cross(currentDir, targetDir);
			float angle = DirectX::XMScalarACos(DirectX::XMVector3Dot(currentDir, targetDir).m128_f32[0]);

			DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(range, range, range);
			DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationAxis(axis, angle);
			DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
			return scale * rotation * translation;
			break;
		}

		default:
			assert(0);
			break;
	}
}
