#ifndef __SKYBOXPS
#define __SKYBOXPS

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

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 Color = (float4) 0.f;

    
    return Color;
}

#endif