#include "pch.h"
#include "CDestroyMgr.h"

DestroyMgr::DestroyMgr()
{

}

DestroyMgr::~DestroyMgr()
{
	ReleaseVector(m_pDestroyObjs);
}

void DestroyMgr::Update()
{
	if (m_pDestroyObjs.size() >= (size_t)100)
	{
		ReleaseVector(m_pDestroyObjs);
	}
}