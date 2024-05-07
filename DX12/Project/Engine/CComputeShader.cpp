#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

ComputeShader::ComputeShader(UINT _threadX, UINT _threadY, UINT _threadZ)
	: Shader(ASSET_TYPE::COMPUTE_SHADER)
	, m_iThreadX(_threadX)
	, m_iThreadY(_threadY)
	, m_iThreadZ(_threadZ)
	, m_iGroupX(1)
	, m_iGroupY(1)
	, m_iGroupZ(1)
	, m_pPLDesc{}
{
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::Default_Create(const wstring& _shaderPath, const string& _csEntry)
{
	CreateShader(_shaderPath, _csEntry);
	CreatePipeLineState();
}

void ComputeShader::CreateShader(const wstring& _path, const string& _entry)
{
	UINT _iCompileFlag = 0;
#ifdef _DEBUG
	_iCompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	wstring _shaderPath = PATH + _path;

	if (FAILED(::D3DCompileFromFile(_shaderPath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _entry.c_str(), "cs_5_0", _iCompileFlag, 0
		, m_CSBlob.GetAddressOf(), m_pErrorBlob.GetAddressOf())))
	{
		HandleError(L"ComputeShader Create Fail", 0);
	}

	SetShader();
}

void ComputeShader::SetShader()
{
	D3D12_SHADER_BYTECODE _tDesc = {};
	_tDesc.pShaderBytecode = m_CSBlob->GetBufferPointer();
	_tDesc.BytecodeLength = m_CSBlob->GetBufferSize();
	m_pPLDesc.CS = _tDesc;
}

void ComputeShader::CreatePipeLineState()
{
	m_pPLDesc.pRootSignature = Device::GetInst()->GetComSignature();

	if (FAILED(DEVICE->CreateComputePipelineState(&m_pPLDesc, IID_PPV_ARGS(&m_pPLState))))
	{
		HandleError(L"Create Failed ComputePipeLineState", 0);
	}
}

void ComputeShader::Execute()
{
	if (FAILED(UpdateData()))
	{
		HandleError(L"Failed CS UpdtaeData", 0);
	}

	Device::GetInst()->GetComCmdList()->SetPipelineState(m_pPLState.Get());

	Device::GetInst()->ComCommitTable();

	Device::GetInst()->GetComCmdList()->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);

	Device::GetInst()->FlushComputeCommandQueue();
}