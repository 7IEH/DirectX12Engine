#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};

class StructuredBuffer :
    public Entity
{
private:
    

public:
    

public:
    CLONE(StructuredBuffer);
    StructuredBuffer();
    ~StructuredBuffer();
};

