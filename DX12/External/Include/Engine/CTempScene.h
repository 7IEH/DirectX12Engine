#pragma once
#include "CScene.h"
class TempScene :
    public Scene
{
private:
    GameObject* m_pTestObject;
    GameObject* m_pTestObject2;

    int         m_iNum;

public:
    virtual void Awake()override;    

    virtual void Update()override;
public:
    TempScene();
    virtual ~TempScene();
};

