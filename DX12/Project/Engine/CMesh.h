#pragma once
#include "CAsset.h"
class Mesh :
    public Asset
{
private:
	ComPtr<ID3D12Resource>		m_VB;
	ComPtr<ID3D12Resource>		m_IB;

	UINT						m_IndexCount;

	void*						m_VertexInfo;
	void*						m_IndexInfo;

	D3D12_VERTEX_BUFFER_VIEW    m_VBView;
	D3D12_INDEX_BUFFER_VIEW		m_IBView;

public:
	CLONE(Mesh)
	Mesh();
	virtual ~Mesh();

public:
	void Create(vector<VertexInfo>& _data, UINT _VertexCount, void* _IBData, UINT _IndexCount);
	void UpdateData();

public:
	void Render();
	void Render_Instancing(UINT _particleCount);

private:
	void CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, vector<VertexInfo>& _data);
	void DrawIndexed();
	void SetBuffer(BUFFER_TYPE _bufferType);
};

