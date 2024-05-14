#ifndef _CARTOONPS3D
#define _CARTOONPS3D

#include "struct.hlsli"
#include "func.hlsli"

static float3 g_LightPos = float3(0.f, 0.f, 0.f);
static float3 g_LightDir = float3(0.f, 0.f, -1.f);
static float3 g_LightColor = float3(1.f, 1.f, 1.f);
static float3 g_LightAmbient = float3(0.1f, 0.1f, 0.1f);
static float3 g_SpecularRatio = float3(0.3f, 0.3f, 0.3f);

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

float4 PS_Cartoon3D(VS_OUT _in) : SV_Target
{
    float4 Color = float4(1.f, 1.f, 1.f, 1.f);
    float4 lightMap = (float4) 0.f;
    float4 ShadowColor = float4(0.9f, 0.9f, 0.9f, 1.f);
    float4 SpecularMap = (float4) 0.f;
    if (gMatrial.spriteCheck0)
    {
        Color = ST0.Sample(sampler1, _in.vUV);
    }
    
    if (gMatrial.spriteCheck1)
    {
        lightMap = ST1.Sample(sampler1, _in.vUV);
    }
    
    if (gMatrial.spriteCheck2)
    {
        SpecularMap = ST2.Sample(sampler1, _in.vUV);
    }
    
    float3 viewNormal = normalize(_in.vViewNormal);
    float3 ViewLightDir = mul(float4(g_LightDir, 0.f), View).xyz;
    
    ViewLightDir = normalize(ViewLightDir);
    
    float lambert = saturate(dot(viewNormal, -ViewLightDir));
    lambert = smoothstep(0.f, 0.01f, lambert);
    
    float halfLambert = lambert * 0.5 + 0.5f;
    
    if (halfLambert < lightMap.g)
    {
        Color.xyz *= ShadowColor.xyz;
    }
    
    float3 reflect = ViewLightDir + 2 * dot(-ViewLightDir, viewNormal) * viewNormal;
    float3 vEye = normalize(_in.vViewPos);
    reflect = normalize(reflect);
    float spec = saturate(dot(reflect, -vEye));
 
    spec = smoothstep(0.005f, 0.01f, spec);
    
    float4 SpecularColor = (float4) 0.f;
    if (SpecularMap.r >= 0.5f)
        SpecularColor += Color * float4(0.3f, 0.3f, 0.3f, 1.f) * spec;
   
    float rimAmount = 0.8f;
    float rimDot = 1 - saturate(dot(-vEye, viewNormal));
    rimDot = smoothstep(rimAmount - 0.01f, rimAmount + 0.01f, rimDot * lambert);
    float4 rimColor = rimDot * Color;
    
    Color.xyz += rimColor.xyz; //+ SpecularColor.xyz;
    
    return Color;
}
#endif