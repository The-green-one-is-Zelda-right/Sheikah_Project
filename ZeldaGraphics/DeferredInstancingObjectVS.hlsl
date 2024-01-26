#include "Buffers.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
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
    float2 tex : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// ������ ��� ����� ���� ��ġ ���͸� ���� ��ǥ�� ��ȯ�Ѵ�.
    input.position.w = 1.0f;
    
    matrix animationMatrix[4];
    
    animationMatrix[0] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[1] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[2] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[3] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    
    // �ִϸ��̼��� �ִ� ���
    if (input.boneIndices[0] != 0xffffffffu)
    {
        float currentFrame = animationFrame[input.instance / 4][input.instance % 4];
        unsigned int currentID = animationID[input.instance / 4][input.instance % 4];
        
        float lower = floor(currentFrame);
        float upper = ceil(currentFrame);
        
        float ratio = 0.0f;
        if (upper != lower)
        {
            ratio = (currentFrame - lower) / (upper - lower);
        }
        
        for (int i = 0; i < 4; i++)
        {
            if (input.boneIndices[i] == 0xffffffffu)
            {
                continue;
            }
            
            matrix lowerMatrix = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            matrix upperMatrix = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            
            float4 l0 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 0, lower, currentID, 0));
            float4 l1 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 1, lower, currentID, 0));
            float4 l2 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 2, lower, currentID, 0));
            float4 l3 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 3, lower, currentID, 0));
            
            float4 u0 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 0, upper, currentID, 0));
            float4 u1 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 1, upper, currentID, 0));
            float4 u2 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 2, upper, currentID, 0));
            float4 u3 = AnimationMap.Load(int4(4 * input.boneIndices[i] + 3, upper, currentID, 0));
            
            lowerMatrix = matrix(l0, l1, l2, l3);
            upperMatrix = matrix(u0, u1, u2, u3);
            
            animationMatrix[i] = lerp(lowerMatrix, upperMatrix, ratio);
        }
        
        // boneIndices�� weight�� �̿��� �ִϸ��̼ǵ� position�� ����Ѵ�.
        // boneIndices�� ���� unsigned int�� MAX���̸� ������ �޴� ���� ���°����� �����Ѵ�.
        output.position =
            mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, animationMatrix[0]), input.weight[0])) +
            mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, animationMatrix[1]), input.weight[1])) +
            mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, animationMatrix[2]), input.weight[2])) +
            mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, animationMatrix[3]), input.weight[3]));
        
        
        // normal�� �ִϸ��̼� ���
        output.normal =
            mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.normal, (float3x3) animationMatrix[0]), input.weight[0])) +
            mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.normal, (float3x3) animationMatrix[1]), input.weight[1])) +
            mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.normal, (float3x3) animationMatrix[2]), input.weight[2])) +
            mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.normal, (float3x3) animationMatrix[3]), input.weight[3])) +
            mul(input.boneIndices[0] == 0xffffffffu, input.normal);
    }
    else
    {
        output.position = input.position;
        output.normal = input.normal;
    }
    
	// ����, ��, �������� ��ĵ��� �̿��� ������ ��ġ�� ����Ѵ�.
    output.position = mul(output.position, instancingWorldMatrix[input.instance]);
    output.position = mul(output.position, viewMatrix);
    
    output.viewPosition = float4(output.position.xyz, 1.0f);
    output.position = mul(output.position, projectionMatrix);
    
	// �Է¹��� ������ �״�� �ȼ� ���̴����� �̿��ϵ��� �����Ѵ�.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(mul(output.normal, (float3x3) instancingWorldMatrix[input.instance]), (float3x3) viewMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}