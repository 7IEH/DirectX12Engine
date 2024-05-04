#ifndef _DEFAULTVS3D
#define _DEFAULTVS3D

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

VS_OUT VS_Default3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vUV = _in.vUV;

    output.vViewPos = mul(float4(_in.vPos, 1.f), WV).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), WV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), WV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), WV).xyz);
    
    return output;
}

#endif