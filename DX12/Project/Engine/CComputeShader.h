#pragma once
#include "CShader.h"
class ComputeShader :
    public Shader
{
private:
    ComPtr<ID3DBlob>                        m_CSBlob;
    ComPtr<ID3D12PipelineState>             m_pPLState;
    D3D12_COMPUTE_PIPELINE_STATE_DESC       m_pPLDesc;

    UINT                                    m_iGroupX;
    UINT                                    m_iGroupY;
    UINT                                    m_iGroupZ;

protected:
    const UINT                              m_iThreadX;
    const UINT                              m_iThreadY;
    const UINT                              m_iThreadZ;

public:
    void Execute();

protected:
    UINT GetGroupX() { return m_iGroupX; }
    UINT GetGroupY() { return m_iGroupY; }
    UINT GetGroupZ() { return m_iGroupZ; }

    void SetGroupX(UINT _iGroup) { m_iGroupX = _iGroup; }
    void SetGroupY(UINT _iGroup) { m_iGroupY = _iGroup; }
    void SetGroupZ(UINT _iGroup) { m_iGroupZ = _iGroup; }

public:
    void Default_Create(const wstring& _shaderPath, const string& _csEntry);

private:
    virtual HRESULT UpdateData() = 0;
    virtual void UpdateGroupCount() = 0;
    virtual void Clear() = 0;

private:
    void CreateShader(const wstring& _path, const string& _entry);
    void SetShader();
    void CreatePipeLineState();

public:
    ComputeShader(UINT _threadX, UINT _threadY, UINT _threadZ);
    virtual ~ComputeShader();
};