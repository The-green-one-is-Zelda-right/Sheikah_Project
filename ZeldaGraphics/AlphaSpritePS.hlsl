#include "Buffers.hlsli"

struct PixelInputType
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor = DiffuseMap.Sample(Sampler, input.uv);
    
    if (useSRGB)
    {
        // SRGB �� �������� ���� �� ��������
        textureColor = pow(textureColor, 1.0f / 2.2f);
    }
    
    // �ؽ����� ���� ���İ����� ���
    return float4(baseColor.rgb, baseColor.a * textureColor.r);
}