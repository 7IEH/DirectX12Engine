#pragma once
#include "CRenderer.h"
#include "CStructuredBuffer.h"

class MeshRenderer :
    public Renderer
{
public:
	virtual void UpdateData() override;
	virtual void LateUpdate() override {};
	virtual void Render() override;

public:
	CLONE(MeshRenderer)
	MeshRenderer();
	virtual ~MeshRenderer();
};