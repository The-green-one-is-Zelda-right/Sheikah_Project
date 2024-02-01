#ifndef _UTILITY_HLSLI_
#define _UTILITY_HLSLI_

#include "Matrix.hlsli"
#include "Quaternion.hlsli"

float4x4 LerpTransformMatrix(float4x4 matrix1, float4x4 matrix2, float ratio)
{
    float3 pos1;
    float4 rotation1;
    float3 scale1;
    
    float3 pos2;
    float4 rotation2;
    float3 scale2;
    
    decompose(matrix1, pos1, rotation1, scale1);
    decompose(matrix2, pos2, rotation2, scale2);
    
    float3 finalPos = lerp(pos1, pos2, ratio);
    float4 finalRotation = q_slerp(rotation1, rotation2, ratio);
    float3 finalScale = lerp(scale1, scale2, ratio);

    return compose(finalPos, finalRotation, finalScale);
}

#endif