#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CGameObject.h"
#include "CCamera.h"

#include "CConstantBuffer.h"
#include "CStructuredBuffer.h"

RenderMgr::RenderMgr()
	:m_pCams{}
	, m_vLights2D{}
	, m_vLights3D{}
	, m_pLight2DBuffer(nullptr)
	, m_pLight3DBuffer(nullptr)
{

}

RenderMgr::~RenderMgr()
{
	if (nullptr != m_pLight2DBuffer)
		delete m_pLight2DBuffer;

	if (nullptr != m_pLight2DBuffer)
		delete m_pLight3DBuffer;
}

void RenderMgr::Update()
{
	Device::GetInst()->RenderBegin();

	UpdateData();
	Render();
	Clear();

	Device::GetInst()->RenderEnd();
}

void RenderMgr::UpdateData()
{
}

void RenderMgr::Render()
{

}

void RenderMgr::Clear()
{
	m_vLights2D.clear();
	m_vLights3D.clear();
}