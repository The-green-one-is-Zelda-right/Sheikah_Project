#include "Buffers.hlsli"
#include "Matrix.hlsli"

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 position2 : POSITION;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float3 direction = normalize(input.position2);
    return cubeMap.Sample(Sampler, direction);
}