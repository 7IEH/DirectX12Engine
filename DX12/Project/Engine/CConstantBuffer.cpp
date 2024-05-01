#include "pch.h"
#include "CConstantBuffer.h"

#include "CDevice.h"

ConstantBuffer::ConstantBuffer()
	: m_Type(CONSTANT_TYPE::END)
	, m_iElementCount(0)
	, m_iElementSize(0)
	, m_pData(nullptr)
	, m_iCurrentIdx(0)
	, m_CpuHandleBegin{}
	, m_iHandleIncrementSize(0)
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
	HRESULT _hr = E_FAIL;
	m_Type = _type;

	m_iElementSize = (_elementSize + 255) & ~255;
	m_iElementCount = _elementCount;

	_hr = CreateBuffer();
	_hr = CreateBufferView();

	return _hr;
}


HRESULT ConstantBuffer::CreateBuffer()
{
	HRESULT _hr = E_FAIL;
	UINT _iBufferSize = m_iElementSize * m_iElementCount;
	D3D12_HEAP_PROPERTIES _heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(_iBufferSize);

	_hr = DEVICE->CreateCommittedResource(
		&_heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&_tDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pCB));

	m_pCB->Map(0, nullptr, reinterpret_cast<void**>(&m_pData));

	return _hr;
}

HRESULT ConstantBuffer::CreateBufferView()
{
	HRESULT _hr = E_FAIL;
	D3D12_DESCRIPTOR_HEAP_DESC _tDesc = {};
	_tDesc.NumDescriptors = m_iElementCount;
	_tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	_tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	_hr = DEVICE->CreateDescriptorHeap(&_tDesc, IID_PPV_ARGS(&m_pLocalHeap));

	m_CpuHandleBegin = m_pLocalHeap->GetCPUDescriptorHandleForHeapStart();
	m_iHandleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (UINT i = 0;i < m_iElementCount;i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE _cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_CpuHandleBegin, m_iHandleIncrementSize * i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC _tDesc2 = {};
		_tDesc2.BufferLocation = m_pCB->GetGPUVirtualAddress() + static_cast<UINT64>(m_iElementSize) * i;
		_tDesc2.SizeInBytes = m_iElementSize;

		DEVICE->CreateConstantBufferView(&_tDesc2, _cbvHandle);
	}

	return _hr;
}

void ConstantBuffer::SetData(void* _data, int _size, int _count)
{
	::memcpy(&m_pData[m_iCurrentIdx * m_iElementSize], _data, _size);
}

void ConstantBuffer::UpdateData(void* _data, int _size, int _count)
{
	SetData(_data, _size, _count);

	Device::GetInst()->SetCBV(CD3DX12_CPU_DESCRIPTOR_HANDLE(m_CpuHandleBegin, m_iHandleIncrementSize * m_iCurrentIdx),m_Type);
	m_iCurrentIdx++;
}

void ConstantBuffer::UpdateData_Constant(void* _data, int _size, int _count)
{
	SetData(_data, _size, _count);

	D3D12_GPU_VIRTUAL_ADDRESS _address = m_pCB->GetGPUVirtualAddress();
	_address += m_iCurrentIdx * m_iElementSize;

	CMDLIST->SetGraphicsRootConstantBufferView(0, _address);
}

void ConstantBuffer::UpdateData_CS(void* _data, int _size, int _count)
{
}

void ConstantBuffer::Clear()
{
	m_iCurrentIdx = 0;
}
