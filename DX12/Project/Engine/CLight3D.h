#pragma once
#include "CComponent.h"
class Light3D :
    public Component
{
private:
    LightInfo m_tLightInfo;

public:
    virtual void LateUpdate() override;

public:
    const LightInfo& GetLightInfo() { return m_tLightInfo; }

    void SetLightColor(Vec4 _vColor) { m_tLightInfo.Color = _vColor; }
    void SetSpecular(Vec4 _vSpec) { m_tLightInfo.Specular = _vSpec; }
    void SetAmbient(Vec4 _vAmb) { m_tLightInfo.Ambient = _vAmb; }

    Vec4 GetLightColor() { return m_tLightInfo.Color; }
    Vec4 GetSpecular() { return m_tLightInfo.Specular; }
    Vec4 GetAmbient() { return m_tLightInfo.Ambient; }

    void SetLightType(LIGHT_TYPE _type);
    void SetRadius(float _Radius);
    void SetAngle(float _Angle);

    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_tLightInfo.LightType; }
    float GetRadius() { return m_tLightInfo.Radius; }
    float GetAngle() { return m_tLightInfo.Angle; }

public:
    CLONE(Light3D)
    Light3D();
    virtual ~Light3D();
};

