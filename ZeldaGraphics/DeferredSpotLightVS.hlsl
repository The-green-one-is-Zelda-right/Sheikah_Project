#include "Buffers.hlsli"

struct VertexIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VertexOut main(VertexIn input)
{
    VertexOut output = (VertexOut) 0;
    
    input.position.w = 1.0f;
    
    output.pos = mul(mul(mul(input.position, worldMatrix), viewMatrix), projectionMatrix);
    output.uv = input.tex;
    
    matrix tmp = viewMatrix;
    
    return output;
}