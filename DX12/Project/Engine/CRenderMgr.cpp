#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CGameObject.h"
#include "CCamera.h"

#include "CConstantBuffer.h"
#include "CStructuredBuffer.h"

RenderMgr::RenderMgr()
	: m_pCams{}
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
	e_Global.Light2DSize = m_vLights2D.size();
	e_Global.Light3DSize = m_vLights3D.size();

	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::GLOBAL)->UpdateData_Constant(&e_Global, sizeof(GlobalDataInfo), 1);

	/*******************
	| Light2DBuffer Set	
	********************/
	{
		/*static vector<LightInfo> _vLight2DInfo = {};

		for (size_t i = 0; i < m_vLights2D.size();i++)
		{
			const LightInfo& _lightInfo = m_vLights2D[i]->GetComponent<Light2D>()->GetLightInfo();
			_vLight2DInfo.push_back(_lightInfo);
		}

		if (!m_vLights2D.empty())
		{
			m_pLight2DBuffer->SetData(&_vLight2DInfo, (UINT)_vLight2DInfo.size());
		}*/
	}


	/*******************
	| Light3DBuffer Set	
	********************/
	{
		static vector<LightInfo> _vLight3DInfo = {};

		for (size_t i = 0; i < m_vLights3D.size();i++)
		{
			const LightInfo& _lightInfo = m_vLights3D[i]->GetComponent<Light3D>()->GetLightInfo();
			_vLight3DInfo.push_back(_lightInfo);
		}

		if (!m_vLights3D.empty())
		{
			m_pLight3DBuffer->SetData(_vLight3DInfo.data(), (UINT)_vLight3DInfo.size());
		}

		m_pLight3DBuffer->UpdateData(12);
		_vLight3DInfo.clear();
	}
}

void RenderMgr::Render()
{
	for (UINT i = 0;i < (UINT)CAMERA_TYPE::END;i++)
	{
		if (nullptr != m_pCams[i])
		{
			Camera* _pCam = m_pCams[i]->GetComponent<Camera>();
			if (nullptr != _pCam)
			{
				_pCam->Render();
			}
		}
	}
}

void RenderMgr::Clear()
{
	m_vLights2D.clear();
	m_vLights3D.clear();
}