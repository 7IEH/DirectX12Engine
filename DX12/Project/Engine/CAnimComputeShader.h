#pragma once
#include "CComputeShader.h"
#include "CStructuredBuffer.h"

class AnimComputeShader :
    public ComputeShader
{
private:
    StructuredBuffer* m_pBoneMatBuffer;
    StructuredBuffer* m_pBoneFrameBuffer;
    StructuredBuffer* m_pBoneOffsetBuffer;

private:
    virtual HRESULT UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() {};

public:
    void SetBoneBuffer(StructuredBuffer* _pBuffer) { m_pBoneMatBuffer = _pBuffer; }
    void SetBoneFrameBuffer(StructuredBuffer* _pBuffer) { m_pBoneFrameBuffer = _pBuffer; }
    void SetBoneOffsetBuffer(StructuredBuffer* _pBuffer) { m_pBoneOffsetBuffer = _pBuffer; }

public:
    CLONE(AnimComputeShader);
    AnimComputeShader(UINT _threadX, UINT _threadY, UINT _threadZ);
    virtual ~AnimComputeShader();
};

