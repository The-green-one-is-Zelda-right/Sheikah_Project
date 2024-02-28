#include "Buffers.hlsli"
#include "Utility.hlsli"

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
	
	// 적절한 행렬 계산을 위해 위치 벡터를 동차 좌표로 변환한다.
    input.position.w = 1.0f;

    float4x4 finalMatrix[4];
    finalMatrix[0] = GetFinalTM(input.boneIndices[0], instancingAnimationInfo[input.instance].time);
    finalMatrix[1] = GetFinalTM(input.boneIndices[1], instancingAnimationInfo[input.instance].time);
    finalMatrix[2] = GetFinalTM(input.boneIndices[2], instancingAnimationInfo[input.instance].time);
    finalMatrix[3] = GetFinalTM(input.boneIndices[3], instancingAnimationInfo[input.instance].time);
        
    // boneIndices와 weight를 이용해 애니메이션된 position을 계산한다.
    // boneIndices의 값이 unsigned int의 MAX값이면 영향을 받는 본이 없는것으로 생각한다.
    output.position =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.position);
        
    // normal의 애니메이션 계산
    output.normal =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.normal, (float3x3) finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.normal);
    
	// 월드, 뷰, 프로젝션 행렬들을 이용해 정점의 위치를 계산한다.
    output.position = mul(output.position, instancingWorldMatrix[input.instance]);
    output.position = mul(output.position, viewMatrix);
    
    output.viewPosition = float4(output.position.xyz, 1.0f);
    output.position = mul(output.position, projectionMatrix);
    
	// 입력받은 색상을 그대로 픽셀 셰이더에서 이용하도록 저장한다.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(mul(output.normal, (float3x3) instancingWorldMatrix[input.instance]), (float3x3) viewMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}