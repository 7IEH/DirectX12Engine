#pragma once
#include "CEntity.h"

#include "CLayer.h"

class GameObject;
class Scene :
    public Entity
{
private:
    Layer*  m_pLayers[(UINT)LAYER_TYPE::END];

    UINT    m_iCollisionMatrix[(UINT)LAYER_TYPE::END];

public:
    void AddObject(GameObject* _obj, LAYER_TYPE _type, bool _bChildMove = TRUE)
    {
        m_pLayers[(UINT)_type]->AddObject(_obj);
    }

    GameObject* FindObjectByName(const wstring& _strName);
    Layer* GetLayer(LAYER_TYPE _type) { return m_pLayers[(UINT)_type]; }

    void SetCamera();

public:
    // Awake는 해당 scene에서 사용되는 설정을 해주는 단계
    virtual void Awake();
    // Start 부터 해당 scene에서 시작되는 로직을 설정해준다.
    // Unity 같은 경우 Start 단계에서 해당 일을 해준다면, Awake단계에서 설정되지 않은
    // 참조등으로인해 오류가 날 수 있음
    virtual void Start();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void LateUpdate();

public:
    CLONE(Scene)
    Scene();
    virtual ~Scene();
};

