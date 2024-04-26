#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CDestroyMgr.h"

Layer::Layer()
	: m_Type(LAYER_TYPE::PLAYER)
	, m_pObjs{}
{
}

Layer::~Layer()
{
	ReleaseVector<GameObject>(m_pObjs);
}

void Layer::Awake()
{
	for (size_t i = 0;i < m_pObjs.size();i++)
	{
		m_pObjs[i]->Awake();
	}
}

void Layer::Start()
{
	for (size_t i = 0;i < m_pObjs.size();i++)
	{
		m_pObjs[i]->Start();
	}
}

void Layer::Update()
{
	for (size_t i = 0;i < m_pObjs.size();i++)
	{
		m_pObjs[i]->Update();
	}
}

void Layer::FixedUpdate()
{
	for (size_t i = 0;i < m_pObjs.size();i++)
	{
		m_pObjs[i]->FixedUpdate();
	}
}

void Layer::LateUpdtae()
{
	for (size_t i = 0;i < m_pObjs.size();i++)
	{
		m_pObjs[i]->LateUpdate();
	}

	vector<GameObject*>::iterator iter = m_pObjs.begin();
	for (;iter != m_pObjs.end();)
	{
		if ((*iter)->GetDead())
		{
			DestroyMgr::GetInst()->PushForDestroy((*iter));
			iter = m_pObjs.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}