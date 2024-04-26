#pragma once
#include "singleton.h"

class DestroyMgr
	:public Singleton<DestroyMgr>
{
	Single(DestroyMgr)
private:
	vector<Entity*> m_pDestroyObjs;

public:
	void PushForDestroy(Entity* m_pDead) { m_pDestroyObjs.push_back(m_pDead); }

public:
	void Update();
};