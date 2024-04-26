#pragma once
#include "CShader.h"
class GraphicShader :
    public Shader
{
private:
	ComPtr<ID3DBlob>                        m_VBlob;
	ComPtr<ID3DBlob>                        m_PBlob;
	ComPtr<ID3DBlob>                        m_HBlob;
	ComPtr<ID3DBlob>                        m_DBlob;
	ComPtr<ID3DBlob>                        m_GBlob;

	//ComPtr<ID3D11VertexShader>              m_VS;
	//ComPtr<ID3D11PixelShader>               m_PS;
	//ComPtr<ID3D11HullShader>                m_HS;
	//ComPtr<ID3D11DomainShader>              m_DS;
	//ComPtr<ID3D11GeometryShader>            m_GS;

	//ComPtr<ID3D11InputLayout>               m_LayOut;
	D3D_PRIMITIVE_TOPOLOGY                  m_Topology;

	CULL_TYPE                               m_CullType;
	DS_TYPE                                 m_DSType;
	BLEND_TYPE                              m_BlendType;
	SAMPLER_TYPE                            m_SamplerType;

	SHADER_DOMAIN                           m_Domain;

public:
	CLONE(GraphicShader)
	GraphicShader();
	virtual ~GraphicShader();

public:
	void SetTopology(D3D_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
	void SetCullType(CULL_TYPE _type) { m_CullType = _type; }
	void SetDSType(DS_TYPE _type) { m_DSType = _type; }
	void SetBlendType(BLEND_TYPE _type) { m_BlendType = _type; }

	void SetDomain(SHADER_DOMAIN _domain) { m_Domain = _domain; }

	SHADER_DOMAIN GetDomain() { return m_Domain; }

public:
	void Default_Create(const wstring& _shaderPath, const string& _vsEntry, const string& _psEntry);
	void Default_Create(const wstring& _pVSShaderPath, const wstring& _pPSShaderPath, const string& _vsEntry, const string& _psEntry);
	void Custom_Create(const wstring& _shaderPath, const string& _gsEntry, const string& _huEntry, const string& _dmEntry);
	virtual HRESULT UpdateData()override;

private:
	void CreateBlobFile(SHADER_TYPE _type, const wstring& _path, const string& _entry);
	void CreateShader(SHADER_TYPE _type);
	void CreateLayOut();
	void SetShader(SHADER_TYPE _type);
};

