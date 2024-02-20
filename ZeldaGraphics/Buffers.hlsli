#ifndef _BUFFERS_HLSLI_
#define _BUFFERS_HLSLI_

#define LIGHT_TYPE_DIRECTIONAL 0u
#define LIGHT_TYPE_POINT 1u
#define LIGHT_TYPE_SPOT 2u

#define LIGHT_COUNT_MAX 50u

#define BONE_COUNT_MAX 256u
#define ANIMATION_TICK_MAX 8192u

#define INSTANCING_MAX 1024u

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D ARMMap : register(t2);
Texture2D HeightMap : register(t3);
Texture2D EmissionMap : register(t4);
Texture2D Temp0Map : register(t5);
Texture2D Temp1Map : register(t6);
Texture2D Temp2Map : register(t7);
Texture2D AnimationMap : register(t8);
TextureCube cubeMap : register(t9);

SamplerState Sampler : register(s0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    float cameraFar;
    float3 b0padding;
};

struct AnimationInfo
{
    float time;
    float3 padding;
};

cbuffer AnimationBufferType : register(b1)
{
    AnimationInfo animationInfo;
};

struct LightColor
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

struct LightInfo
{
    LightColor color;
    float4 position;
    float4 direction;
    unsigned int type;
    float range;
    float angle;
    float padding;
};

cbuffer LightInfoBufferType : register(b2)
{
    LightInfo lights[LIGHT_COUNT_MAX];
};

cbuffer LightIndexBufferType : register(b3)
{
    unsigned int lightIndex;
    float3 b3padding;
};

cbuffer MaterialBufferType : register(b4)
{
    float4 baseColor;
    
    unsigned int useBaseColor;
    unsigned int useSRGB;
    unsigned int useDiffuse;
    unsigned int useNormal;
    
    unsigned int useARM;
    unsigned int useHeight;
    unsigned int useEmission;
    unsigned int useTemp0;
    
    unsigned int useTemp1;
    unsigned int useTemp2;
    unsigned int useTemp3;
    unsigned int useTemp4;
};

cbuffer ScreenBufferType : register(b5)
{
    float2 screenSize;
    float2 b5padding;
}

cbuffer InstancingMatrixBufferType : register(b6)
{
    matrix instancingWorldMatrix[INSTANCING_MAX];
};

cbuffer InstancingAnimationBufferType : register(b7)
{
    AnimationInfo instancingAnimationInfo[INSTANCING_MAX];
}

cbuffer BlendingAnimationBufferType : register(b8)
{
    matrix finalTM[BONE_COUNT_MAX];
}

#endif