#ifndef _FUNC
#define _FUNC

#include "struct.hlsli"

// Luna LightExample
void ComputeDirectionalLight(LightMaterial mat, DirectionalLight L,
                             float3 normal, float3 toEye,
                             out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.f);
    
    float3 lightVec = -L.Direction;
    
    ambient = mat.Ambient * L.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
}

void ComputePointLight(LightMaterial mat, PointLight L, float3 pos, float3 normal, float3 toEye,
out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    pos.z = 0.f;
    float3 lightVec = L.Position - pos;
    
    float d = length(lightVec);
    
    if (d > L.Range)
    {
        ambient = float4(1.f, 0.f, 0.f, 1.f);
        diffuse = float4(1.f, 0.f, 0.f, 1.f);
        spec = float4(1.f, 0.f, 0.f, 1.f);
        return;
    }
    
    lightVec /= d;
    
    ambient = mat.Ambient * L.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
    
    float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
    
    diffuse *= att;
    spec *= att;
}

void ComputeSpotLight(LightMaterial mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = L.Position - pos;
    
    float d = length(lightVec);
    
    if (d > L.Range)
        return;
    
    lightVec /= d;
    
    ambient = mat.Ambient * L.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
    
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
    
    float att = spot / dot(L.Att, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att;
    spec *= att;
}

void ComputeDirectionalLight2D(in LightInfo _lightInfo, inout LightInfo lightColor)
{
    lightColor.Ambient.rgb += _lightInfo.Ambient.rgb;
}

void ComputePointLight2D(float3 _surfacePos, in LightInfo _lightInfo, inout LightInfo lightColor)
{
    float fAttenu = 1.f;
    
    float2 _lightPos = _lightInfo.Position.xy;
    
    float2 _btwDistance = _lightPos - _surfacePos.xy;
    
    if (length(_btwDistance) <= _lightInfo.Radius)
    {
        fAttenu = 1.f - (length(_btwDistance) / _lightInfo.Radius);
        lightColor.Color.rgb += _lightInfo.Color.rgb * fAttenu;
    }
}


void ComputeSpotLight2D(float3 _surfacePos, in LightInfo _lightInfo, inout LightInfo lightColor)
{
    float fAttenu = 1.f;
    
    float3 _lightPos = _lightInfo.Position;
    
    float3 _btwDistance = _lightPos - _surfacePos;
    
    if (length(_btwDistance) <= _lightInfo.Radius)
    {
        float _Cos = dot(normalize(_btwDistance), normalize(_lightInfo.LightDir));
            
        if (_lightInfo.Angle / 2.f >= degrees(acos(_Cos)))
        {
            fAttenu = 1.f - (length(_btwDistance) / _lightInfo.Radius);
        
            lightColor.Color.rgb += _lightInfo.Color.rgb * fAttenu;
        }
    }
}

void ComputeLight2D(float3 _surfacePos, int _infoIdx, inout LightInfo lightColor)
{
    // Type
    // 0 : Directional
    // 1 : Point
    // 2 : Spot
    LightInfo _info = g_Light[_infoIdx];
    
    if (_info.LightType == 0)
    {
        ComputeDirectionalLight2D(_info, lightColor);
    }
    else if (_info.LightType == 1)
    {
        ComputePointLight2D(_surfacePos, _info, lightColor);

    }
    else
    {
        ComputeSpotLight2D(_surfacePos, _info, lightColor);
    }
}
#endif