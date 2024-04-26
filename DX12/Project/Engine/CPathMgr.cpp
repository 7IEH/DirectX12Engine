#include "pch.h"
#include "CPathMgr.h"

wchar_t PathMgr::m_sPath[255];

PathMgr::PathMgr()
{

}

PathMgr::~PathMgr()
{

}

void PathMgr::Awake()
{
	GetCurrentDirectory(255, m_sPath);

	size_t _iLength = wcslen(m_sPath);

	for (size_t i = _iLength - 1;i >= 0;--i)
	{
		if (m_sPath[i] == '\\')
		{
			m_sPath[i + 1] = '\0';
			break;
		}
	}

	_iLength = wcslen(m_sPath);

	for (size_t i = _iLength - 2;i >= 0;--i)
	{
		if (m_sPath[i] == '\\')
		{
			m_sPath[i + 1] = '\0';
			break;
		}
	}

	wcscat_s(m_sPath, L"content");
}