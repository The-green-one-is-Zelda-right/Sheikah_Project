cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer BoneBufferType : register(b1)
{
    matrix boneTM[256];
};

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
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// ������ ��� ����� ���� ��ġ ���͸� ���� ��ǥ�� ��ȯ�Ѵ�.
    input.position.w = 1.0f;
        
    // boneIndices�� weight�� �̿��� �ִϸ��̼ǵ� position�� ����Ѵ�.
    // boneIndices�� ���� unsigned int�� MAX���̸� ������ �޴� ���� ���°����� �����Ѵ�.
    output.position =
    mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, boneTM[input.boneIndices[0]]), input.weight[0])) +
    mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, boneTM[input.boneIndices[1]]), input.weight[1])) +
    mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, boneTM[input.boneIndices[2]]), input.weight[2])) +
    mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, boneTM[input.boneIndices[3]]), input.weight[3])) +
    mul(input.boneIndices[0] == 0xffffffffu, input.position);
    
	// ����, ��, �������� ��ĵ��� �̿��� ������ ��ġ�� ����Ѵ�.
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// �Է¹��� ������ �״�� �ȼ� ���̴����� �̿��ϵ��� �����Ѵ�.
    output.tex = input.tex;
    
    // normal�� �ִϸ��̼� ���
    output.normal =
    mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.normal, (float3x3)boneTM[input.boneIndices[0]]), input.weight[0])) +
    mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.normal, (float3x3)boneTM[input.boneIndices[1]]), input.weight[1])) +
    mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.normal, (float3x3)boneTM[input.boneIndices[2]]), input.weight[2])) +
    mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.normal, (float3x3)boneTM[input.boneIndices[3]]), input.weight[3])) +
    mul(input.boneIndices[0] == 0xffffffffu, input.normal);
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(output.normal, (float3x3) worldMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}