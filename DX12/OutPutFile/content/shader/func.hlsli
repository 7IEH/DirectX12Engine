#ifndef _FUNC
#define _FUNC

#include "struct.hlsli"

struct SkinningInfo
{
    float3 pos;
    float3 normal;
    float3 tangent;
};

void Skinning(inout float3 pos, inout float3 normal, inout float3 tangent,
    inout float4 weight, inout float4 indices)
{
    SkinningInfo _info = (SkinningInfo) 0.f;
    
    for (int i = 0; i < 4; i++)
    {
        if (weight[i] == 0.f)
            continue;
        
        int _boneIdx = indices[i];
        matrix matBone = g_mat_bone[_boneIdx];
        
        _info.pos += (mul(float4(pos, 1.f), matBone) * weight[i]).xyz;
        _info.normal += (mul(float4(normal, 0.f), matBone) * weight[i]).xyz;
        _info.tangent += (mul(float4(tangent, 0.f), matBone) * weight[i]).xyz;
    }

    pos = _info.pos;
    normal = normalize(_info.normal);
    tangent = normalize(_info.tangent);
}

float3 CaculateLight(int index, float3 viewNormal, float3 viewPos,float4 Color)
{
    float3 lightColor = (float3) 0.f;
    // Directional Light
    float3 ambient = float3(0.1f, 0.1f, 0.1f);
    float3 specularRatio = float3(0.3f, 0.3f, 0.3f);
    // 1. Lambert
    float3 viewLigthDir = normalize(mul(float4(gLightInfo3D[index].LightDir, 0.f), View).xyz);
    viewNormal = normalize(viewNormal);
   
    float Lambert = saturate(dot(viewNormal, -viewLigthDir));
    
    // 2. Specular
    float3 Eye = normalize(viewPos);
    
    float3 reflectDir = normalize(viewLigthDir + 2.f * viewNormal * dot(-viewLigthDir, viewNormal));
    
    float reflectPow = saturate(dot(-Eye, reflectDir));
    
    lightColor = Color.rgb * gLightInfo3D[index].Color.rgb * Lambert
                + Color.rgb * ambient
                + gLightInfo3D[index].Color.rgb * specularRatio * reflectPow;
    
    // Point Light
    
    return lightColor;
}


#endif