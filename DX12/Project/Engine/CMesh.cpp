#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

Mesh::Mesh()
	:Asset(ASSET_TYPE::MESH)
	, m_IndexCount(0)
	, m_VertexInfo(nullptr)
	, m_IndexInfo(nullptr)
{
}

Mesh::~Mesh()
{
	if (nullptr != m_VertexInfo)
		delete m_VertexInfo;

	if (nullptr != m_IndexInfo)
		delete m_IndexInfo;
}

void Mesh::Create(void* _VBData, UINT _VertexCount, void* _IBData, UINT _IndexCount)
{
	m_IndexCount = _IndexCount;

	CreateBuffer(BUFFER_TYPE::VERTEX, _VertexCount, _VBData);
	CreateBuffer(BUFFER_TYPE::INDEX, _IndexCount, _IBData);

	// 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
	m_VertexInfo = new VertexInfo[_VertexCount];
	m_IndexInfo = new UINT[_IndexCount];

	memcpy(m_VertexInfo, _VBData, sizeof(VertexInfo) * _VertexCount);
	memcpy(m_IndexInfo, _IBData, sizeof(UINT) * _IndexCount);
}

void Mesh::UpdateData()
{
	SetBuffer(BUFFER_TYPE::VERTEX);
	SetBuffer(BUFFER_TYPE::INDEX);
}

void Mesh::Render()
{
	UpdateData();
	DrawIndexed();
}

void Mesh::CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, void* _data)
{
	
}

void Mesh::DrawIndexed()
{
	
}

void Mesh::Render_Instancing(UINT _particleCount)
{
	
}

void Mesh::SetBuffer(BUFFER_TYPE _bufferType)
{
	
}