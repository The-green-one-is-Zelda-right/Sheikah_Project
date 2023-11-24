cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
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
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// ������ ��� ����� ���� ��ġ ���͸� ���� ��ǥ�� ��ȯ�Ѵ�.
    input.position.w = 1.0f;

	// ����, ��, �������� ��ĵ��� �̿��� ������ ��ġ�� ����Ѵ�.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// �Է¹��� ������ �״�� �ȼ� ���̴����� �̿��ϵ��� �����Ѵ�.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}