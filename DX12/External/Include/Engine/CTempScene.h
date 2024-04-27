#pragma once
#include "CScene.h"
class TempScene :
    public Scene
{
public:
    virtual void Awake()override;    

    virtual void Update()override;
public:
    TempScene();
    virtual ~TempScene();
};

