#pragma once
#include "CEntity.h"

#include "CComponent.h"
#include "Comp.h"

class Script;
class GameObject :
	public Entity
{
private:
	bool            m_bActive;
	bool            m_bDead;
	LAYER_TYPE      m_eLayerType;

	Component* m_pComps[(UINT)COMPONENT_TYPE::END];
	Renderer* m_pRenderer;
	vector<Script*> m_vScripts;

public:
	template<typename T>
	T* AddComponent()
	{
		T* _pComp = new T;
		COMPONENT_TYPE _eType = _pComp->GetType();
		UINT _iType = (UINT)_eType;
		if (COMPONENT_TYPE::SCRIPT == _eType)
		{
			m_vScripts.push_back((Script*)_pComp);
		}
		else
		{
			assert(!m_pComps[_iType]);
			m_pComps[_iType] = _pComp;
			if (COMPONENT_TYPE::RENDERER == _pComp->GetType())
			{
				m_pRenderer = dynamic_cast<Renderer*>(_pComp);
			}
		}
		_pComp->SetOwner(this);
		return _pComp;
	}


	template<typename T>
	T* GetComponent()
	{
		T* _pComp = nullptr;
		if constexpr (std::is_same_v<T, MeshRenderer>)
		{
			return dynamic_cast<T*>(m_pRenderer);
		}

		if constexpr (std::is_same_v<T, SkyBox>)
		{
			return dynamic_cast<T*>(m_pRenderer);
		}

		if constexpr (std::is_same_v<T, Renderer>)
		{
			return m_pRenderer;
		}

		if constexpr (std::is_same_v<T, Camera>)
		{
			_pComp = dynamic_cast<T*>(m_pComps[(UINT)COMPONENT_TYPE::CAMERA]);
		}

		if constexpr (std::is_same_v<T, Light2D>)
		{
			_pComp = dynamic_cast<T*>(m_pComps[(UINT)COMPONENT_TYPE::LIGHT2D]);
		}

		if constexpr (std::is_same_v<T, Light3D>)
		{
			_pComp = dynamic_cast<T*>(m_pComps[(UINT)COMPONENT_TYPE::LIGHT3D]);
		}

		if constexpr (std::is_same_v<T, Transform>)
		{
			_pComp = dynamic_cast<T*>(m_pComps[(UINT)COMPONENT_TYPE::TRANSFORM]);
		}

		if constexpr (std::is_same_v<T, Animator3D>)
		{
			_pComp = dynamic_cast<T*>(m_pComps[(UINT)COMPONENT_TYPE::ANIMATOR3D]);
		}

		return _pComp;
	}

public:
	void						SetDead(bool _dead) { m_bDead = _dead; }
	bool						GetDead() { return m_bDead; }

	void						Enabled(bool _flag) { m_bActive = _flag; }

public:
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();

	void Render();

public:
	CLONE(GameObject)
		GameObject();
	~GameObject();
};

