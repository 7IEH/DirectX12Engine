#include "pch.h"
#include "CAnimComputeShader.h"

AnimComputeShader::AnimComputeShader(UINT _threadX, UINT _threadY, UINT _threadZ)
    :ComputeShader(_threadX, _threadY, _threadZ)
{
}

AnimComputeShader::~AnimComputeShader()
{
}

HRESULT AnimComputeShader::UpdateData()
{
    HRESULT _hr = S_OK;
    
    m_pBoneFrameBuffer->UpdateData_CS_SRV(11);
    m_pBoneOffsetBuffer->UpdateData_CS_SRV(12);
    m_pBoneMatBuffer->UpdateData_CS_UAV(0);

    UpdateGroupCount();

    return _hr;
}

void AnimComputeShader::UpdateGroupCount()
{
    UINT _iGroupCount = (m_pBoneMatBuffer->GetElementCount() / 256) + 1;
    SetGroupX(_iGroupCount);
}