#pragma once

class KeyMgr
	:public Singleton<KeyMgr>
{
	Single(KeyMgr);
private:
	vector<FKeyData>	m_vecKeyData;

	Vec2				m_vPrevMouse;
	Vec2				m_vCurMouse;
	Vec2				m_vMouseDir;

public:
	KEY_STATE GetKeyState(KEY _Key) { return m_vecKeyData[_Key].eState; }
	Vec2 GetMousePos() { return m_vCurMouse; }
	Vec2 GetMouseDir() { return m_vMouseDir; }

public:
	void Awake();
	void Update();
};

