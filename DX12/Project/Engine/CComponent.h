#pragma once

#include "CEntity.h"

class GameObject;
class Component :
    public Entity
{
private:
    COMPONENT_TYPE  m_Type;
    GameObject* m_pOwner;

public:
    COMPONENT_TYPE  GetType() { return m_Type; }
    GameObject*     GetOwner() { return m_pOwner; }

    void            SetOwner(GameObject* _owner) { m_pOwner = _owner; }

public:
    virtual void Awake() {};
    virtual void Start() {};
    virtual void Update() {};
    virtual void FixedUpdate() {};
    virtual void LateUpdate() = 0;
    virtual void UpdateData() {};

    virtual Component* Clone() = 0;
public:
    Component(COMPONENT_TYPE _type);
    virtual ~Component();
};

