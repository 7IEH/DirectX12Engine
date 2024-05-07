#include "pch.h"
#include "CMeshData.h"

#include "CFBXLoader.h"
#include "CAssetMgr.h"	

#include "CGameObject.h"

#include "CMesh.h"
MeshData::MeshData()
	:Asset(ASSET_TYPE::MESHDATA)
{
}

MeshData::~MeshData()
{
}

HRESULT MeshData::Load(const wstring& _strFilePath)
{
	FBXLoader _loader;
	_loader.LoadFBX(_strFilePath);

	for (int i = 0;i < _loader.GetMeshCount();i++)
	{
		Ptr<Mesh> _mesh = Mesh::CreateFromFBX(&_loader.GetMesh(i), _loader);

		MeshRenderInfo _info = {};
		_info.mesh = _mesh;
		m_vMeshRenders.push_back(_info);
	}

	return S_OK;
}

vector<GameObject*> MeshData::Instantiate()
{
	vector<GameObject*> _temp = {};

	for (MeshRenderInfo& _info : m_vMeshRenders)
	{
		GameObject* _pObj = new GameObject;
		_pObj->AddComponent<Transform>();
		MeshRenderer* _render = _pObj->AddComponent<MeshRenderer>();
		_render->SetMesh(_info.mesh);

		for (size_t i = 0;i < _info.matrials.size();i++)
		{
			_render->SetMaterial(_info.matrials[i]);
		}

		if (_info.mesh->IsAnimMesh())
		{
			Animator3D* _anim3D = _pObj->AddComponent<Animator3D>();
			_anim3D->SetBones(_info.mesh->GetBones());
			_anim3D->SetAnimClip(_info.mesh->GetAnimClip());
		}

		_temp.push_back(_pObj);
	}

	return _temp;
}
