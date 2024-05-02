#ifndef __SKYBOXVS
#define __SKYBOXVS

#include "struct.hlsli"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float LightPow : FOG;
       
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};


VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT OutPut = (VS_OUT) 0.f;
    
    _in.vPos *= 2.f;
    float3 viewPos = mul(float4(_in.vPos, 0.f), View).xyz;
    OutPut.vPosition = mul(float4(viewPos, 1.f), Projection);
    OutPut.vPosition.z = OutPut.vPosition.w;
    OutPut.vUV = _in.vUV;
    
    return OutPut;
}

#endif