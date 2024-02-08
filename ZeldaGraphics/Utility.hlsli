#ifndef _UTILITY_HLSLI_
#define _UTILITY_HLSLI_

#include "Buffers.hlsli"
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


// Buffers.hlsli 필요
// 0번째 애니메이션 데이터의 정보
// Frame 0 : TransformMatrix
// Frame 1 : OffsetMatrix
// Frame 2 : ParentID

matrix GetTransformMatrix(unsigned int boneID)
{
    float4 l0 = AnimationMap.Load(int4(4 * boneID + 0, 0, 0, 0));
    float4 l1 = AnimationMap.Load(int4(4 * boneID + 1, 0, 0, 0));
    float4 l2 = AnimationMap.Load(int4(4 * boneID + 2, 0, 0, 0));
    float4 l3 = AnimationMap.Load(int4(4 * boneID + 3, 0, 0, 0));
    
    return matrix(l0, l1, l2, l3);
}

matrix GetOffsetMatrix(unsigned int boneID)
{
    float4 l0 = AnimationMap.Load(int4(4 * boneID + 0, 1, 0, 0));
    float4 l1 = AnimationMap.Load(int4(4 * boneID + 1, 1, 0, 0));
    float4 l2 = AnimationMap.Load(int4(4 * boneID + 2, 1, 0, 0));
    float4 l3 = AnimationMap.Load(int4(4 * boneID + 3, 1, 0, 0));
    
    return matrix(l0, l1, l2, l3);
}

unsigned int GetParent(unsigned int boneID)
{
    return parentBone[boneID / 4][boneID % 4];
}

matrix GetWorldTM()
{
    // 안써도 될듯?
    return 0;
}

matrix GetFinalTM(unsigned int boneID, unsigned int animationID1, float frame1, unsigned int animationID2, float frame2, float ratio)
{
    float lower1 = floor(frame1);
    float upper1 = ceil(frame1);
    
    float ratio1 = 0.0f;
    if (upper1 != lower1)
    {
        ratio1 = (frame1 - lower1) / (upper1 - lower1);
    }
    
    float lower2 = floor(frame2);
    float upper2 = ceil(frame2);
    
    float ratio2 = 0.0f;
    if (upper2 != lower2)
    {
        ratio2 = (frame2 - lower2) / (upper2 - lower2);
    }
    
    unsigned int currentBoneID = boneID;
    
    // identity로 초기화
    matrix result = GetOffsetMatrix(boneID);
    
    while (true)
    {
        float4 a1l0 = AnimationMap.Load(int4(4 * currentBoneID + 0, lower1, animationID1, 0));
        float4 a1l1 = AnimationMap.Load(int4(4 * currentBoneID + 1, lower1, animationID1, 0));
        float4 a1l2 = AnimationMap.Load(int4(4 * currentBoneID + 2, lower1, animationID1, 0));
        float4 a1l3 = AnimationMap.Load(int4(4 * currentBoneID + 3, lower1, animationID1, 0));
    
        float4 a1u0 = AnimationMap.Load(int4(4 * currentBoneID + 0, upper1, animationID1, 0));
        float4 a1u1 = AnimationMap.Load(int4(4 * currentBoneID + 1, upper1, animationID1, 0));
        float4 a1u2 = AnimationMap.Load(int4(4 * currentBoneID + 2, upper1, animationID1, 0));
        float4 a1u3 = AnimationMap.Load(int4(4 * currentBoneID + 3, upper1, animationID1, 0));
    
        matrix a1lowerMatrix = matrix(a1l0, a1l1, a1l2, a1l3);
        matrix a1upperMatrix = matrix(a1u0, a1u1, a1u2, a1u3);
    
        
        float4 a2l0 = AnimationMap.Load(int4(4 * currentBoneID + 0, lower2, animationID2, 0));
        float4 a2l1 = AnimationMap.Load(int4(4 * currentBoneID + 1, lower2, animationID2, 0));
        float4 a2l2 = AnimationMap.Load(int4(4 * currentBoneID + 2, lower2, animationID2, 0));
        float4 a2l3 = AnimationMap.Load(int4(4 * currentBoneID + 3, lower2, animationID2, 0));
    
        float4 a2u0 = AnimationMap.Load(int4(4 * currentBoneID + 0, upper2, animationID2, 0));
        float4 a2u1 = AnimationMap.Load(int4(4 * currentBoneID + 1, upper2, animationID2, 0));
        float4 a2u2 = AnimationMap.Load(int4(4 * currentBoneID + 2, upper2, animationID2, 0));
        float4 a2u3 = AnimationMap.Load(int4(4 * currentBoneID + 3, upper2, animationID2, 0));
    
        matrix a2lowerMatrix = matrix(a2l0, a2l1, a2l2, a2l3);
        matrix a2upperMatrix = matrix(a2u0, a2u1, a2u2, a2u3);
        
        matrix animationMatrix = LerpTransformMatrix(LerpTransformMatrix(a1lowerMatrix, a1upperMatrix, ratio1), LerpTransformMatrix(a2lowerMatrix, a2upperMatrix, ratio2), ratio);
        
        result = mul(result, animationMatrix);
        
        unsigned int parentID = GetParent(currentBoneID);
        if (currentBoneID == parentID)
        {
            break;
        }
        else
        {
            currentBoneID = parentID;
        }
    }
    
    return result;
}

#endif