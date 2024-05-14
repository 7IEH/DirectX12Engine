#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"
#include "CEngine.h"

StructuredBuffer::StructuredBuffer()
	:m_iElementCount(0)
	, m_iElementSize(0)
	, m_iResourceState{}
	, m_pSRVHandle{}
	, m_pUAVHandle{}
	, m_eType(SB_TYPE::READ_ONLY)
	, m_bSysMemMove(FALSE)
{
}

StructuredBuffer::~StructuredBuffer()
{
}

HRESULT StructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bSysMemMove, void* _pSysMem)
{
	HRESULT _hr = S_OK;

	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	m_eType = _eType;

	UINT64 _iBufferSize = static_cast<UINT64>(_iElementSize) * _iElementCount;
	D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(_iBufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	D3D12_HEAP_PROPERTIES _property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	DEVICE->CreateCommittedResource(
		&_property,
		D3D12_HEAP_FLAG_NONE,
		&_tDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&m_pBuffer)
	);

	if (_pSysMem)
	{
		SetData(_pSysMem, m_iElementCount);
	}

	D3D12_DESCRIPTOR_HEAP_DESC _srvHeapDesc = {};
	_srvHeapDesc.NumDescriptors = 1;
	_srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	_srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	_hr = DEVICE->CreateDescriptorHeap(&_srvHeapDesc, IID_PPV_ARGS(&m_pSRV));

	m_pSRVHandle = m_pSRV->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
	_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	_srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	_srvDesc.Buffer.FirstElement = 0;
	_srvDesc.Buffer.NumElements = m_iElementCount;
	_srvDesc.Buffer.StructureByteStride = m_iElementSize;
	_srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	DEVICE->CreateShaderResourceView(m_pBuffer.Get(), &_srvDesc, m_pSRVHandle);

	if (SB_TYPE::READ_WRITE == m_eType)
	{
		D3D12_DESCRIPTOR_HEAP_DESC _uavHeapDesc = {};
		_uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		_uavHeapDesc.NumDescriptors = 1;
		_uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		_hr = DEVICE->CreateDescriptorHeap(&_uavHeapDesc, IID_PPV_ARGS(&m_pUAV));

		m_pUAVHandle = m_pUAV->GetCPUDescriptorHandleForHeapStart();

		D3D12_UNORDERED_ACCESS_VIEW_DESC _uavDesc = {};
		_uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		_uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		_uavDesc.Buffer.FirstElement = 0;
		_uavDesc.Buffer.NumElements = m_iElementCount;
		_uavDesc.Buffer.StructureByteStride = m_iElementSize;
		_uavDesc.Buffer.CounterOffsetInBytes = 0;
		_uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		DEVICE->CreateUnorderedAccessView(m_pBuffer.Get(), nullptr, &_uavDesc, m_pUAVHandle);
	}

	return _hr;
}

void StructuredBuffer::UpdateData(UINT _RegisterNum)
{
	Device::GetInst()->SetSRV(m_pSRVHandle, _RegisterNum);
}

int StructuredBuffer::UpdateData_CS_SRV(UINT _RegisterNum)
{
	Device::GetInst()->SetSRV_CS(m_pSRVHandle, _RegisterNum);
	return S_OK;
}

int StructuredBuffer::UpdateData_CS_UAV(UINT _RegisterNum)
{
	Device::GetInst()->SetUAV_CS(m_pUAVHandle, _RegisterNum);
	return S_OK;
}

void StructuredBuffer::Clear(UINT _RegisterNum)
{
}

void StructuredBuffer::Clear_CS_SRV()
{
}

void StructuredBuffer::Clear_CS_UAV()
{
}

void StructuredBuffer::SetData(void* _SysMem, UINT _ElementCount)
{
	UINT64 _iBufferSize = static_cast<UINT64>(m_iElementSize) * m_iElementCount;
	ComPtr<ID3D12Resource> readBuffer = nullptr;
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(_iBufferSize, D3D12_RESOURCE_FLAG_NONE);
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	DEVICE->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&readBuffer));
	
	UINT8* dataBegin = nullptr;
	D3D12_RANGE readRange{ 0, 0 };
	readBuffer->Map(0, &readRange, reinterpret_cast<void**>(&dataBegin));
	memcpy(dataBegin, _SysMem, _iBufferSize);
	readBuffer->Unmap(0, nullptr);

	// Common -> Copy
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

		Device::GetInst()->GetResCmdList()->ResourceBarrier(1, &barrier);
	}

	Device::GetInst()->GetResCmdList()->CopyBufferRegion(m_pBuffer.Get(), 0, readBuffer.Get(), 0, _iBufferSize);

	// Copy -> Common
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON);

		Device::GetInst()->GetResCmdList()->ResourceBarrier(1, &barrier);
	}

	Device::GetInst()->FlushResourceCommandQueue();
}

void StructuredBuffer::GetData(void* _Dest, UINT _ElementCount)
{
}