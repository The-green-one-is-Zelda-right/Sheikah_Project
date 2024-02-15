#ifndef _UTILITY_HLSLI_
#define _UTILITY_HLSLI_

#include "Buffers.hlsli"

matrix GetFinalTM(unsigned int boneID, float frame)
{
    int iframe = (0.0f <= frame && frame <= (float) ANIMATION_TICK_MAX) * (round(frame)) +((float) ANIMATION_TICK_MAX < frame) * (ANIMATION_TICK_MAX);
    
    float4 m0 = AnimationMap.Load(int3(4 * boneID + 0, iframe, 0));
    float4 m1 = AnimationMap.Load(int3(4 * boneID + 1, iframe, 0));
    float4 m2 = AnimationMap.Load(int3(4 * boneID + 2, iframe, 0));
    float4 m3 = AnimationMap.Load(int3(4 * boneID + 3, iframe, 0));
    
    return matrix(m0, m1, m2, m3);
}

#endif