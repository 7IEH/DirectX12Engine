#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};

class StructuredBuffer :
    public Entity
{
private:
    ComPtr<ID3D12Resource>          m_pBuffer;
    ComPtr<ID3D12DescriptorHeap>    m_pSRV;
    ComPtr<ID3D12DescriptorHeap>    m_pUAV;

    D3D12_CPU_DESCRIPTOR_HANDLE     m_pSRVHandle;
    D3D12_CPU_DESCRIPTOR_HANDLE     m_pUAVHandle;

    UINT						    m_iElementSize;
    UINT						    m_iElementCount;
    D3D12_RESOURCE_STATES		    m_iResourceState;

    SB_TYPE                         m_eType;
    bool                            m_bSysMemMove;

public:
    HRESULT Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, bool _bSysMemMove = FALSE, void* _pSysMem = nullptr);

    void UpdateData(UINT _RegisterNum);
    int UpdateData_CS_SRV(UINT _RegisterNum);
    int UpdateData_CS_UAV(UINT _RegisterNum);

    void Clear(UINT _RegisterNum);
    void Clear_CS_SRV();
    void Clear_CS_UAV();


    void SetData(void* _SysMem, UINT _ElementCount = 0);
    void GetData(void* _Dest, UINT _ElementCount = 0);

    UINT GetElementSize() { return m_iElementSize; }
    UINT GetElementCount() { return m_iElementCount; }

public:
    CLONE(StructuredBuffer);
    StructuredBuffer();
    ~StructuredBuffer();
};

