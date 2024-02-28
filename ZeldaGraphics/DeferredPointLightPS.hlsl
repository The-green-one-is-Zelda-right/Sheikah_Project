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
    
     // input.pos = SV_Position = Screen 좌표
    float2 uv = float2(input.pos.x / screenSize.x, input.pos.y / screenSize.y);
    
    float3 viewPos = Temp0Map.Sample(Sampler, uv).xyz;
    
    // 카메라보다 뒤에 있는 오브젝트 거르기
    if (viewPos.z <= 0.f)
    {
        clip(-1);
    }
    
    float3 viewLightPos = mul(float4(lights[lightIndex].position.xyz, 1.f), viewMatrix).xyz;
    float distance = length(viewPos - viewLightPos);
    
    // Light의 범위보다 멀리 있는 오브젝트 거르기
    if (distance > lights[lightIndex].range)
    {
        clip(-1);
    }
    
    float3 viewNormal = Temp1Map.Sample(Sampler, uv).xyz;
    
    LightColor color;
    
    CalculateLight(lightIndex, viewNormal, viewPos, color.diffuse, color.ambient, color.specular);
    
    output.diffuse = float4((color.diffuse + color.ambient).xyz, 1.0f);
    output.specular = float4(color.specular.xyz, 1.0f);
    
    return output;
}