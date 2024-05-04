#include "pch.h"
#include "CSkyBox.h"

#include "CAssetMgr.h"

#include "CGameObject.h"

#include "CTransform.h"

SkyBox::SkyBox()
	:Renderer(RENDERER_TYPE::SKYBOX)
	, m_eType(SkyBoxType::END)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::UpdateData()
{
	Transform* _pTr = GetOwner()->GetComponent<Transform>();
	if (nullptr == _pTr)
		return;
	

	if (SkyBoxType::Sphere == m_eType)
	{
		SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"SphereMesh"));
	}
	else if (SkyBoxType::Box == m_eType)
	{
		SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"CubeMesh"));
	}

	GetMaterial()->UpdateData();
	_pTr->UpdateData();
}

void SkyBox::Render()
{
	UpdateData();
	GetMesh()->Render(0);
}