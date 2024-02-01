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
    
    matrix animationMatrix[2][4];
    
    animationMatrix[0][0] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[0][1] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[0][2] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[0][3] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    
    animationMatrix[1][0] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[1][1] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[1][2] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    animationMatrix[1][3] = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    
    // 애니메이션이 있는 경우
    if (input.boneIndices[0] != 0xffffffffu)
    {
        float frameData[2] = { instancingAnimationInfo[input.instance].firstAnimationFrame, instancingAnimationInfo[input.instance].secondAnimationFrame };
        unsigned int idData[2] = { instancingAnimationInfo[input.instance].firstAnimationID, instancingAnimationInfo[input.instance].secondAnimationID };
        
        for (int cnt = 0; cnt < 2; cnt++)
        {
            float currentFrame = frameData[cnt];
            unsigned int currentID = idData[cnt];
            
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
            
                animationMatrix[cnt][i] = lerp(lowerMatrix, upperMatrix, ratio);
            }
        }
        
        float finalRatio = instancingAnimationInfo[input.instance].ratio;
        float4x4 finalMatrix[4];
        finalMatrix[0] = lerp(animationMatrix[0][0], animationMatrix[1][0], finalRatio);
        finalMatrix[1] = lerp(animationMatrix[0][1], animationMatrix[1][1], finalRatio);
        finalMatrix[2] = lerp(animationMatrix[0][2], animationMatrix[1][2], finalRatio);
        finalMatrix[3] = lerp(animationMatrix[0][3], animationMatrix[1][3], finalRatio);
        
        // boneIndices와 weight를 이용해 애니메이션된 position을 계산한다.
        // boneIndices의 값이 unsigned int의 MAX값이면 영향을 받는 본이 없는것으로 생각한다.
        output.position =
            mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
            mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
            mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
            mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3]));
        
            // normal의 애니메이션 계산
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