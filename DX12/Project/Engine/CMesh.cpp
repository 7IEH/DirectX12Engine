#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CAssetMgr.h"

Mesh::Mesh()
	:Asset(ASSET_TYPE::MESH)
	, m_IndexCount(0)
	, m_VertexInfo(nullptr)
	, m_IndexInfo(nullptr)
	, m_VBView{}
	, m_IBView{}
{
}

Mesh::~Mesh()
{
	if (nullptr != m_VertexInfo)
		delete m_VertexInfo;

	if (nullptr != m_IndexInfo)
		delete m_IndexInfo;
}

void Mesh::Create(vector<VertexInfo>& _VBdata, UINT _VertexCount, vector<UINT>& _IBData, UINT _IndexCount)
{
	m_IndexCount = _IndexCount;

	CreateBuffer(BUFFER_TYPE::VERTEX, _VertexCount, _VBdata, _IBData);
	CreateBuffer(BUFFER_TYPE::INDEX, _IndexCount, _VBdata, _IBData);

	// 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
	m_VertexInfo = new VertexInfo[_VertexCount];
	m_IndexInfo = new UINT[_IndexCount];

	memcpy(m_VertexInfo, &_VBdata[0], sizeof(VertexInfo) * _VertexCount);
	memcpy(m_IndexInfo, &_IBData[0], sizeof(UINT) * _IndexCount);
}

void Mesh::UpdateData()
{
	SetBuffer(BUFFER_TYPE::VERTEX);
	SetBuffer(BUFFER_TYPE::INDEX);
}

void Mesh::Render()
{
	Device::GetInst()->CommitTable();
	UpdateData();
	DrawIndexed();
}

void Mesh::CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, vector<VertexInfo>& _vertexData, vector<UINT>& _indexData)
{
	D3D12_HEAP_PROPERTIES _heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	switch (_bufferType)
	{
	case BUFFER_TYPE::VERTEX:
	{
		D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexInfo) * _count);
		DEVICE->CreateCommittedResource(
			&_heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&_tDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_VB)
		);

		void* vertexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		m_VB->Map(0, &readRange, &vertexDataBuffer);
		::memcpy(vertexDataBuffer, &_vertexData[0], sizeof(VertexInfo) * _count);
		m_VB->Unmap(0, nullptr);

		m_VBView.BufferLocation = m_VB->GetGPUVirtualAddress();
		m_VBView.StrideInBytes = sizeof(VertexInfo);
		m_VBView.SizeInBytes = sizeof(VertexInfo) * _count;
	}
	break;
	case BUFFER_TYPE::INDEX:
	{
		D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT) * _count);
		DEVICE->CreateCommittedResource(
			&_heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&_tDesc,
			D3D12_RESOURCE_STATE_INDEX_BUFFER,
			nullptr,
			IID_PPV_ARGS(&m_IB)
		);

		void* IndexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		m_IB->Map(0, &readRange, &IndexDataBuffer);
		::memcpy(IndexDataBuffer, &_indexData[0], sizeof(UINT) * _count);
		m_IB->Unmap(0, nullptr);

		m_IBView.BufferLocation = m_IB->GetGPUVirtualAddress();
		m_IBView.Format = DXGI_FORMAT_R32_UINT;
		m_IBView.SizeInBytes = sizeof(UINT) * _count;
	}
	break;
	case BUFFER_TYPE::END:
		break;
	default:
		break;
	}
}

void Mesh::DrawIndexed()
{
	CMDLIST->DrawIndexedInstanced(m_IndexCount, 1, 0, 0, 0);
}

void Mesh::Render_Instancing(UINT _particleCount)
{
}

void Mesh::SetBuffer(BUFFER_TYPE _bufferType)
{
	switch (_bufferType)
	{
	case BUFFER_TYPE::VERTEX:
	{
		CMDLIST->IASetVertexBuffers(0, 1, &m_VBView);
	}
	break;
	case BUFFER_TYPE::INDEX:
	{
		CMDLIST->IASetIndexBuffer(&m_IBView);
	}
	break;
	case BUFFER_TYPE::END:
		break;
	default:
		break;
	}
}