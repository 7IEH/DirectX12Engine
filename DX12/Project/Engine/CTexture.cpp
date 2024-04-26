#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"
#include "CPathMgr.h"

Texture::Texture()
	:Asset(ASSET_TYPE::TEXTURE)
{
}

Texture::~Texture()
{
}

HRESULT Texture::Load(const wstring& _strFilePath)
{
	return S_OK;
}

void Texture::UpdateData(int _resgisterNumber)
{

}

HRESULT Texture::UpdateData_CS_SRV(int _registerNumber)
{

	return S_OK;
}

HRESULT Texture::UpdateData_CS_UAV(int _registerNumber)
{

	return S_OK;
}

void Texture::Clear(int _resgisterNumber)
{
	
}

void Texture::Clear_CS_SRV()
{

}

void Texture::Clear_CS_UAV()
{

}

//HRESULT Texture::Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindflags, D3D11_USAGE _usage)
//{
//
//	return S_OK;
//}
//
//HRESULT Texture::Create(ComPtr<ID3D11Texture2D> _texture2D)
//{
//	
//
//	return S_OK;
//}