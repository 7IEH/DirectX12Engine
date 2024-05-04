#include "pch.h"
#include "CRenderer.h"

Renderer::Renderer(RENDERER_TYPE _type)
	:Component(COMPONENT_TYPE::RENDERER)
	, m_Type(_type)
	, m_pMesh(nullptr)
	, m_pCurMtrl(nullptr)
	, m_pDynamicMtrl(nullptr)
	, m_pSharedMtrl(nullptr)
	, m_bIsRender(FALSE)
	, m_vMtrls{}
{
}

Renderer::~Renderer()
{
}

Ptr<Material> Renderer::GetDynamicMaterial()
{
	// 이미 동적재질을 보유하고 있으면 그걸 준다.
	if (nullptr != m_pDynamicMtrl)
		return m_pDynamicMtrl;

	// 공유재질이 있다면
	if (nullptr != m_pDynamicMtrl)
	{
		// 공유재질을 복사해서 동적재질을 만들고 그걸 현재 사용재질로 설정한다.
		m_pDynamicMtrl = m_pSharedMtrl->Clone();
		m_pCurMtrl = m_pDynamicMtrl;
		return m_pDynamicMtrl;
	}

	return nullptr;
}