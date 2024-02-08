#include "Buffers.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 position2 : POSITION;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType) 0;
    
    input.position.w = 1.0f;
    
    matrix moveMatrix = worldMatrix;
    moveMatrix[0][0] = 1.0f;
    moveMatrix[0][1] = 0.0f;
    moveMatrix[0][2] = 0.0f;
    
    moveMatrix[1][0] = 0.0f;
    moveMatrix[1][1] = 1.0f;
    moveMatrix[1][2] = 0.0f;
    
    moveMatrix[2][0] = 0.0f;
    moveMatrix[2][1] = 0.0f;
    moveMatrix[2][2] = 1.0f;
    
    output.position = mul(mul(input.position, moveMatrix), viewMatrix);
    
    //output.position.xyz = output.position.xyz * (cameraFar / output.position.z);
    output.position = mul(output.position, projectionMatrix).xyww;
    
    //output.position.z = output.position.w - 0.0001f;
    
    output.position2 = input.position;
    
    return output;
}