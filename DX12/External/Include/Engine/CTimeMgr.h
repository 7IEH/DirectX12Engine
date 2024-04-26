#pragma once
class TimeMgr
	:public Singleton<TimeMgr>
{
	Single(TimeMgr)
private:
	float	m_fDT;

	LARGE_INTEGER		m_iPrevCounter;
	LARGE_INTEGER		m_iCurCounter;
	LARGE_INTEGER		m_iFrequency;

	UINT			m_iCall;
	float			m_fTime;

public:
	float GetDT() { return m_fDT; }

public:
	void Awake();
	void Update();
};

