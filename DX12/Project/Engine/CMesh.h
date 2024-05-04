#pragma once
#include "CAsset.h"

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>				IB;
	D3D12_INDEX_BUFFER_VIEW				IBView;
	DXGI_FORMAT							format;
	UINT								count;
	void*								IndexInfo;
};

class Mesh :
	public Asset
{
private:
	ComPtr<ID3D12Resource>				m_VB;
	void*								m_VertexInfo;

	D3D12_VERTEX_BUFFER_VIEW			m_VBView;
	vector<IndexBufferInfo>				m_IBInfo;
public:
	UINT GetSubSetCount() { return static_cast<UINT>(m_IBInfo.size()); }

public:
	void Create(vector<VertexInfo>& _VBdata, UINT _VertexCount, vector<UINT>& _IBData, UINT _IndexCount);
	void UpdateData(UINT _idx);

public:
	void Render(UINT _idx);
	void Render_Instancing(UINT _particleCount);

private:
	void CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, vector<VertexInfo>& _vertexData, vector<UINT>& _indexData);
	void DrawIndexed(UINT _idx);
	void SetBuffer(BUFFER_TYPE _bufferType, UINT _idx);


public:
	CLONE(Mesh)
	Mesh();
	virtual ~Mesh();

	friend class MeshData;
};

