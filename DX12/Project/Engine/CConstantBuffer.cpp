#include "pch.h"
#include "CConstantBuffer.h"

#include "CDevice.h"

ConstantBuffer::ConstantBuffer()
	: m_Type(CONSTANT_TYPE::END)
	, m_iElementCount(0)
	, m_iElementSize(0)
	, m_pData(nullptr)
	, m_iCurrentIdx(0)
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_pCB)
	{
		if (m_pCB != nullptr)
			m_pCB->Unmap(0, nullptr);

		m_pCB = nullptr;
	}
}

HRESULT ConstantBuffer::Create(UINT _elementSize, UINT _elementCount, CONSTANT_TYPE _type)
{
	m_Type = _type;

	m_iElementSize = _elementSize;
	m_iElementCount = _elementCount;

	UINT _iBufferSize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES _heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(_iBufferSize);

	DEVICE->CreateCommittedResource(
		&_heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&_tDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pCB));

	m_pCB->Map(0, nullptr, reinterpret_cast<void**>(&m_pData));
	return S_OK;
}

void ConstantBuffer::SetData(void* _data, int _size, int _count)
{
	::memcpy(&m_pData[m_iCurrentIdx * m_iElementSize], _data, _size);
}

void ConstantBuffer::UpdateData(void* _data, int _size, int _count)
{
	SetData(_data, _size, _count);

	D3D12_GPU_VIRTUAL_ADDRESS _objAddress = m_pCB->GetGPUVirtualAddress();
	_objAddress += m_iCurrentIdx * m_iElementSize;

	CMDLIST->SetGraphicsRootConstantBufferView((UINT)m_Type, _objAddress);
	m_iCurrentIdx++;
}

void ConstantBuffer::UpdateData_CS(void* _data, int _size, int _count)
{
	SetData(_data, _size, _count);

	D3D12_GPU_VIRTUAL_ADDRESS _objAddress = m_pCB->GetGPUVirtualAddress();
	_objAddress += m_iCurrentIdx * m_iElementSize;

	CMDLIST->SetGraphicsRootConstantBufferView((UINT)m_Type, _objAddress);
	m_iCurrentIdx++;
}

void ConstantBuffer::Clear()
{
	m_iCurrentIdx = 0;
}
