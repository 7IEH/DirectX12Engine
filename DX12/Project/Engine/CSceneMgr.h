#pragma once
#include "singleton.h"
#include "CScene.h"

class SceneMgr
	:public Singleton<SceneMgr>
{
	Single(SceneMgr)
private:
	map<wstring, Scene*> m_mScenes;
	Scene* m_pCurScene;

public:
	template<typename T>
	T* AddScene(const wstring& _sceneName)
	{
		T* _pScene = new T;
		map<wstring, Scene*>::iterator iter = m_mScenes.find(_sceneName);
		if (m_mScenes.end() != iter)
		{
			delete _pScene;
			_pScene = nullptr;
			HandleError(L"Scene Duplicated", 0);
		}
		_pScene->SetName(_sceneName);
		m_mScenes.insert(std::make_pair(_sceneName, _pScene));

		return _pScene;
	}

	void SelectScene(const wstring& _levelName)
	{
		Scene* _pScene = FindScene(_levelName);
		if (nullptr == _pScene)
		{
			HandleError(L"Not Exist Finding Scene", 0);
		}
		m_pCurScene = _pScene;
		m_pCurScene->Awake();
	}


	Scene* FindScene(const wstring& _levelName)
	{
		map<wstring, Scene*>::iterator iter = m_mScenes.find(_levelName);

		if (m_mScenes.end() == iter)
		{
			return nullptr;
		}

		return iter->second;
	}

public:
	Scene* GetCurScene()const { return m_pCurScene; }

public:
	void Awake();
	void Update();
};