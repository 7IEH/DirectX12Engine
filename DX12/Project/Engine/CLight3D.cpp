#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"

#include "CGameObject.h"

Light3D::Light3D()
	:Component(COMPONENT_TYPE::LIGHT3D)
	,m_tLightInfo{}
{
}

Light3D::~Light3D()
{
}

void Light3D::LateUpdate()
{
	Vec3 _vWorldPos = GetOwner()->GetComponent<Transform>()->GetMatWorld().Translation();
	m_tLightInfo.Position = _vWorldPos;

	RenderMgr::GetInst()->RegisterLight3D(GetOwner());
}

void Light3D::SetLightType(LIGHT_TYPE _type)
{
	m_tLightInfo.LightType = (int)_type;
}

void Light3D::SetRadius(float _Radius)
{
	m_tLightInfo.Radius = _Radius;
}

void Light3D::SetAngle(float _Angle)
{
	m_tLightInfo.Angle = _Angle;
}