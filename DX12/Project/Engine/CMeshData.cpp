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
		Mesh* _mesh = new Mesh;
		vector<VertexInfo> _vertices = _loader.GetMesh(i).vertices;
		vector<UINT> _indices;
		_mesh->CreateBuffer(BUFFER_TYPE::VERTEX, (UINT)_vertices.size(), _vertices, _indices);
		for (const vector<UINT>& _buffer : _loader.GetMesh(i).indices)
		{
			if (_buffer.empty())
			{
				vector<UINT> _defaultBuffer{ 0 };
				_indices = _defaultBuffer;
			}
			else
			{
				_indices = _buffer;
			}

			_mesh->CreateBuffer(BUFFER_TYPE::INDEX, (UINT)_indices.size(), _vertices, _indices);
		}

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
		
		for (size_t i = 0;i < 5;i++)
		{
			if (i == 0)
			{
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"HarunaBodyMtrl"));
			}
			else if (i == 1)
			{
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"HarunaFaceMtrl"));
			}
			else if (i == 2)
			{
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"HarunaEyeMouthMtrl"));
			}
			else if (i == 3)
			{
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"HarunaHairMtrl"));
			}
			else
			{
				_render->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Default3DMat"));
			}
		}

		_temp.push_back(_pObj);
	}

	return _temp;
}
