#include "pch.h"
#include "CSceneMgr.h"

#include "CTempScene.h"

SceneMgr::SceneMgr()
	:m_mScenes{}
	,m_pCurScene(nullptr)
{
}

SceneMgr::~SceneMgr()
{
	ReleaseMap<wstring, Scene*>(m_mScenes);
}

void SceneMgr::Awake()
{
	// 새로운 레벨 만들고 선택
	Scene* _pScene = AddScene<TempScene>(L"TestLevel");
	SelectScene(L"TestLevel");
}

void SceneMgr::Update()
{
	if (nullptr == m_pCurScene)
		return;

	m_pCurScene->Update();
	
	// Rigidbody등 물리 연산에 연관되는 Update  
	m_pCurScene->FixedUpdate();

	// Camera Targeting 함수 같이 Update 이후에 관여해야하는 Update
	m_pCurScene->LateUpdate();
}