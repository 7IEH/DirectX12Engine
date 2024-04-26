#include "pch.h"
#include "CScene.h"

#include "CRenderMgr.h"

#include "CCamera.h"

Scene::Scene()
	: m_pLayers{}
	, m_iCollisionMatrix{}
{
	for (UINT _type = 0;_type < (UINT)LAYER_TYPE::END;_type++)
	{
		m_pLayers[_type] = new Layer;
		m_pLayers[_type]->SetType(LAYER_TYPE(_type));
	}
}

Scene::~Scene()
{
	ReleaseArray<Layer, (UINT)LAYER_TYPE::END>(m_pLayers);
}

GameObject* Scene::FindObjectByName(const wstring& _strName)
{
	GameObject* _pObj = nullptr;
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		_pObj = m_pLayers[i]->FindObject(_strName);
	}

	return _pObj;
}

void Scene::SetCamera()
{
	Layer* _pCamLayer = m_pLayers[(UINT)LAYER_TYPE::CAMERA];
	const vector<GameObject*> _pCamObjs = _pCamLayer->GetLayerObjs();

	for (size_t i = 0;i < _pCamObjs.size();i++)
	{
		Camera* _pCam = _pCamObjs[i]->GetComponent<Camera>();
		if (nullptr != _pCam)
		{
			_pCam->SetCameraType(_pCam->GetCameraType());
		}
	}
}

void Scene::Awake()
{
	SetCamera();
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_pLayers[i]->Awake();
	}
	Start();
}

void Scene::Start()
{
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_pLayers[i]->Start();
	}
}

void Scene::Update()
{
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_pLayers[i]->Update();
	}
}

void Scene::FixedUpdate()
{
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_pLayers[i]->FixedUpdate();
	}
}

void Scene::LateUpdate()
{
	for (UINT i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		m_pLayers[i]->LateUpdtae();
	}
}