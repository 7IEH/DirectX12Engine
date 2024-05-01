#ifndef _DEFAULTPS2D
#define _DEFAULTPS2D

#include "struct.hlsli"
//#include "func.hlsli"

static float3 g_LightPos = float3(0.f, 0.f, 0.f);
static float3 g_LightDir = float3(1.f, 0.f, 0.f);
static float3 g_LightColor = float3(1.f, 1.f, 1.f);
static float3 g_LightAmbient = float3(0.1f, 0.1f, 0.1f);
static float3 g_SpecularRatio = float3(0.3f, 0.3f, 0.3f);

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

float4 PS_Default2D(VS_OUT _in) : SV_Target
{
    float4 Color = (float4) 0.f;
    Color = float4(1.f, 1.f, 1.f, 1.f);
   
    float3 viewNormal = _in.vViewNoraml;
    
    // NormalMapping
    
    float3 viewLightDir = mul(float4(g_LightDir, 0.f), View).xyz;
    viewLightDir = normalize(viewLightDir);
    float fLightPow = saturate(dot(viewNormal, -viewLightDir));
                                         
                                        // 2 * ½ºÄ®¶ó * ¹æÇâ
    float3 fReflectDir = viewLightDir + 2 * dot(-viewLightDir, viewNormal) * viewNormal;
    fReflectDir = normalize(fReflectDir);
    float3 vEye = normalize(_in.vViewPos);
    
    float fReflectPow = saturate(dot(-vEye, fReflectDir));
    fReflectPow = pow(fReflectPow, 20.f);
    
    Color.xyz = Color.xyz * g_LightColor * fLightPow
                + Color.xyz * g_LightColor * g_LightAmbient
                + g_LightColor * g_SpecularRatio * fReflectPow;
    
    
    
    //Color = ST0.Sample(sampler1, _in.vUV);
    
    return Color;
}
#endif