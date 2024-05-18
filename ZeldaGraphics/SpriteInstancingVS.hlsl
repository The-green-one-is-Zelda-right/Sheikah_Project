#include "Buffers.hlsli"

struct VertexIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
    uint instance : SV_InstanceID;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VertexOut main(VertexIn input)
{
    input.position.w = 1.0f;
    
    VertexOut output = (VertexOut) 0;
    
    output.pos = input.position;
    
    // ����, ��, �������� ��ĵ��� �̿��� ������ ��ġ�� ����Ѵ�.
    output.pos = mul(output.pos, instancingWorldMatrix[input.instance]);
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);
    
    output.uv = input.tex;
    
    return output;
}