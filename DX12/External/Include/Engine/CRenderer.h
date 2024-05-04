#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class Renderer :
    public Component
{
private:
	RENDERER_TYPE					m_Type;
	Ptr<Mesh>						m_pMesh;

	Ptr<Material>					m_pCurMtrl;
	Ptr<Material>					m_pDynamicMtrl;
	Ptr<Material>					m_pSharedMtrl;

	vector<Ptr<Material>>			m_vMtrls;

	bool							m_bIsRender;

public:
	RENDERER_TYPE			GetRenderType() { return m_Type; }
	Ptr<Mesh>				GetMesh() { return m_pMesh; }
	Ptr<Material>			GetMaterial() { return m_pCurMtrl; }
	Ptr<Material>			GetDynamicMaterial();
	Ptr<Material>			GetSharedMaterial() { return m_pSharedMtrl; }

	bool					GetIsRender() { return m_bIsRender; }
	void					SetIsRender(bool _bIsRender) { m_bIsRender = _bIsRender; }

	void				SetMesh(Ptr<Mesh> _mesh) { m_pMesh = _mesh; }
	void				SetMaterial(Ptr<Material> _material)
	{
		m_vMtrls.push_back(_material);
		m_pSharedMtrl	=	_material;
		m_pCurMtrl		=	m_pSharedMtrl;
		m_pDynamicMtrl	=	nullptr;
	}

	void				SetMaterial(int _idx) { m_pCurMtrl = m_vMtrls[_idx]; }

public:
	virtual void Render() = 0;

	virtual Renderer* Clone() = 0;

public:
	Renderer(RENDERER_TYPE _type);
	~Renderer();
};

