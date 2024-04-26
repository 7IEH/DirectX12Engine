#include "pch.h"
#include "global.h"

#include "CDevice.h"

// HandleError Initailze Failed 1, Error Failed 2
void HandleError(wstring _content, int flag)
{
	if (flag == 1)
	{
		MessageBoxW(Device::GetInst()->GetHwnd(), _content.c_str(), L"INITIALIZE FAILED!", MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBoxW(Device::GetInst()->GetHwnd(), _content.c_str(), L"ERROR!", MB_OK | MB_ICONEXCLAMATION);
	}
	DestroyWindow(Device::GetInst()->GetHwnd());
}