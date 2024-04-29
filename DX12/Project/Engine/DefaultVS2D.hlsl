#ifndef _DEFAULTVS2D
#define _DEFAULTVS2D

#include "struct.hlsli"

struct VS_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    //float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    //float2 vUV : TEXCOORD;
};

VS_OUT VS_Default2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos, 1.f);
    //output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vColor = _in.vColor;
    
    return output;
}

#endif