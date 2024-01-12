#include "Buffers.hlsli"

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 viewPosition : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelOutputType
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
    float4 depth : SV_Target3;
};

PixelOutputType main(PixelInputType input)
{
    PixelOutputType output = (PixelOutputType) 0;
    
    float4 textureColor;
    float3 lightDir;
    float diffuseIntensity;
    float specularIntensity = 0.0f;
    float4 color;
    
    if (useBaseColor)
    {
        textureColor = baseColor;
    }
    else
    {
        if (useDiffuse)
        {
            textureColor = DiffuseMap.Sample(Sampler, input.tex);

            if (useSRGB)
            {
                // SRGB �� �������� ���� �� ��������
                textureColor = pow(textureColor, 1.0f / 2.2f);
            }
        }
        else
        {
            textureColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    
    float4 projPos = mul(input.viewPosition, projectionMatrix);
    
    // Proj���� ��������
    // ProjPos.z�� far * (z - near) / (far - near)
    // ProjPos.w�� positionZ�� �ȴ�.
    // 
    float depth = projPos.z / cameraFar;
    
    output.position = float4(input.viewPosition.xyz, 1.0f);
    output.normal = float4(input.normal, 1.0f);
    output.color = textureColor;
    output.depth = float4(depth, depth, depth, 1.0f);
    
#ifdef _Debug

#endif
    
    if (depth > 1.0f)
    {
        output.depth = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    
    return output;
}