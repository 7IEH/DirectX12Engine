#ifndef _DEFAULTVS2D
#define _DEFAULTVS2D

#include "struct.hlsli"

struct VS_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNoraml : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNoraml : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

VS_OUT VS_Default2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vViewPos = mul(float4(_in.vPos, 1.f), WV).xyz;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    output.vViewNoraml = normalize(mul(float4(_in.vNoraml, 0.f), WV).xyz);
    return output;
}

#endif