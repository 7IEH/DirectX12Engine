#pragma once
#include "CAsset.h"

class Mesh;
class Material;
class GameObject;

struct MeshRenderInfo
{
	Ptr<Mesh> mesh;
	vector<Ptr<Material>>	matrials;
};

class MeshData
	:public Asset
{
private:
	Ptr<Mesh>				m_pMesh;
	vector<Ptr<Material>>   m_pMaterials;

	vector<MeshRenderInfo>	m_vMeshRenders;

private:
	virtual HRESULT Load(const wstring& _strFilePath) override;

public:
	vector< GameObject*>	Instantiate();
	void SetMaterial(Ptr<Material> _mtrl, int _idx) { m_vMeshRenders[_idx].matrials.push_back(_mtrl); }


public:
	CLONE(MeshData)
	MeshData();
	virtual ~MeshData();

	friend class AssetMgr;
};

