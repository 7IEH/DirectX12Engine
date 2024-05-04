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
	// �̹� ���������� �����ϰ� ������ �װ� �ش�.
	if (nullptr != m_pDynamicMtrl)
		return m_pDynamicMtrl;

	// ���������� �ִٸ�
	if (nullptr != m_pDynamicMtrl)
	{
		// ���������� �����ؼ� ���������� ����� �װ� ���� ��������� �����Ѵ�.
		m_pDynamicMtrl = m_pSharedMtrl->Clone();
		m_pCurMtrl = m_pDynamicMtrl;
		return m_pDynamicMtrl;
	}

	return nullptr;
}