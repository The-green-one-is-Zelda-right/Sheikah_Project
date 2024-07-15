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
    uint instance : SV_InstanceID;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float4 clipPos : POSITIONT;
};

// Instancing Value 0 : Animation Frame Info        float4(frame, 0.0f, 0.0f, 0.0f)
// Instancing Value 1 : Not Used
// Instancing Value 2 : Not Used
// Instancing Value 3 : Not Used

VertexOut main(VertexIn input)
{
    VertexOut output = (VertexOut) 0;
    
    // ������ ��� ����� ���� ��ġ ���͸� ���� ��ǥ�� ��ȯ�Ѵ�.
    input.position.w = 1.0f;

    float4x4 finalMatrix[4];
    finalMatrix[0] = GetFinalTM(input.boneIndices[0], instancingValue0[input.instance].x);
    finalMatrix[1] = GetFinalTM(input.boneIndices[1], instancingValue0[input.instance].x);
    finalMatrix[2] = GetFinalTM(input.boneIndices[2], instancingValue0[input.instance].x);
    finalMatrix[3] = GetFinalTM(input.boneIndices[3], instancingValue0[input.instance].x);
        
    // boneIndices�� weight�� �̿��� �ִϸ��̼ǵ� position�� ����Ѵ�.
    // boneIndices�� ���� unsigned int�� MAX���̸� ������ �޴� ���� ���°����� �����Ѵ�.
    output.pos =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.position);
    
    //output.pos = float4(output.pos.xyz * 0.98, 1.0f); // �׸��ڰ� �����ϰ� ������ �ϱ� ����, ũ�⸦ ��¦ �ٿ��� �׸���.
    output.pos = mul(mul(mul(output.pos, instancingWorldMatrix[input.instance]), lightViewMatrix[0]), lightProjectionMatrix);
    output.clipPos = output.pos;
    
    return output;
}