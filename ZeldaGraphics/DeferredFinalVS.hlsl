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
    
    output.pos = float4((input.position * 2.0f).xyz, 1.0f);
    output.uv = input.tex;
    
    matrix tmp = viewMatrix;
    
    return output;
}