#include "Buffers.hlsli"
#include "Utility.hlsli"

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
    float4 viewPosition : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// ������ ��� ����� ���� ��ġ ���͸� ���� ��ǥ�� ��ȯ�Ѵ�.
    input.position.w = 1.0f;
    
    // �ִϸ��̼��� �ִ� ���
    if (input.boneIndices[0] != 0xffffffffu)
    {
        float frameData[2] = { animationInfo.firstAnimationFrame, animationInfo.secondAnimationFrame };
        unsigned int idData[2] = { animationInfo.firstAnimationID, animationInfo.secondAnimationID };
        float finalRatio = animationInfo.ratio;
        
        float4x4 finalMatrix[4];
        finalMatrix[0] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        finalMatrix[1] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        finalMatrix[2] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        finalMatrix[3] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        
        if (input.boneIndices[0] != 0xffffffffu) finalMatrix[0] = GetFinalTM(input.boneIndices[0], idData[0], frameData[0], idData[1], frameData[1], finalRatio);
        if (input.boneIndices[1] != 0xffffffffu) finalMatrix[1] = GetFinalTM(input.boneIndices[1], idData[0], frameData[0], idData[1], frameData[1], finalRatio);
        if (input.boneIndices[2] != 0xffffffffu) finalMatrix[2] = GetFinalTM(input.boneIndices[2], idData[0], frameData[0], idData[1], frameData[1], finalRatio);
        if (input.boneIndices[3] != 0xffffffffu) finalMatrix[3] = GetFinalTM(input.boneIndices[3], idData[0], frameData[0], idData[1], frameData[1], finalRatio);
        
        // boneIndices�� weight�� �̿��� �ִϸ��̼ǵ� position�� ����Ѵ�.
        // boneIndices�� ���� unsigned int�� MAX���̸� ������ �޴� ���� ���°����� �����Ѵ�.
        output.position =
            mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
            mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
            mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
            mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3]));
        
            // normal�� �ִϸ��̼� ���
        output.normal =
            mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[0]), input.weight[0])) +
            mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[1]), input.weight[1])) +
            mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[2]), input.weight[2])) +
            mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[3]), input.weight[3]));
    }
    else
    {
        output.position = input.position;
        output.normal = input.normal;
    }
    
	// ����, ��, �������� ��ĵ��� �̿��� ������ ��ġ�� ����Ѵ�.
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    
    output.viewPosition = float4(output.position.xyz, 1.0f);
    
    output.position = mul(output.position, projectionMatrix);
    
	// �Է¹��� ������ �״�� �ȼ� ���̴����� �̿��ϵ��� �����Ѵ�.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(mul(output.normal, (float3x3) worldMatrix), (float3x3)viewMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}