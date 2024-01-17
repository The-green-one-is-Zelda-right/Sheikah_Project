#include "Buffers.hlsli"

struct PixelIn
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PixelIn input) : SV_Target
{
    float4 output = (float4) 0;
    
    float4 diffuseLight = Temp1Map.Sample(Sampler, input.uv);
    
    if (diffuseLight.x == 0.0f && diffuseLight.y == 0.0f, diffuseLight.z == 0.0f)
    {
        clip(-1);
    }
    
    float4 color = Temp0Map.Sample(Sampler, input.uv);
    float4 specular = Temp2Map.Sample(Sampler, input.uv);
    
    output = (color * diffuseLight) + specular;
    //output = float4(0, 0, 0, 1);
    return output;
}