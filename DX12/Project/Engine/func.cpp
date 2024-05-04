#include "pch.h"

string EH::ConvertString(const wstring& _wstr)
{
	int len;
	int slength = static_cast<int>(_wstr.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), slength, 0, 0, 0, 0);
	string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

wstring EH::ConvertWstring(const string& _str)
{
	int len;
	int slength = static_cast<int>(_str.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, _str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, _str.c_str(), slength, buf, len);
	wstring ret(buf);
	delete[] buf;
	return ret;
}
