#include "pch.h"
#include "CRenderMgr.h"

#include "CStructuredBuffer.h"

void RenderMgr::Awake()
{
	m_pLight2DBuffer = new StructuredBuffer;
	m_pLight2DBuffer->Create(sizeof(LightInfo), 10, SB_TYPE::READ_ONLY, TRUE);

	m_pLight3DBuffer = new StructuredBuffer;
	m_pLight3DBuffer->Create(sizeof(LightInfo), 10, SB_TYPE::READ_ONLY, TRUE);
}