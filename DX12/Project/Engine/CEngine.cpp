#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CRenderMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CDestroyMgr.h"

Engine::Engine()
	:m_WinInfo{}
{
}

Engine::~Engine()
{
}

int Engine::Awake(const WindowInfo& _winInfo)
{
	m_WinInfo = _winInfo;

	ResizeWindow(_winInfo.Res.x, _winInfo.Res.y);

	//Device
	if (FAILED(Device::GetInst()->Awake(m_WinInfo)))
	{
		return E_FAIL;
	}

	AwakeManager();

	Device::GetInst()->WaitSync();

	return S_OK;
}

void Engine::ResizeWindow(float _width, float _height)
{
	m_WinInfo.Res = Vec2(_width, _height);

	RECT _rt = { 0,0,(int)_width ,(int)_height };

	AdjustWindowRect(&_rt, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_WinInfo.Hwnd, 0, 10, 10, (int)_width, (int)_height, 0);
}

void Engine::AwakeManager()
{
	TimeMgr::GetInst()->Awake();
	PathMgr::GetInst()->Awake();
	KeyMgr::GetInst()->Awake();
	AssetMgr::GetInst()->Awake();
	SceneMgr::GetInst()->Awake();
	RenderMgr::GetInst()->Awake();
}

void Engine::Update()
{
	TimeMgr::GetInst()->Update();
	KeyMgr::GetInst()->Update();
	SceneMgr::GetInst()->Update();
	RenderMgr::GetInst()->Update();
	DestroyMgr::GetInst()->Update();
}