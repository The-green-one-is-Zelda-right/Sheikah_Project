#include "Buffers.hlsli"

#include "Light.hlsli"

struct PixelIn
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

PS_OUT main(PixelIn input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float3 viewPos = Temp0Map.Sample(Sampler, input.uv).xyz;
    if (viewPos.z <= 0.f)
    {
        clip(-1);
    }
    
    float3 viewNormal = Temp1Map.Sample(Sampler, input.uv).xyz;
    
    LightColor color;
    
    CalculateLight(lightIndex, viewNormal, viewPos, color.diffuse, color.ambient, color.specular);
    
    output.diffuse = float4((color.diffuse + color.ambient).xyz, 1.0f);
    output.specular = float4((color.specular).xyz, 1.0f);
    
    
    
    //float dx = 2.0f / 1920.0f;
    //float dy = 2.0f / 1080.0f;
    
    //bool same = true;
    
    //for (int i = 0; i < 9; i++)
    //{
    //    float3 tempNor = Temp1Map.Sample(Sampler, input.uv + float2((i / 3 - 2) * dx, ((i % 3) - 2) * dy)).xyz;
        
    //    if (tempNor.x != viewNormal.x || tempNor.y != viewNormal.y || tempNor.z != viewNormal.z)
    //    {
    //        same = false;
    //    }
    //}
    
    //if (same)
    //{
    //    output.diffuse = float4(0.0f, 0.0f, 1.0f, 1.0f);
    //    output.specular = float4(0.0f, 0.0f, 1.0f, 1.0f);
    //}
    //else
    //{
    //    output.diffuse = float4(1.0f, 0.0f, 0.0f, 1.0f);
    //    output.specular = float4(1.0f, 0.0f, 0.0f, 1.0f);
    //}
    
    return output;
}