#ifndef _DEFAULTPS3D
#define _DEFAULTPS3D

#include "struct.hlsli"
#include "func.hlsli"

static float3 g_LightPos = float3(0.f, 0.f, 0.f);
static float3 g_LightDir = float3(1.f, 0.f, 0.f);
static float3 g_LightColor = float3(1.f, 1.f, 1.f);
static float3 g_LightAmbient = float3(0.1f, 0.1f, 0.1f);
static float3 g_SpecularRatio = float3(0.3f, 0.3f, 0.3f);

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

float4 PS_Default3D(VS_OUT _in) : SV_Target
{
    float4 Color = float4(1.f, 1.f, 1.f, 1.f);
    
    if (gMatrial.spriteCheck0)
    {
        Color = ST0.Sample(sampler1, _in.vUV);

    }
    
    float3 viewNormal = _in.vViewNormal;
   
    // Normal Mapping
    if (gMatrial.spriteCheck1)
    {
        float3 vNormal = ST1.Sample(sampler1, _in.vUV).xyz;
        vNormal = 2.f * vNormal - 1.f;
        
        float3x3 matRot =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal
        };
        
        viewNormal = normalize(mul(vNormal.xyz, matRot));
    }
    
    float3 vLightDir = normalize(mul(float4(g_LightDir, 0.f), View).xyz);
    float fLightPow = saturate(dot(viewNormal, -vLightDir));
    
    float3 vReflect = vLightDir + 2 * dot(-vLightDir, viewNormal) * viewNormal;
    vReflect = normalize(vReflect);
    
    float3 vEye = normalize(_in.vViewPos);
    
    float vReflectPow = saturate(dot(-vEye, vReflect));
    vReflectPow = pow(vReflectPow, 20.f);
    
    Color.rgb = Color.rgb * g_LightColor * fLightPow
                + Color.rgb * g_LightAmbient
                + g_LightColor * g_SpecularRatio * vReflectPow;

    
    return Color;
}
#endif