#include "pch.h"
#include "CTimeMgr.h"

#include "CDevice.h"

TimeMgr::TimeMgr()
	:m_fDT(0.f)
	,m_iPrevCounter{}
	,m_iCurCounter{}
	,m_iFrequency{}
	,m_iCall(0)
	,m_fTime(0.f)
{
}

TimeMgr::~TimeMgr()
{

}

void TimeMgr::Awake()
{
	QueryPerformanceFrequency(&m_iFrequency);
	QueryPerformanceCounter(&m_iPrevCounter);
}

void TimeMgr::Update()
{
	QueryPerformanceCounter(&m_iCurCounter);

	m_fDT = static_cast<float>(m_iCurCounter.QuadPart - m_iPrevCounter.QuadPart) / static_cast<float>(m_iFrequency.QuadPart);

	m_iPrevCounter = m_iCurCounter;

	m_fTime += m_fDT;
	if (1.f <= m_fTime)
	{
		wchar_t szText[50] = {};
		swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_fDT, m_iCall);
		SetWindowText(Device::GetInst()->GetHwnd(), szText);

		m_iCall = 0;
		m_fTime = 0.f;
	}

	++m_iCall;
}
