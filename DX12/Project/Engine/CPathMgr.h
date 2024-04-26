#pragma once
class PathMgr
	:public Singleton<PathMgr>
{
	Single(PathMgr)
private:
	static wchar_t m_sPath[255];

public:
	wchar_t* GetPath() { return m_sPath; }

public:
	void Awake();
};

