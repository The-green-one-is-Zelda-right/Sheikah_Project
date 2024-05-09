#include "Buffers.hlsli"
#include "Utility.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
    uint instance : SV_InstanceID;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 viewPosition : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 tex : TEXCOORD0;
    uint instance : InstanceID;
};

// Instancing Value 0 : Animation Frame Info        float4(frame, 0.0f, 0.0f, 0.0f)     // inst vs
// Instancing Value 1 : Color                       float4(r, g, b, a)                  // ps
// Instancing Value 2 : Not Used
// Instancing Value 3 : Not Used

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// ������ ��� ����� ���� ��ġ ���͸� ���� ��ǥ�� ��ȯ�Ѵ�.
    input.position.w = 1.0f;

    float4x4 finalMatrix[4];
    finalMatrix[0] = GetFinalTM(input.boneIndices[0], instancingValue0[input.instance].x);
    finalMatrix[1] = GetFinalTM(input.boneIndices[1], instancingValue0[input.instance].x);
    finalMatrix[2] = GetFinalTM(input.boneIndices[2], instancingValue0[input.instance].x);
    finalMatrix[3] = GetFinalTM(input.boneIndices[3], instancingValue0[input.instance].x);
        
    // boneIndices�� weight�� �̿��� �ִϸ��̼ǵ� position�� ����Ѵ�.
    // boneIndices�� ���� unsigned int�� MAX���̸� ������ �޴� ���� ���°����� �����Ѵ�.
    output.position =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.position);
        
    // normal�� �ִϸ��̼� ���
    output.normal =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.normal);
    
	// ����, ��, �������� ��ĵ��� �̿��� ������ ��ġ�� ����Ѵ�.
    output.position = mul(output.position, instancingWorldMatrix[input.instance]);
    output.position = mul(output.position, viewMatrix);
    
    output.viewPosition = float4(output.position.xyz, 1.0f);
    output.position = mul(output.position, projectionMatrix);
    
	// �Է¹��� ������ �״�� �ȼ� ���̴����� �̿��ϵ��� �����Ѵ�.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(mul(output.normal, (float3x3) instancingWorldMatrix[input.instance]), (float3x3) viewMatrix);
    output.tangent = mul(mul(input.tangent, (float3x3) instancingWorldMatrix[input.instance]), (float3x3) viewMatrix);
    output.biNormal = cross(output.tangent, output.normal);
    
    output.normal = (length(output.normal) > F_EPSILON) ? (normalize(output.normal)) : (float3(0.0f, 0.0f, 0.0f));
    output.tangent = (length(output.tangent) > F_EPSILON) ? (normalize(output.tangent)) : (float3(0.0f, 0.0f, 0.0f));
    output.biNormal = (length(output.biNormal) > F_EPSILON) ? (normalize(output.biNormal)) : (float3(0.0f, 0.0f, 0.0f));
    
    output.instance = input.instance;
    
    return output;
}