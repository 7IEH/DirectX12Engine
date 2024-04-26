#pragma once

class Engine
	:public Singleton<Engine>
{
	Single(Engine)
private:
	WindowInfo	m_WinInfo;

public:
	HWND	GetHwnd() { return m_WinInfo.Hwnd; }
	Vec2	GetRes() { return m_WinInfo.Res; }

public:
	void	ResizeWindow(float _width, float _height);

private:
	void AwakeManager();

public:
	int		Awake(const WindowInfo& _winInfo);
	void    Update();
};

