#include "pch.h"
#include "CMeshRenderer.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CMaterial.h"
#include "CMesh.h"

#include "CDevice.h"

MeshRenderer::MeshRenderer()
	:Renderer(RENDERER_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render()
{
	if (!GetMaterial().Get() && !GetMesh().Get())
		return;
	UINT test = GetMesh()->GetSubSetCount();

	for (UINT i = 0;i < test;i++)
	{
		UpdateData();
		GetMesh()->Render(i);
	}
}

void MeshRenderer::UpdateData()
{
	Transform* _pTr = GetOwner()->GetComponent<Transform>();
	if (nullptr == _pTr)
		return;

	// Material Render
	// 1. Shader Update
	// 2. Texture Binding
	GetMaterial().Get()->Render();
	_pTr->UpdateData();
}