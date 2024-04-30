#pragma once

#include "CEntity.h"

class ConstantBuffer
	:public Entity
{
private:
	ComPtr<ID3D12Resource>	m_pCB;
	BYTE* m_pData;

	CONSTANT_TYPE			m_Type;

	UINT					m_iElementSize;
	UINT					m_iElementCount;

	UINT					m_iCurrentIdx;

	// Local 영역에서 GPU에게 업로드할 힙
	ComPtr<ID3D12DescriptorHeap>		m_pLocalHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_CpuHandleBegin;
	UINT								m_iHandleIncrementSize;

public:
	CONSTANT_TYPE	GetCBType() { return m_Type; }
	void			SetCBType(CONSTANT_TYPE _type) { m_Type = _type; }

public:
	HRESULT			Create(UINT _elementSize, UINT _elementCount, CONSTANT_TYPE _type);
	HRESULT			CreateBuffer();
	HRESULT			CreateBufferView();
	void			SetData(void* _data, int _size, int _count);
	void			UpdateData(void* _data, int _size, int _count);
	void			UpdateData_CS(void* _data, int _size, int _count);

	void			Clear();

	CLONE_DISABLE(ConstantBuffer)

public:
	ConstantBuffer();
	~ConstantBuffer();
};

