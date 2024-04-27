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

	ComPtr<ID3D12PipelineState>				m_pPLState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC		m_pPLDesc;

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
	ID3D12PipelineState* GetPLState() { return m_pPLState.Get(); }
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
	void CreateShader(SHADER_TYPE _type, const wstring& _path, const string& _entry);
	// DX11 IA-> LAYOUT SET + SHADER별로 SET주던거 다 PIPELINESTATE로 정리
	void CreatePipeLineState();
	void SetShader(SHADER_TYPE _type);
};

