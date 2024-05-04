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
	vector<MeshRenderInfo>	m_vMeshRenders;

private:
	virtual HRESULT Load(const wstring& _strFilePath) override;

public:
	vector< GameObject*>	Instantiate();

public:
	CLONE(MeshData)
	MeshData();
	virtual ~MeshData();

	friend class AssetMgr;
};

