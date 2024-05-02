#pragma once
#include "CRenderer.h"

enum class SkyBoxType
{
	Sphere,
	Box,
	END,
};

class SkyBox :
	public Renderer
{
private:
	SkyBoxType m_eType;

public:
	void SetType(SkyBoxType _eType) { m_eType = _eType; }

	void SetSphereTex(Ptr<Texture> _texture) { GetMaterial()->SetTexParam(TEX_0, _texture); }
	void SetCubeTex(Ptr<Texture> _texture) { GetMaterial()->SetTexParam(TEXCUBE_0, _texture); }

public:
	virtual void UpdateData() override;
	virtual void LateUpdate() override {};
	virtual void Render() override;

public:
	CLONE(SkyBox)
	SkyBox();
	virtual ~SkyBox();
};

