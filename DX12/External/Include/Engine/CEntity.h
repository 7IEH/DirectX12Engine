#pragma once
class Entity
{
private:
	static UINT g_NextID;

private:
	const UINT	m_ID;
	wstring		m_strName;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() const { return m_strName; }
	UINT GetID() { return m_ID; }

	virtual Entity* Clone() = 0;

public:
	Entity();
	Entity(const Entity& _origin);
	virtual ~Entity();
};

