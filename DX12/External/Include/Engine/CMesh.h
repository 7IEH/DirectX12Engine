#pragma once
#include "CAsset.h"
#include "CStructuredBuffer.h"

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>				IB;
	D3D12_INDEX_BUFFER_VIEW				IBView;
	DXGI_FORMAT							format;
	UINT								count;
	void*								IndexInfo;
};

struct KeyFrameInfo
{
	double	time;
	int		frame;
	Vec3	scale;
	Vec4	rotation;
	Vec3	translate;
};

struct BoneInfo
{
	wstring		boneName;
	int			parentIdx;
	Matrix		matOffset;
};

struct AnimClipInfo
{
	wstring animName;
	int		frameCount;
	double	duration;
	vector<vector<KeyFrameInfo>>	KeyFrame;
};

class Mesh :
	public Asset
{
private:
	ComPtr<ID3D12Resource>				m_VB;
	void*								m_VertexInfo;

	D3D12_VERTEX_BUFFER_VIEW			m_VBView;
	vector<IndexBufferInfo>				m_IBInfo;

	// Animation
	vector<AnimClipInfo>				m_vAnimClips;
	vector<BoneInfo>					m_vBones;

	StructuredBuffer*					m_pOffsetBuffer;
	vector<StructuredBuffer*>			m_pFrameBuffer;

public:
	UINT GetSubSetCount() { return static_cast<UINT>(m_IBInfo.size()); }

	StructuredBuffer* GetOffsetBuffer() { return m_pOffsetBuffer; }
	StructuredBuffer* GetFrameBuffer(int _idx) { return m_pFrameBuffer[_idx]; }

	bool			  IsAnimMesh() { return !m_vAnimClips.empty(); }

	const vector<BoneInfo>* GetBones()				{ return &m_vBones; }
	const vector<AnimClipInfo>* GetAnimClip()		{ return &m_vAnimClips; }

public:
	void Create(vector<VertexInfo>& _VBdata, UINT _VertexCount, vector<UINT>& _IBData, UINT _IndexCount);
	void UpdateData(UINT _idx);
	
	static Ptr<Mesh>		CreateFromFBX(const struct FBXMeshInfo* _meshInfo,class FBXLoader& _loader);

public:
	void Render(UINT _idx);
	void Render_Instancing(UINT _particleCount);

private:
	void CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, vector<VertexInfo>& _vertexData, vector<UINT>& _indexData);
	void DrawIndexed(UINT _idx);
	void SetBuffer(BUFFER_TYPE _bufferType, UINT _idx);

	void CreateBonesAndAnimation(class FBXLoader& _loader);
	Matrix GetMatrix(FbxAMatrix& _matrix);

public:
	CLONE(Mesh)
	Mesh();
	virtual ~Mesh();

	friend class MeshData;
};

