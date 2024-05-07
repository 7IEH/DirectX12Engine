#ifndef _ANIMATIONCS
#define _ANIMATIONCS

#include "math.hlsli"

struct AnimFrameParams
{
    float4 scale;
    float4 rotation;
    float4 translation;
};

StructuredBuffer<AnimFrameParams> g_bone_frame : register(t11);
StructuredBuffer<matrix> g_offset : register(t12);
RWStructuredBuffer<matrix> g_final : register(u0);

// ComputeAnimation
// int0 : BoneCount
// int1 : CurrentFrame
// int2 : NextFrame
// float0 : Ration

[numthreads(256, 1, 1)]
void CS_Main(uint3 threadIdx : SV_DispatchThreadID)
{
    if ((uint) gMatrial._int0 <= threadIdx.x)
        return;
    
    int boneCount = gMatrial._int0;
    int currentFrame = gMatrial._int1;
    int nextFrame = gMatrial._int2;
    int ratio = gMatrial._float0;
    
    uint idx = (boneCount * currentFrame) + threadIdx.x;
    uint nextidx = (boneCount * nextFrame) + threadIdx.x;

    float4 quaternionZero = float4(0.f, 0.f, 0.f, 1.f);
    
    float4 scale = lerp(g_bone_frame[idx].scale, g_bone_frame[nextidx].scale, ratio);
    float4 rotation = QuaternionSlerp(g_bone_frame[idx].rotation, g_bone_frame[nextidx].rotation, ratio);
    float4 translation = lerp(g_bone_frame[idx].translation, g_bone_frame[nextidx].translation, ratio);

    matrix matBone = MatrixAffineTransformation(scale, quaternionZero, rotation, translation);
    
    g_final[threadIdx.x] = mul(g_offset[threadIdx.x], matBone);
}

#endif