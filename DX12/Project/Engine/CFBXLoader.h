#pragma once

struct FBXMaterialInfo
{
	Vec4	diffuse;
	Vec4	ambient;
	Vec4	Specular;
	wstring name;
	wstring diffuseTexName;
	wstring normalTexName;
	wstring SpecularTexName;
};

struct BoneWeight
{
	using Pair = std::pair<int, double>;
	vector<Pair> boneWeights;

	void AddWeights(UINT index, double weight)
	{
		if (weight <= 0.f)
			return;

		auto findit = std::find_if(boneWeights.begin(), boneWeights.end(),
			[=](const Pair& p) {return p.second < weight;});

		if (findit != boneWeights.end())
			boneWeights.insert(findit, Pair(index, weight));
		else
			boneWeights.push_back(Pair(index, weight));

		if (boneWeights.size() > 4)
			boneWeights.pop_back();
	}

	void Normalize()
	{
		double sum = 0.f;
		std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) {sum += p.second;});
		std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
	}
};

struct FBXMeshInfo
{
	wstring			name;
	vector<VertexInfo>			vertices;
	vector<vector<UINT>>		indices;
	vector<FBXMaterialInfo>	materials;
	vector<BoneWeight>			boneWeights;
	bool						hasAnimation;
};

struct FBXKeyFrameInfo
{
	FbxMatrix matTransform;
	double time;
};

struct FBXBoneInfo
{
	wstring					boneName;
	int						parentIndex;
	FbxAMatrix				matOffset;
};

struct FBXAnimClipInfo
{
	wstring			name;
	FbxTime			startTime;
	FbxTime			endTime;
	FbxTime::EMode	mode;
	vector<vector<FBXKeyFrameInfo>>	keyFrames;
};

class FBXLoader
{
private:
	FbxManager*										m_pManager;
	FbxScene*										m_pScene;
	FbxImporter*									m_pImporter;
	wstring											m_pResDirectory;

	vector<FBXMeshInfo>								m_vMeshes;
	vector<std::shared_ptr<FBXBoneInfo>>			m_vBones;
	vector<std::shared_ptr<FBXAnimClipInfo>>		m_vAnimClips;
	FbxArray<FbxString*>							m_AnimNames;

public:
	void LoadFBX(const wstring& _path);

public:
	int GetMeshCount() { return static_cast<int>(m_vMeshes.size()); }
	const FBXMeshInfo& GetMesh(int _idx) { return m_vMeshes[_idx]; }
	vector<std::shared_ptr<FBXBoneInfo>>& GetBones() { return m_vBones; }
	vector<std::shared_ptr<FBXAnimClipInfo>>& GetAnimClip() { return m_vAnimClips; }

private:
	void Import(const wstring& _path);
	void ParseNode(FbxNode* _node);
	void LoadMesh(FbxMesh* _mesh);
	void LoadMaterial(FbxSurfaceMaterial* _surfaceMaterial);

	void GetNormal(FbxMesh* _mesh, FBXMeshInfo* _container, int _idx, int _vertexCounter);
	void GetTargent(FbxMesh* _mesh, FBXMeshInfo* _container, int _idx, int _vertexCounter);
	void GetUV(FbxMesh* _mesh, FBXMeshInfo* _container, int _idx, int vertexCounter);
	Vec4 GetMaterialData(FbxSurfaceMaterial* _surface, const char* _materialName, const char* _factorName);
	wstring GetTextureRelativeName(FbxSurfaceMaterial* _pSurface, const char* _materialProperty);


	void CreateTextures();
	void CreateMaterials();

	// Animation
	void LoadBones(FbxNode* _node) { LoadBones(_node, 0, -1); }
	void LoadBones(FbxNode* _node, int _idx, int parentidx);
	void LoadAnimationInfo();
	
	void LoadAnimationData(FbxMesh* _mesh, FBXMeshInfo* _meshInfo);
	void LoadBoneWeight(FbxCluster* _cluster, int _boneIdx, FBXMeshInfo* _meshInfo);
	void LoadOffsetMatrix(FbxCluster* _cluster, const FbxMatrix& _matNodeTransform, int _boneIdx, FBXMeshInfo* _meshInfo);
	void LoadKeyFrame(int _animIndex, FbxNode* _node, FbxCluster* _cluster, const FbxMatrix& _matNodeTransform, int _boneIdx, FBXMeshInfo* _container);

	int FindBoneIndex(string name);
	FbxMatrix GetTransform(FbxNode* _node);

	void FillBoneWeight(FbxMesh* _mesh, FBXMeshInfo* _meshInfo);

public:
	FBXLoader();
	virtual ~FBXLoader();
};