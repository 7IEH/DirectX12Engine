#include "pch.h"
#include "CGraphicShader.h"

#include "CPathMgr.h"
#include "CDevice.h"

GraphicShader::GraphicShader()
	: Shader(ASSET_TYPE::GRAPHIC_SHADER)
	, m_Topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_CullType(CULL_TYPE::BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BlendType(BLEND_TYPE::DEFAULT)
	, m_SamplerType(SAMPLER_TYPE::POINT)
	, m_Domain(SHADER_DOMAIN::DOMAIN_MASKED)
	, m_pPLDesc{}
{
}

GraphicShader::~GraphicShader()
{
}

// FX File Create
void GraphicShader::Default_Create(const wstring& _shaderPath, const string& _vsEntry, const string& _psEntry)
{
	CreateShader(SHADER_TYPE::VERTEX, _shaderPath, _vsEntry);
	CreateShader(SHADER_TYPE::PIXEL, _shaderPath, _psEntry);

	CreatePipeLineState();
}

// HLSL File Create
void GraphicShader::Default_Create(const wstring& _pVSShaderPath, const wstring& _pPSShaderPath, const string& _vsEntry, const string& _psEntry)
{
	CreateShader(SHADER_TYPE::VERTEX, _pVSShaderPath, _vsEntry);
	CreateShader(SHADER_TYPE::PIXEL, _pPSShaderPath, _psEntry);

	CreatePipeLineState();
}

void GraphicShader::Custom_Create(const wstring& _shaderPath, const string& _gsEntry, const string& _huEntry, const string& _dmEntry)
{
	if ("" == _gsEntry)
	{
		CreateShader(SHADER_TYPE::GEOMETRY, _shaderPath, _gsEntry);
	}

	if ("" == _huEntry)
	{
		CreateShader(SHADER_TYPE::HULL, _shaderPath, _huEntry);
	}

	if ("" == _dmEntry)
	{
		CreateShader(SHADER_TYPE::DOMAlN, _shaderPath, _dmEntry);
	}
}

HRESULT GraphicShader::UpdateData()
{
	CMDLIST->SetPipelineState(m_pPLState.Get());
	CMDLIST->IASetPrimitiveTopology(m_Topology);

	// Rasterizer Stage
	//CONTEXT->RSSetState(Device::GetInst()->GetRSState(m_CullType).Get());

	// Output-Merger Stage
	//CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(m_DSType).Get(), 0);
	//CONTEXT->OMSetBlendState(Device::GetInst()->GetBSState(m_BlendType).Get(), nullptr, 0xffffffff);

	return S_OK;
}

void GraphicShader::CreateShader(SHADER_TYPE _type, const wstring& _path, const string& _entry)
{
	UINT _iCompileFlag = 0;
#ifdef _DEBUG
	_iCompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	wstring _shaderPath = PATH + _path;

	switch (_type)
	{
	case SHADER_TYPE::VERTEX:
	{
		D3DCompileFromFile(_shaderPath.c_str()
			, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _entry.c_str(), "vs_5_0", _iCompileFlag, 0
			, m_VBlob.GetAddressOf(), m_pErrorBlob.GetAddressOf());
	}
	break;
	case SHADER_TYPE::PIXEL:
	{
		D3DCompileFromFile(_shaderPath.c_str()
			, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _entry.c_str(), "ps_5_0", _iCompileFlag, 0
			, m_PBlob.GetAddressOf(), m_pErrorBlob.GetAddressOf());
	}
	break;
	case SHADER_TYPE::HULL:
	{
	}
	break;
	case SHADER_TYPE::DOMAlN:
	{
	}
	break;
	case SHADER_TYPE::GEOMETRY:
	{
		D3DCompileFromFile(_shaderPath.c_str()
			, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _entry.c_str(), "gs_5_0", D3DCOMPILE_DEBUG, 0
			, m_GBlob.GetAddressOf(), m_pErrorBlob.GetAddressOf());
	}
	break;
	case SHADER_TYPE::END:
		break;
	default:
		break;
	}

	if (nullptr != m_pErrorBlob)
	{
		char* pErrMsg = (char*)m_pErrorBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
	}

	SetShader(_type);
}

void GraphicShader::SetShader(SHADER_TYPE _type)
{
	D3D12_SHADER_BYTECODE _tDesc = {};
	switch (_type)
	{
	case SHADER_TYPE::VERTEX:
	{
		_tDesc.pShaderBytecode = m_VBlob->GetBufferPointer();
		_tDesc.BytecodeLength = m_VBlob->GetBufferSize();
		m_pPLDesc.VS = _tDesc;
	}
	break;
	case SHADER_TYPE::PIXEL:
	{
		_tDesc.pShaderBytecode = m_PBlob->GetBufferPointer();
		_tDesc.BytecodeLength = m_PBlob->GetBufferSize();
		m_pPLDesc.PS = _tDesc;
	}
	break;
	case SHADER_TYPE::HULL:
		break;
	case SHADER_TYPE::DOMAlN:
		break;
	case SHADER_TYPE::GEOMETRY:
	{
		_tDesc.pShaderBytecode = m_GBlob->GetBufferPointer();
		_tDesc.BytecodeLength = m_GBlob->GetBufferSize();
		m_pPLDesc.GS = _tDesc;
	}
	break;
	case SHADER_TYPE::END:
		break;
	default:
		break;
	}
}

void GraphicShader::CreatePipeLineState()
{
	D3D12_INPUT_ELEMENT_DESC _arrElement[6] = {};

	_arrElement[0].SemanticName = "POSITION";
	_arrElement[0].SemanticIndex = 0;
	_arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	_arrElement[0].InputSlot = 0;
	_arrElement[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	_arrElement[0].InstanceDataStepRate = 0;
	_arrElement[0].AlignedByteOffset = 0;

	_arrElement[1].SemanticName = "COLOR";
	_arrElement[1].SemanticIndex = 0;
	_arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	_arrElement[1].InputSlot = 0;
	_arrElement[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	_arrElement[1].InstanceDataStepRate = 0;
	_arrElement[1].AlignedByteOffset = 12;

	_arrElement[2].SemanticName = "TEXCOORD";
	_arrElement[2].SemanticIndex = 0;
	_arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	_arrElement[2].InputSlot = 0;
	_arrElement[2].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	_arrElement[2].InstanceDataStepRate = 0;
	_arrElement[2].AlignedByteOffset = 28;

	_arrElement[3].SemanticName = "TANGENT";
	_arrElement[3].SemanticIndex = 0;
	_arrElement[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	_arrElement[3].InputSlot = 0;
	_arrElement[3].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	_arrElement[3].InstanceDataStepRate = 0;
	_arrElement[3].AlignedByteOffset = 36;

	_arrElement[4].SemanticName = "NORMAL";
	_arrElement[4].SemanticIndex = 0;
	_arrElement[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	_arrElement[4].InputSlot = 0;
	_arrElement[4].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	_arrElement[4].InstanceDataStepRate = 0;
	_arrElement[4].AlignedByteOffset = 48;

	_arrElement[5].SemanticName = "BINORMAL";
	_arrElement[5].SemanticIndex = 0;
	_arrElement[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	_arrElement[5].InputSlot = 0;
	_arrElement[5].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	_arrElement[5].InstanceDataStepRate = 0;
	_arrElement[5].AlignedByteOffset = 60;

	m_pPLDesc.InputLayout = { _arrElement,_countof(_arrElement) };
	m_pPLDesc.pRootSignature = Device::GetInst()->GetSignature();
	
	m_pPLDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_pPLDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_pPLDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_pPLDesc.SampleMask = UINT_MAX;
	m_pPLDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_pPLDesc.NumRenderTargets = 1;
	m_pPLDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pPLDesc.SampleDesc.Count = 1;
	m_pPLDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	switch (m_DSType)
	{
	case DS_TYPE::LESS:
	{
		m_pPLDesc.DepthStencilState.DepthEnable = TRUE;
		m_pPLDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	}
		break;
	case DS_TYPE::LESS_EQUAL:
	{
		m_pPLDesc.DepthStencilState.DepthEnable = TRUE;
		m_pPLDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}
		break;
	case DS_TYPE::GREATER:
	{
		m_pPLDesc.DepthStencilState.DepthEnable = TRUE;
		m_pPLDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
	}
		break;
	case DS_TYPE::GREATER_EQUAL:
	{
		m_pPLDesc.DepthStencilState.DepthEnable = TRUE;
		m_pPLDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
	}
		break;
	case DS_TYPE::NO_TEST:
	{
		m_pPLDesc.DepthStencilState.DepthEnable = FALSE;
		m_pPLDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	}
		break;
	case DS_TYPE::N0_WRITE:
	{
		m_pPLDesc.DepthStencilState.DepthEnable = FALSE;
		m_pPLDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		m_pPLDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}
		break;
	case DS_TYPE::NO_TEST_NO_WRITE:
	{
		m_pPLDesc.DepthStencilState.DepthEnable = FALSE;
		m_pPLDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
		m_pPLDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}
		break;
	case DS_TYPE::END:
		break;
	default:
		break;
	}

	switch (m_BlendType)
	{
	case BLEND_TYPE::ALPHABLENDING:
	{
		m_pPLDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
		m_pPLDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		m_pPLDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		m_pPLDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	}
		break;
	case BLEND_TYPE::ONE_ONE:
	{
		m_pPLDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
		m_pPLDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		m_pPLDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	}
		break;
	case BLEND_TYPE::END:
		break;
	default:
		break;
	}


	switch (m_CullType)
	{
	case CULL_TYPE::BACK:
	{
		m_pPLDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_pPLDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	}
		break;
	case CULL_TYPE::FRONT:
	{
		m_pPLDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_pPLDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
	}
		break;
	case CULL_TYPE::NONE:
	{
		m_pPLDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_pPLDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}
		break;
	case CULL_TYPE::WIRE:
	{
		m_pPLDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		m_pPLDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}
		break;
	case CULL_TYPE::END:
		break;
	default:
		break;
	}
	
	if (FAILED(DEVICE->CreateGraphicsPipelineState(&m_pPLDesc, IID_PPV_ARGS(&m_pPLState))))
	{
		HandleError(L"PipeLineState Create Failed", 0);
	}
}