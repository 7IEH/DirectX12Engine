#include "pch.h"
#include "CFBXLoader.h"

#include "CPathMgr.h"
#include "CMaterial.h"
#include "CAssetMgr.h"

FBXLoader::FBXLoader()
	:m_pManager(nullptr)
	, m_pScene(nullptr)
	, m_pImporter(nullptr)
	, m_pResDirectory{}
	, m_vMeshes{}
	, m_vBones{}
	, m_vAnimClips{}
	, m_AnimNames{}
{
}

FBXLoader::~FBXLoader()
{
	if (m_pScene)
		m_pScene->Destroy();

	if (m_pManager)
		m_pManager->Destroy();
}

void FBXLoader::LoadFBX(const wstring& _path)
{
	Import(_path);

	//LoadBones(m_pScene->GetRootNode());
	//LoadAnimationInfo();

	ParseNode(m_pScene->GetRootNode());

	CreateTextures();
	CreateMaterials();
}

void FBXLoader::Import(const wstring& _path)
{
	m_pManager = FbxManager::Create();

	FbxIOSettings* _settings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(_settings);

	m_pScene = FbxScene::Create(m_pManager, "");

	//m_pResDirectory = 

	m_pImporter = FbxImporter::Create(m_pManager, "");

	string _strPath = EH::ConvertString(_path);
	m_pImporter->Initialize(_strPath.c_str(), -1, m_pManager->GetIOSettings());

	m_pImporter->Import(m_pScene);


	m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	FbxGeometryConverter _geometryConverter(m_pManager);
	_geometryConverter.Triangulate(m_pScene, TRUE);

	m_pImporter->Destroy();
}

void FBXLoader::ParseNode(FbxNode* _node)
{
	FbxNodeAttribute* _attribute = _node->GetNodeAttribute();

	if (_attribute)
	{
		switch (_attribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			LoadMesh(_node->GetMesh());
			break;
		}
	}

	const UINT _materaiCount = _node->GetMaterialCount();

	for (UINT i = 0;i < _materaiCount;i++)
	{
		FbxSurfaceMaterial* _surfaceMtrl = _node->GetMaterial(i);
		LoadMaterial(_surfaceMtrl);
	}

	const UINT _childCount = _node->GetChildCount();
	for (UINT i = 0;i < _childCount;i++)
	{
		ParseNode(_node->GetChild(i));
	}
}

void FBXLoader::LoadMesh(FbxMesh* _mesh)
{
	m_vMeshes.push_back(FBXMeshInfo());
	FBXMeshInfo& _meshInfo = m_vMeshes.back();

	_meshInfo.name = EH::ConvertWstring(_mesh->GetName());

	const int _vertexCount = _mesh->GetControlPointsCount();
	_meshInfo.vertices.resize(_vertexCount);
	_meshInfo.boneWeights.resize(_vertexCount);


	//Postion
	FbxVector4* _controlPoints = _mesh->GetControlPoints();
	for (int i = 0;i < _vertexCount;i++)
	{
		_meshInfo.vertices[i].Position.x = static_cast<float>(_controlPoints[i].mData[0]);
		_meshInfo.vertices[i].Position.y = static_cast<float>(_controlPoints[i].mData[2]);
		_meshInfo.vertices[i].Position.z = static_cast<float>(_controlPoints[i].mData[1]);
	}

	const int _materialCount = _mesh->GetNode()->GetMaterialCount();
	_meshInfo.indices.resize(_materialCount);


	FbxGeometryElementMaterial* _geoElementMtrl = _mesh->GetElementMaterial();

	const int polygonSize = _mesh->GetPolygonSize(0);
	assert(polygonSize == 3);

	UINT _arrIdx[3];
	UINT _vertexCounter = 0;

	const int _triCount = _mesh->GetPolygonCount();

	for (int i = 0;i < _triCount;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			int _controlPointIndex = _mesh->GetPolygonVertex(i, j);
			_arrIdx[j] = _controlPointIndex;

			GetNormal(_mesh, &_meshInfo, _controlPointIndex, _vertexCounter);
			GetTargent(_mesh, &_meshInfo, _controlPointIndex, _vertexCounter);

			if (-1 == _mesh->GetTextureUVIndex(i, j))
			{
				GetUV(_mesh, &_meshInfo, _controlPointIndex, _controlPointIndex);
			}
			else
			{
				GetUV(_mesh, &_meshInfo, _controlPointIndex, _mesh->GetTextureUVIndex(i, j));
			}

			_vertexCounter++;
		}

		const  UINT subSetIdx = _geoElementMtrl->GetIndexArray().GetAt(i);
		_meshInfo.indices[subSetIdx].push_back(_arrIdx[0]);
		_meshInfo.indices[subSetIdx].push_back(_arrIdx[2]);
		_meshInfo.indices[subSetIdx].push_back(_arrIdx[1]);
	}


	LoadAnimationData(_mesh, &_meshInfo);
}

void FBXLoader::LoadMaterial(FbxSurfaceMaterial* _surfaceMaterial)
{
	FBXMaterialInfo _material{};

	_material.name = EH::ConvertWstring(_surfaceMaterial->GetName());
	_material.diffuse = GetMaterialData(_surfaceMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDisplacementFactor);
	_material.ambient = GetMaterialData(_surfaceMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	_material.Specular = GetMaterialData(_surfaceMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

	_material.diffuseTexName = GetTextureRelativeName(_surfaceMaterial, FbxSurfaceMaterial::sDiffuse);
	_material.normalTexName = GetTextureRelativeName(_surfaceMaterial, FbxSurfaceMaterial::sNormalMap);
	_material.SpecularTexName = GetTextureRelativeName(_surfaceMaterial, FbxSurfaceMaterial::sSpecular);

	m_vMeshes.back().materials.push_back(_material);
}

void FBXLoader::GetNormal(FbxMesh* _mesh, FBXMeshInfo* _container, int _idx, int _vertexCounter)
{
	if (0 == _mesh->GetElementNormalCount())
		return;

	FbxGeometryElementNormal* _normal = _mesh->GetElementNormal();
	UINT _normalIdx = 0;

	if (FbxGeometryElement::eByPolygonVertex == _normal->GetMappingMode())
	{
		if (FbxGeometryElement::eDirect == _normal->GetReferenceMode())
			_normalIdx = _vertexCounter;
		else
			_normalIdx = _normal->GetIndexArray().GetAt(_vertexCounter);
	}
	else if (FbxGeometryElement::eByControlPoint == _normal->GetMappingMode())
	{
		if (FbxGeometryElement::eDirect == _normal->GetReferenceMode())
			_normalIdx = _idx;
		else
			_normalIdx = _normal->GetIndexArray().GetAt(_idx);
	}

	FbxVector4 _vec = _normal->GetDirectArray().GetAt(_normalIdx);
	_container->vertices[_idx].Normal.x = static_cast<float>(_vec.mData[0]);
	_container->vertices[_idx].Normal.y = static_cast<float>(_vec.mData[2]);
	_container->vertices[_idx].Normal.z = static_cast<float>(_vec.mData[1]);
}

void FBXLoader::GetTargent(FbxMesh* _mesh, FBXMeshInfo* _container, int _idx, int _vertexCounter)
{
	if (0 == _mesh->GetElementTangentCount())
	{
		_container->vertices[_idx].Tangent.x = 1.f;
		_container->vertices[_idx].Tangent.y = 0.f;
		_container->vertices[_idx].Tangent.z = 0.f;
		return;
	}

	FbxGeometryElementTangent* _tangent = _mesh->GetElementTangent();
	UINT _tangentIdx = 0;

	if (FbxGeometryElement::eByPolygonVertex == _tangent->GetMappingMode())
	{
		if (FbxGeometryElement::eDirect == _tangent->GetReferenceMode())
			_tangentIdx = _vertexCounter;
		else
			_tangentIdx = _tangent->GetIndexArray().GetAt(_vertexCounter);
	}
	else if (FbxGeometryElement::eByControlPoint == _tangent->GetMappingMode())
	{
		if (FbxGeometryElement::eDirect == _tangent->GetReferenceMode())
			_tangentIdx = _idx;
		else
			_tangentIdx = _tangent->GetIndexArray().GetAt(_idx);
	}

	FbxVector4 _vec = _tangent->GetDirectArray().GetAt(_tangentIdx);
	_container->vertices[_idx].Tangent.x = static_cast<float>(_vec.mData[0]);
	_container->vertices[_idx].Tangent.y = static_cast<float>(_vec.mData[2]);
	_container->vertices[_idx].Tangent.z = static_cast<float>(_vec.mData[1]);
}

void FBXLoader::GetUV(FbxMesh* _mesh, FBXMeshInfo* _container, int _idx, int _vertexCounter)
{
	FbxVector2 _uv = _mesh->GetElementUV()->GetDirectArray().GetAt(_vertexCounter);
	_container->vertices[_idx].UV.x = static_cast<float>(_uv.mData[0]);
	_container->vertices[_idx].UV.y = 1.f - static_cast<float>(_uv.mData[1]);
}

Vec4 FBXLoader::GetMaterialData(FbxSurfaceMaterial* _surface, const char* _materialName, const char* _factorName)
{
	FbxDouble3 _mtrl;
	FbxDouble _factor = 0.f;

	FbxProperty _mtrlProperty = _surface->FindProperty(_materialName);
	FbxProperty _factorProperty = _surface->FindProperty(_factorName);

	if (_mtrlProperty.IsValid() && _factorProperty.IsValid())
	{
		_mtrl = _mtrlProperty.Get<FbxDouble3>();
		_factor = _factorProperty.Get<FbxDouble>();
	}

	Vec4 _ret = Vec4(
		static_cast<float>(_mtrl.mData[0] * _factor),
		static_cast<float>(_mtrl.mData[1] * _factor),
		static_cast<float>(_mtrl.mData[2] * _factor),
		static_cast<float>(_factor));

	return _ret;
}

wstring FBXLoader::GetTextureRelativeName(FbxSurfaceMaterial* _pSurface, const char* _materialProperty)
{
	string _name;

	FbxProperty _textureProperty = _pSurface->FindProperty(_materialProperty);
	if (_textureProperty.IsValid())
	{
		UINT _count = _textureProperty.GetSrcObjectCount();

		if (1 <= _count)
		{
			FbxFileTexture* _texture = _textureProperty.GetSrcObject<FbxFileTexture>(0);
			if (_texture)
				_name = _texture->GetRelativeFileName();
		}
	}


	return EH::ConvertWstring(_name);
}

void FBXLoader::CreateTextures()
{
	for (size_t i = 0;i < m_vMeshes.size();i++)
	{
		for (size_t j = 0;j < m_vMeshes[i].materials.size();j++)
		{
			// DiffuseTexture
			{
				/*wstring _relativePath = m_vMeshes[i].materials[j].diffuseTexName.c_str();
				wstring _fileName;
				wstring _fullPath = PATH + */
			}
		}
	}
}

void FBXLoader::CreateMaterials()
{
	for (size_t i = 0;i < m_vMeshes.size();i++)
	{
		for (size_t j = 0;j < m_vMeshes[i].materials.size();j++)
		{
		}
	}
}

void FBXLoader::LoadBones(FbxNode* _node, int _idx, int parentidx)
{
	FbxNodeAttribute* _attriBute = _node->GetNodeAttribute();

	if (_attriBute && FbxNodeAttribute::eSkeleton == _attriBute->GetAttributeType())
	{
		std::shared_ptr<FBXBoneInfo> _bone = std::make_shared<FBXBoneInfo>();
		_bone->boneName = EH::ConvertWstring(_node->GetName());
		_bone->parentIndex = parentidx;
		m_vBones.push_back(_bone);
	}

	const int _childCount = _node->GetChildCount();
	for (int i = 0;i < _childCount;i++)
		LoadBones(_node->GetChild(i), static_cast<int>(m_vBones.size()), _idx);
}

void FBXLoader::LoadAnimationInfo()
{
	m_pScene->FillAnimStackNameArray(OUT m_AnimNames);

	const int _animCount = m_AnimNames.GetCount();
	for (int i = 0;i < _animCount;i++)
	{
		FbxAnimStack* _animStack = m_pScene->FindMember<FbxAnimStack>(m_AnimNames[i]->Buffer());
		if (nullptr == _animStack)
			continue;

		std::shared_ptr<FBXAnimClipInfo> _animClip = std::make_shared<FBXAnimClipInfo>();
		_animClip->name = EH::ConvertWstring(_animStack->GetName());
		_animClip->keyFrames.resize(m_vBones.size());

		FbxTakeInfo* _takeInfo = m_pScene->GetTakeInfo(_animStack->GetName());
		_animClip->startTime = _takeInfo->mLocalTimeSpan.GetStart();
		_animClip->endTime = _takeInfo->mLocalTimeSpan.GetStop();
		_animClip->mode = m_pScene->GetGlobalSettings().GetTimeMode();

		m_vAnimClips.push_back(_animClip);
	}
}

void FBXLoader::LoadAnimationData(FbxMesh* _mesh, FBXMeshInfo* _meshInfo)
{
	const int _skinCount = _mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (_skinCount <= 0 || m_vAnimClips.empty())
		return;

	_meshInfo->hasAnimation = true;

	for (int i = 0;i < _skinCount;i++)
	{
		FbxSkin* _fbxSkin = static_cast<FbxSkin*>(_mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (_fbxSkin)
		{
			FbxSkin::EType _type = _fbxSkin->GetSkinningType();
			if (FbxSkin::eRigid == _type || FbxSkin::eLinear)
			{
				const int _clusterCount = _fbxSkin->GetClusterCount();
				for (int j = 0; j < _clusterCount; j++)
				{
					FbxCluster* _cluster = _fbxSkin->GetCluster(j);
					if (nullptr == _cluster->GetLink())
						continue;

					int _boneIdx = FindBoneIndex(_cluster->GetLink()->GetName());
					assert(_boneIdx >= 0);

					FbxAMatrix _matNodeTransform = GetTransform(_mesh->GetNode());
					LoadBoneWeight(_cluster, _boneIdx, _meshInfo);
					LoadOffsetMatrix(_cluster, _matNodeTransform, _boneIdx, _meshInfo);

					const int _animCount = m_AnimNames.Size();
					for (int k = 0;k < _animCount;k++)
					{
						LoadKeyFrame(k, _mesh->GetNode(), _cluster, _matNodeTransform, _boneIdx, _meshInfo);
					}
				}
			}
		}
	}

	FillBoneWeight(_mesh, _meshInfo);
}

void FBXLoader::LoadBoneWeight(FbxCluster* _cluster, int _boneIdx, FBXMeshInfo* _meshInfo)
{
	const int _indicesCount = _cluster->GetControlPointIndicesCount();
	for (int i = 0;i < _indicesCount;i++)
	{
		double _weight = _cluster->GetControlPointWeights()[i];
		int _vtxIdx = _cluster->GetControlPointIndices()[i];
		_meshInfo->boneWeights[_vtxIdx].AddWeights(_boneIdx, _weight);
	}
}

void FBXLoader::LoadOffsetMatrix(FbxCluster* _cluster, const FbxMatrix& _matNodeTransform, int _boneIdx, FBXMeshInfo* _meshInfo)
{
	FbxAMatrix _matClusterTrans;
	FbxAMatrix _matClutserLinkTrans;

	_cluster->GetTransformMatrix(_matClusterTrans);
	_cluster->GetTransformLinkMatrix(_matClutserLinkTrans);

	FbxVector4 _v0 = { 1, 0, 0, 0 };
	FbxVector4 _v1 = { 0, 0, 1, 0 };
	FbxVector4 _v2 = { 0, 1, 0, 0 };
	FbxVector4 _v3 = { 0, 0, 0, 1 };

	FbxAMatrix _matReflect;
	_matReflect[0] = _v0;
	_matReflect[1] = _v1;
	_matReflect[2] = _v2;
	_matReflect[3] = _v3;

	FbxAMatrix _matOffset;
	_matOffset = _matClutserLinkTrans.Inverse() * _matClusterTrans;
	_matOffset = _matReflect * _matOffset * _matReflect;

	m_vBones[_boneIdx]->matOffset = _matOffset.Transpose();
}

void FBXLoader::LoadKeyFrame(int _animIndex, FbxNode* _node, FbxCluster* _cluster, const FbxMatrix& _matNodeTransform, int _boneIdx, FBXMeshInfo* _container)
{
	if (m_vAnimClips.empty())
		return;

	FbxVector4 _v1 = { 1, 0, 0, 0 };
	FbxVector4 _v2 = { 0, 0, 1, 0 };
	FbxVector4 _v3 = { 0, 1, 0, 0 };
	FbxVector4 _v4 = { 0, 0, 0, 1 };
	FbxAMatrix _matReflect;
	_matReflect.mData[0] = _v1;
	_matReflect.mData[0] = _v2;
	_matReflect.mData[0] = _v3;
	_matReflect.mData[0] = _v4;

	FbxTime::EMode _timeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	FbxAnimStack* _animStack = m_pScene->FindMember<FbxAnimStack>(m_AnimNames[_animIndex]->Buffer());
	m_pScene->SetCurrentAnimationStack(OUT _animStack);
	FbxLongLong _startFrame = m_vAnimClips[_animIndex]->startTime.GetFrameCount(_timeMode);
	FbxLongLong _endFrame = m_vAnimClips[_animIndex]->endTime.GetFrameCount(_timeMode);

	for (FbxLongLong _frame = _startFrame;_frame < _endFrame;_frame++)
	{
		FBXKeyFrameInfo _keyFrameInfo = {};
		FbxTime _fbxTime = 0;

		_fbxTime.SetFrame(_frame, _timeMode);

		FbxAMatrix _matFromNode = _node->EvaluateGlobalTransform(_fbxTime);
		FbxAMatrix _matTransform = _matFromNode.Inverse() * _cluster->GetLink()->EvaluateGlobalTransform(_fbxTime);
		_matTransform = _matReflect * _matTransform * _matReflect;

		_keyFrameInfo.time = _fbxTime.GetSecondDouble();
		_keyFrameInfo.matTransform = _matTransform;

		m_vAnimClips[_animIndex]->keyFrames[_boneIdx].push_back(_keyFrameInfo);
	}

}

int FBXLoader::FindBoneIndex(string name)
{
	wstring _boneNames = wstring(name.begin(), name.end());

	for (size_t i = 0;i < m_vBones.size();i++)
	{
		if (_boneNames == m_vBones[i]->boneName)
			return i;
	}

	return -1;
}

FbxAMatrix FBXLoader::GetTransform(FbxNode* _node)
{
	const FbxVector4 _translation = _node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 _rotation = _node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 _scalling = _node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(_translation, _rotation, _scalling);
}

void FBXLoader::FillBoneWeight(FbxMesh* _mesh, FBXMeshInfo* _meshInfo)
{
	const int _size = static_cast<int>(_meshInfo->boneWeights.size());
	for (int _v = 0;_v < _size;_v++)
	{
		BoneWeight& _boneWeight = _meshInfo->boneWeights[_v];
		_boneWeight.Normalize();

		float _animBoneIndex[4] = {};
		float _animBoneWeight[4] = {};

		const int _weightCount = static_cast<int>(_boneWeight.boneWeights.size());
		for (int _w = 0;_w < _weightCount;_w++)
		{
			_animBoneIndex[_w] = static_cast<float>(_boneWeight.boneWeights[_w].first);
			_animBoneWeight[_w] = static_cast<float>(_boneWeight.boneWeights[_w].second);
		}

		memcpy(&_meshInfo->vertices[_v].Indices, _animBoneIndex, sizeof(Vec4));
		memcpy(&_meshInfo->vertices[_v].Weights, _animBoneWeight, sizeof(Vec4));
	}
}