#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CSceneMgr.h"

#include "CDevice.h"
#include "CConstantBuffer.h"

#include "CGameObject.h"

Transform::Transform()
	:Component(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeTransform{}
	, m_WorldMatrix{}
	, m_LocalDir{}
	, m_WorldDir{}
	, m_bAbsolute(true)
{
	m_RelativeTransform.Scale = { 1.f,1.f,1.f };
}

Transform::~Transform()
{
}

void Transform::LateUpdate()
{
	// world(SRT)
	Vec3 _Scale = m_RelativeTransform.Scale;
	Vec3 _Rotation = m_RelativeTransform.Rotation;
	Vec3 _Position = m_RelativeTransform.Position;

	XMMATRIX _ScaleMatrix = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
	XMMATRIX _RotateMatrixX = XMMatrixRotationX(_Rotation.x * (XM_PI / 180.f));
	XMMATRIX _RotateMatrixY = XMMatrixRotationY(_Rotation.y * (XM_PI / 180.f));
	XMMATRIX _RotateMatrixZ = XMMatrixRotationZ(_Rotation.z * (XM_PI / 180.f));
	XMMATRIX _TranslationMatrix = XMMatrixTranslation(_Position.x, _Position.y, _Position.z);

	XMMATRIX _WorldMatrix = XMMatrixMultiply(_ScaleMatrix, _RotateMatrixX);
	_WorldMatrix = XMMatrixMultiply(_WorldMatrix, _RotateMatrixY);
	_WorldMatrix = XMMatrixMultiply(_WorldMatrix, _RotateMatrixZ);
	_WorldMatrix = XMMatrixMultiply(_WorldMatrix, _TranslationMatrix);
	m_WorldMatrix = _WorldMatrix;

	// 물체의 방향값을 다시 계산한다.
	m_WorldDir[(UINT)DIRECTION_TYPE::RIGHT] = m_LocalDir[(UINT)DIRECTION_TYPE::RIGHT] = { 1.f,0.f,0.f,0.f };
	m_WorldDir[(UINT)DIRECTION_TYPE::UP] = m_LocalDir[(UINT)DIRECTION_TYPE::UP] = { 0.f,1.f,0.f,0.f };
	m_WorldDir[(UINT)DIRECTION_TYPE::FRONT] = m_LocalDir[(UINT)DIRECTION_TYPE::FRONT] = { 0.f,0.f,1.f,0.f };

	// Vec3 를 vec4 타입으로 확장
	// XMVector3TransformCoord w를 1로 확장
	// XMVector3TransformNormal w를 0으로 확장
	// 기저 벡터는 말 그대로 방향만을 의미 하기 위해 존재함으로써
	// 위치 값이 존재하는 w는 0으로 확장한다.
	for (int i = 0;i < (UINT)DIRECTION_TYPE::END;i++)
	{
		m_LocalDir[i] = XMVector3TransformNormal(m_LocalDir[i], m_WorldMatrix);
		// Normalize;
		m_LocalDir[i].Normalize(m_WorldDir[i]);
	}

	/*GameObject* _parent = GetOwner()->GetParent();
	if (_parent != nullptr)
	{
		Transform* _parenttr = _parent->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
		XMMATRIX _parentMat = _parenttr->GetMatWorld();
		if (m_Absolute)
		{
			Vec4 vParentScale = _parenttr->GetRelativeScale();

			XMMATRIX matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_RelativeWorld = _temp * matParentScaleInv * _parentMat;
		}
		else
		{
			m_RelativeWorld = XMMatrixMultiply(_temp, _parentMat);
		}
	}*/
}

void Transform::UpdateData()
{
	// 전치 해주는 이유는 hlsl은 Colum-major이기 때문이다.
	e_MatrixData.WorldInv = XMMatrixTranspose(XMMatrixInverse(nullptr, m_WorldMatrix));

	e_MatrixData.ViewInv = XMMatrixTranspose(XMMatrixInverse(nullptr, e_MatrixData.View));

	e_MatrixData.ProjInv = XMMatrixTranspose(XMMatrixInverse(nullptr, e_MatrixData.Projection));

	// World
	e_MatrixData.World = XMMatrixTranspose(m_WorldMatrix);

	// WV 
	e_MatrixData.WV = XMMatrixTranspose(XMMatrixMultiply(m_WorldMatrix, e_MatrixData.View));

	// WVP
	e_MatrixData.WVP = XMMatrixTranspose(XMMatrixMultiply(XMMatrixMultiply(m_WorldMatrix, e_MatrixData.View), e_MatrixData.Projection));

	// V
	e_MatrixData.View = e_MatrixData.View.Transpose();

	// P
	e_MatrixData.Projection = e_MatrixData.Projection.Transpose();

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::TRANSFORM)->UpdateData(&e_MatrixData, sizeof(MatrixInfo), 1);

	e_MatrixData.View = e_MatrixData.View.Transpose();
	e_MatrixData.Projection = e_MatrixData.Projection.Transpose();
}

Vec3 Transform::GetWorldScale()
{
	//GameObject* pParent = GetOwner()->GetParent();
	Vec3 vWorldScale = Vec3(m_RelativeTransform.Scale);

	/*while (pParent)
	{
		vWorldScale *= Vec3(pParent->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativeScale());
		pParent = pParent->GetParent();
	}*/

	return vWorldScale;
}

void Transform::InitializeDir()
{
	m_RelativeTransform.Rotation = { 0.f,0.f,0.f };
}
