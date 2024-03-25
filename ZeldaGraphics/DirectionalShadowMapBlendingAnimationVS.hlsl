#include "Buffers.hlsli"
#include "Utility.hlsli"

struct VertexIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float4 clipPos : POSITIONT;
};

VertexOut main(VertexIn input)
{
    VertexOut output = (VertexOut) 0;
    
    // ������ ��� ����� ���� ��ġ ���͸� ���� ��ǥ�� ��ȯ�Ѵ�.
    input.position.w = 1.0f;
    
    float4x4 finalMatrix[4];
    finalMatrix[0] = finalTM[input.boneIndices[0]];
    finalMatrix[1] = finalTM[input.boneIndices[1]];
    finalMatrix[2] = finalTM[input.boneIndices[2]];
    finalMatrix[3] = finalTM[input.boneIndices[3]];
        
    // boneIndices�� weight�� �̿��� �ִϸ��̼ǵ� position�� ����Ѵ�.
    // boneIndices�� ���� unsigned int�� MAX���̸� ������ �޴� ���� ���°����� �����Ѵ�.
    output.pos =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.position);
    
    output.pos = mul(mul(mul(output.pos, worldMatrix), lightViewMatrix), lightProjectionMatrix);
    output.clipPos = output.pos;
    
    return output;
}