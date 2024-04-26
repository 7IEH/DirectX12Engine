#include "pch.h"
#include "CkeyMgr.h"
#include "CEngine.h"

int g_KeySync[KEY::KEY_END] =
{
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',

	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',

	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_TAB,
	VK_LSHIFT,
	VK_LMENU,
	VK_LCONTROL,
	VK_SPACE,

	VK_ESCAPE,
	VK_RETURN,
	VK_BACK,

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,
};


KeyMgr::KeyMgr()
{

}

KeyMgr::~KeyMgr()
{

}

void KeyMgr::Awake()
{
	// Capaticy �� �̸� Ȯ��
	m_vecKeyData.reserve(KEY::KEY_END);

	// ������ ������ �ø�
	//m_vecKeyData.resize(KEY::KEY_END);

	for (UINT i = 0; i < KEY_END; ++i)
	{
		m_vecKeyData.push_back(FKeyData{ (KEY)i, NONE, false });
	}
}

void KeyMgr::Update()
{
	if (nullptr == GetFocus())
	{
		for (size_t i = 0; i < m_vecKeyData.size(); ++i)
		{
			if (TAP == m_vecKeyData[i].eState)
			{
				m_vecKeyData[i].eState = PRESSED;
			}
			else if (PRESSED == m_vecKeyData[i].eState)
			{
				m_vecKeyData[i].eState = RELEASED;
			}
			else if (RELEASED == m_vecKeyData[i].eState)
			{
				m_vecKeyData[i].eState = NONE;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecKeyData.size(); ++i)
		{
			if (GetAsyncKeyState(g_KeySync[m_vecKeyData[i].eKey]) & 0x8001)
			{
				// �̹� �����ӿ� �ش� Ű�� �����ִ�.
				if (m_vecKeyData[i].bPressed)
				{
					// �������� �����־��� ==> ��� ���� ����
					m_vecKeyData[i].eState = PRESSED;
				}
				else
				{
					// �������� �������� �ʾҴ� ==> �� ���� ����
					m_vecKeyData[i].eState = TAP;
				}

				m_vecKeyData[i].bPressed = true;
			}
			else
			{
				// �̹� �����ӿ� �ش�Ű�� �� ���� �ִ�
				if (m_vecKeyData[i].bPressed)
				{
					// ���� �����ӿ��� ���� �־��� ==> �� ��
					m_vecKeyData[i].eState = RELEASED;
				}
				else
				{
					m_vecKeyData[i].eState = NONE;
				}

				m_vecKeyData[i].bPressed = false;
			}
		}

		// ���콺 ��ǥ ���
		m_vPrevMouse = m_vCurMouse;
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(Engine::GetInst()->GetHwnd(), &pt);
		m_vCurMouse = Vec2((float)pt.x, (float)pt.y);

		m_vMouseDir = m_vCurMouse - m_vPrevMouse;
		m_vMouseDir.Normalize();
	}
}