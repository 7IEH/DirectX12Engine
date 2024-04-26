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
	// ���ο� ���� ����� ����
	Scene* _pScene = AddScene<TempScene>(L"TestLevel");
	SelectScene(L"TestLevel");
}

void SceneMgr::Update()
{
	if (nullptr == m_pCurScene)
		return;

	m_pCurScene->Update();
	
	// Rigidbody�� ���� ���꿡 �����Ǵ� Update  
	m_pCurScene->FixedUpdate();

	// Camera Targeting �Լ� ���� Update ���Ŀ� �����ؾ��ϴ� Update
	m_pCurScene->LateUpdate();
}