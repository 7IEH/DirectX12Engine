#include "pch.h"
#include "CSkyBox.h"

#include "CAssetMgr.h"

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
	if (SkyBoxType::Sphere == m_eType)
	{
		SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"SphereMesh"));
	}
	else if (SkyBoxType::Box == m_eType)
	{
		SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"CubeMesh"));
	}

	GetMaterial()->UpdateData();
}

void SkyBox::Render()
{
	UpdateData();
	GetMesh()->Render();
}