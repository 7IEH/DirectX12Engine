#pragma once
#include "CEntity.h"

#include "CGameObject.h"

class Layer :
	public Entity
{
private:
	LAYER_TYPE m_Type;
	vector<GameObject*> m_pObjs;

public:
	void AddObject(GameObject* _obj)
	{
		m_pObjs.push_back(_obj);
	}

	GameObject* FindObject(const wstring& _name)
	{
		for (size_t i = 0; i < m_pObjs.size();i++)
		{
			if (_name == m_pObjs[i]->GetName())
			{
				return m_pObjs[i];
			}
		}

		return nullptr;
	}

	void SetType(LAYER_TYPE _type) { m_Type = _type; }

	const vector<GameObject*>& GetLayerObjs() { return m_pObjs; }

public:
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdtae();

public:
	CLONE(Layer)
	Layer();
	virtual ~Layer();
};

