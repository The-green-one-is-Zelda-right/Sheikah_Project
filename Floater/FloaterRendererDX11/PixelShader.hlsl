Texture2D textureMap : register(t0);
SamplerState g_Sampler : register(s0);


struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 PositionW : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	// 텍스처 샘플링
    //float4 texColor = textureMap.Sample(g_Sampler, input.UV);
    //float4 finalColor = texColor;
    
    float4 finalColor;
    finalColor.r = 1.0f;
    finalColor.g = 1.0f;
    finalColor.b = 1.0f;
    finalColor.a = 1.0f;
    return finalColor;
}