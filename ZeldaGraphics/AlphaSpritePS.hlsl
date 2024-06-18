#include "Buffers.hlsli"

struct PixelInputType
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    uint instance : InstanceID;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor = DiffuseMap.Sample(Sampler, input.uv);
    
    if (useSRGB)
    {
        // SRGB �� �������� ���� �� ��������
        textureColor = pow(textureColor, 1.0f / 2.2f);
    }
    
    if (input.instance != 0xffffffffu)
    {
        return float4(instancingValue0[input.instance].rgb, instancingValue0[input.instance].a * textureColor.r);
    }
    
    // �ؽ����� ���� ���İ����� ���
    return float4(baseColor.rgb, baseColor.a * textureColor.r);
}