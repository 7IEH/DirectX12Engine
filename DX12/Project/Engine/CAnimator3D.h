#pragma once
#include "CComponent.h"
#include "CMesh.h"

class Material;
class StructredBuffer;
class Mesh;

class Animator3D :
    public Component
{
private:

public:
    virtual void LateUpdate()override;

public:
    virtual void Awake() override;
    virtual void Start() override;
    virtual void Update() override;
public:
    CLONE(Animator3D)
    Animator3D();
    virtual ~Animator3D();
};