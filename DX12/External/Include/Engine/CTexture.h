#pragma once
#include "CAsset.h"
class Texture :
    public Asset
{
private:
	ScratchImage									m_Image;
	ComPtr<ID3D12Resource>							m_pTexture;

	ComPtr<ID3D12DescriptorHeap>					m_pSRV;
	D3D12_CPU_DESCRIPTOR_HANDLE						m_SRVHandle;

	ComPtr<ID3D12Resource>							m_pTextureUploadHeap;

public:
	/*UINT									GetSpriteWidth() { return m_DESC.Width; }
	UINT									GetSpriteHeight() { return m_DESC.Height; }

	ComPtr<ID3D11RenderTargetView>			GetRTV() { return m_RenderTargetView; }
	ComPtr<ID3D11DepthStencilView>			GetDSV() { return m_DepthStencilView; }
	ComPtr<ID3D11ShaderResourceView>		GetSRV() { return m_ShaderResourceView; }
	ComPtr<ID3D11UnorderedAccessView>		GetUAV() { return m_UnorderedAccessView; }

	ID3D11Texture2D* GetTexture2D() { return m_pTexture.Get(); }
	ScratchImage* GetScratchImage() { return &m_Image; }*/

public:
	void UpdateData(int _resgisterNumber);

	HRESULT UpdateData_CS_SRV(int _registerNumber);
	HRESULT UpdateData_CS_UAV(int _registerNumber);

	static void Clear(int _resgisterNumber);
	void Clear_CS_SRV();
	void Clear_CS_UAV();

private:
	virtual HRESULT Load(const wstring& _strFilePath)override;
	/*HRESULT Create(UINT _width, UINT _height,
		DXGI_FORMAT _format, UINT _bindflags,
		D3D11_USAGE _usage = D3D11_USAGE_DEFAULT);

	HRESULT Create(ComPtr<ID3D11Texture2D> _texture2D);*/

	void CreateView();

public:
	CLONE_DISABLE(Texture)
	Texture();
	virtual ~Texture();

	friend class AssetMgr;
};

