#ifndef _DEFAULTPS2D
#define _DEFAULTPS2D

//#include "struct.hlsli"
//#include "func.hlsli"

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    //float2 vUV : TEXCOORD;
};

float4 PS_Default2D(VS_OUT _in) : SV_Target
{
    float4 Color = float4(1.f, 1.f, 1.f, 1.f);
    
    return Color;
}
#endif