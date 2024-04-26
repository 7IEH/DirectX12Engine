#include "pch.h"
#include "CGameObject.h"

#include "CSceneMgr.h"

#include "CComponent.h"
#include "CScript.h"
#include "CLayer.h"

GameObject::GameObject()
	: m_bActive(TRUE)
	, m_bDead(FALSE)
	, m_eLayerType(LAYER_TYPE::DEFAULT)
	, m_pComps{}
	, m_pRenderer(nullptr)
	, m_vScripts{}
{
}

GameObject::~GameObject()
{
	ReleaseArray<Component, int(COMPONENT_TYPE::END)>(m_pComps);
	ReleaseVector<Script>(m_vScripts);
}

void GameObject::Awake()
{
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
	{
		if (nullptr != m_pComps[i])
		{
			m_pComps[i]->Awake();
		}
	}

	for (size_t i = 0;i < m_vScripts.size();i++)
	{
		if (nullptr != m_vScripts[i])
		{
			m_vScripts[i]->Awake();
		}
	}
}

void GameObject::Start()
{
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
	{
		if (nullptr != m_pComps[i])
		{
			m_pComps[i]->Start();
		}
	}

	for (size_t i = 0;i < m_vScripts.size();i++)
	{
		if (nullptr != m_vScripts[i])
		{
			m_vScripts[i]->Start();
		}
	}
}

void GameObject::Update()
{
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
	{
		if (nullptr != m_pComps[i])
		{
			m_pComps[i]->Update();
		}
	}

	for (size_t i = 0;i < m_vScripts.size();i++)
	{
		if (nullptr != m_vScripts[i])
		{
			m_vScripts[i]->Update();
		}
	}
}

void GameObject::FixedUpdate()
{
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
	{
		if (nullptr != m_pComps[i])
		{
			m_pComps[i]->FixedUpdate();
		}
	}

	for (size_t i = 0;i < m_vScripts.size();i++)
	{
		if (nullptr != m_vScripts[i])
		{
			m_vScripts[i]->FixedUpdate();
		}
	}
}

void GameObject::LateUpdate()
{
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;i++)
	{
		if (nullptr != m_pComps[i])
		{
			m_pComps[i]->LateUpdate();
		}
	}

	for (size_t i = 0;i < m_vScripts.size();i++)
	{
		if (nullptr != m_vScripts[i])
		{
			m_vScripts[i]->LateUpdate();
		}
	}
}

void GameObject::Render()
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Render();
	}
}