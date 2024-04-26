#include "pch.h"
#include "CGraphicShader.h"

#include "CPathMgr.h"
#include "CDevice.h"

GraphicShader::GraphicShader()
	: Shader(ASSET_TYPE::GRAPHIC_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_CullType(CULL_TYPE::BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BlendType(BLEND_TYPE::DEFAULT)
	, m_SamplerType(SAMPLER_TYPE::POINT)
	, m_Domain(SHADER_DOMAIN::DOMAIN_MASKED)
{
}

GraphicShader::~GraphicShader()
{
}

// FX File Create
void GraphicShader::Default_Create(const wstring& _shaderPath, const string& _vsEntry, const string& _psEntry)
{
	CreateBlobFile(SHADER_TYPE::VERTEX, _shaderPath, _vsEntry);
	CreateBlobFile(SHADER_TYPE::PIXEL, _shaderPath, _psEntry);

	CreateShader(SHADER_TYPE::VERTEX);
	CreateShader(SHADER_TYPE::PIXEL);

	CreateLayOut();
}

// HLSL File Create
void GraphicShader::Default_Create(const wstring& _pVSShaderPath, const wstring& _pPSShaderPath, const string& _vsEntry, const string& _psEntry)
{
	CreateBlobFile(SHADER_TYPE::VERTEX, _pVSShaderPath, _vsEntry);
	CreateBlobFile(SHADER_TYPE::PIXEL, _pPSShaderPath, _psEntry);

	CreateShader(SHADER_TYPE::VERTEX);
	CreateShader(SHADER_TYPE::PIXEL);

	CreateLayOut();
}

void GraphicShader::Custom_Create(const wstring& _shaderPath, const string& _gsEntry, const string& _huEntry, const string& _dmEntry)
{
	if ("" == _gsEntry)
	{
		CreateBlobFile(SHADER_TYPE::GEOMETRY, _shaderPath, _gsEntry);
		CreateShader(SHADER_TYPE::GEOMETRY);
	}

	if ("" == _huEntry)
	{
		CreateBlobFile(SHADER_TYPE::HULL, _shaderPath, _huEntry);
		CreateShader(SHADER_TYPE::HULL);
	}

	if ("" == _dmEntry)
	{
		CreateBlobFile(SHADER_TYPE::DOMAlN, _shaderPath, _dmEntry);
		CreateShader(SHADER_TYPE::DOMAlN);
	}
}

HRESULT GraphicShader::UpdateData()
{
	//CONTEXT->IASetPrimitiveTopology(m_Topology);
	//CONTEXT->IASetInputLayout(m_LayOut.Get());

	SetShader(SHADER_TYPE::VERTEX);

	// Rasterizer Stage
	//CONTEXT->RSSetState(Device::GetInst()->GetRSState(m_CullType).Get());

	// Pixel Shader Stage
	SetShader(SHADER_TYPE::PIXEL);

	// DOMAIN Shader Stage
	SetShader(SHADER_TYPE::DOMAlN);

	// HULL Shader Stage
	SetShader(SHADER_TYPE::HULL);

	// GeoMetry Shader Stage
	SetShader(SHADER_TYPE::GEOMETRY);

	// Output-Merger Stage
	//CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(m_DSType).Get(), 0);
	//CONTEXT->OMSetBlendState(Device::GetInst()->GetBSState(m_BlendType).Get(), nullptr, 0xffffffff);

	return S_OK;
}

void GraphicShader::CreateBlobFile(SHADER_TYPE _type, const wstring& _path, const string& _entry)
{
	wstring _shaderPath = PATH + _path;

	switch (_type)
	{
	case SHADER_TYPE::VERTEX:
	{
		D3DCompileFromFile(_shaderPath.c_str()
			, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _entry.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
			, m_VBlob.GetAddressOf(), m_pErrorBlob.GetAddressOf());
	}
		break;
	case SHADER_TYPE::PIXEL:
	{
		D3DCompileFromFile(_shaderPath.c_str()
			, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _entry.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
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
}

void GraphicShader::CreateShader(SHADER_TYPE _type)
{
	/*switch (_type)
	{
	case SHADER_TYPE::VERTEX:
		DEVICE->CreateVertexShader(m_VBlob->GetBufferPointer(),
			m_VBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());
		break;
	case SHADER_TYPE::PIXEL:
		DEVICE->CreatePixelShader(m_PBlob->GetBufferPointer(),
			m_PBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());
		break;
	case SHADER_TYPE::HULL:
		DEVICE->CreateHullShader(m_HBlob->GetBufferPointer(),
			m_HBlob->GetBufferSize(), nullptr, m_HS.GetAddressOf());
		break;
	case SHADER_TYPE::DOMAlN:
		DEVICE->CreateDomainShader(m_DBlob->GetBufferPointer(),
			m_DBlob->GetBufferSize(), nullptr, m_DS.GetAddressOf());
		break;
	case SHADER_TYPE::GEOMETRY:
		DEVICE->CreateGeometryShader(m_GBlob->GetBufferPointer(),
			m_GBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf());
		break;
	case SHADER_TYPE::END:
		break;
	default:
		break;
	}*/
}

void GraphicShader::CreateLayOut()
{
	//D3D11_INPUT_ELEMENT_DESC arrElement[6] = {};

	//arrElement[0].InputSlot = 0;
	//arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//arrElement[0].SemanticName = "POSITION";
	//arrElement[0].SemanticIndex = 0;
	//arrElement[0].InstanceDataStepRate = 0;
	//arrElement[0].AlignedByteOffset = 0;
	//arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	//arrElement[1].InputSlot = 0;
	//arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//arrElement[1].SemanticName = "COLOR";
	//arrElement[1].SemanticIndex = 0;
	//arrElement[1].InstanceDataStepRate = 0;
	//arrElement[1].AlignedByteOffset = 12;
	//arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//arrElement[2].InputSlot = 0;
	//arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//arrElement[2].SemanticName = "TEXCOORD";
	//arrElement[2].SemanticIndex = 0;
	//arrElement[2].InstanceDataStepRate = 0;
	//arrElement[2].AlignedByteOffset = 28;
	//arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	//arrElement[3].InputSlot = 0;
	//arrElement[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//arrElement[3].SemanticName = "TANGENT";
	//arrElement[3].SemanticIndex = 0;
	//arrElement[3].InstanceDataStepRate = 0;
	//arrElement[3].AlignedByteOffset = 36;
	//arrElement[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	//arrElement[4].InputSlot = 0;
	//arrElement[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//arrElement[4].SemanticName = "NORMAL";
	//arrElement[4].SemanticIndex = 0;
	//arrElement[4].InstanceDataStepRate = 0;
	//arrElement[4].AlignedByteOffset = 48;
	//arrElement[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	//arrElement[5].InputSlot = 0;
	//arrElement[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//arrElement[5].SemanticName = "BINORMAL";
	//arrElement[5].SemanticIndex = 0;
	//arrElement[5].InstanceDataStepRate = 0;
	//arrElement[5].AlignedByteOffset = 60;
	//arrElement[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	//DEVICE->CreateInputLayout(arrElement, 6, m_VBlob->GetBufferPointer(), m_VBlob->GetBufferSize(), m_LayOut.GetAddressOf());
}

void GraphicShader::SetShader(SHADER_TYPE _type)
{
	/*switch (_type)
	{
	case SHADER_TYPE::VERTEX:
	{
		CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	}
	break;
	case SHADER_TYPE::PIXEL:
	{
		CONTEXT->PSSetShader(m_PS.Get(), 0, 0);
	}
	break;
	case SHADER_TYPE::HULL:
	{
		CONTEXT->HSSetShader(m_HS.Get(), 0, 0);
	}
	break;
	case SHADER_TYPE::DOMAlN:
	{
		CONTEXT->DSSetShader(m_DS.Get(), 0, 0);
	}
	break;
	case SHADER_TYPE::GEOMETRY:
	{
		CONTEXT->GSSetShader(m_GS.Get(), 0, 0);
	}
	break;
	case SHADER_TYPE::END:
		break;
	default:
		break;
	}*/
}