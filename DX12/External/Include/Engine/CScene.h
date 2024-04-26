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
    // Awake�� �ش� scene���� ���Ǵ� ������ ���ִ� �ܰ�
    virtual void Awake();
    // Start ���� �ش� scene���� ���۵Ǵ� ������ �������ش�.
    // Unity ���� ��� Start �ܰ迡�� �ش� ���� ���شٸ�, Awake�ܰ迡�� �������� ����
    // �������������� ������ �� �� ����
    virtual void Start();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void LateUpdate();

public:
    CLONE(Scene)
    Scene();
    virtual ~Scene();
};

