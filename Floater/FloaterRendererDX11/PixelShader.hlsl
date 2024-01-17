Texture2D textureMap : register(t0);
SamplerState g_Sampler : register(s0);


struct VS_OUTPUT
{
    float4 Position : SV_Position;
    //float3 PositionW : POSITION;
    //float2 UV : TEXCOORD;
    //float3 Normal : NORMAL;
};

struct PS_OUTPUT
{
    float4 test : SV_Target0;
};

PS_OUTPUT main(VS_OUTPUT input) : SV_Target
{
    PS_OUTPUT output;

    //float4 texColor = textureMap.Sample(g_Sampler, input.UV);
    //float4 finalColor = texColor;
    
    output.test = float4(1, 1, 1, 1);
    return output;
}