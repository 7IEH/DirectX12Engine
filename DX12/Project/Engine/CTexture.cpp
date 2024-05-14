#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"
#include "CPathMgr.h"

Texture::Texture()
	: Asset(ASSET_TYPE::TEXTURE)
	, m_SRVHandle{}
{
}

Texture::~Texture()
{
}

HRESULT Texture::Load(const wstring& _strFilePath)
{
	wstring _path = PATH + _strFilePath;
	wstring _ext = std::filesystem::path(_path).extension();

	if (_ext == L".dds" || _ext == L".DDS")
	{
		::LoadFromDDSFile(_path.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}
	else if (_ext == L".tga" || _ext == L".TGA")
	{
		::LoadFromTGAFile(_path.c_str(), nullptr, m_Image);
	}
	else
	{
		::LoadFromWICFile(_path.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	HRESULT _hr = ::CreateTexture(DEVICE, m_Image.GetMetadata(), m_pTexture.GetAddressOf());

	vector<D3D12_SUBRESOURCE_DATA> _subResource;

	_hr = ::PrepareUpload(DEVICE,
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		_subResource);

	if (FAILED(_hr))
	{
		HandleError(L"Texture Loading Failed", 0);
	}

	const UINT64 _iBufferSize = GetRequiredIntermediateSize(m_pTexture.Get(), 0, static_cast<UINT>(_subResource.size()));

	D3D12_HEAP_PROPERTIES _heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(_iBufferSize);

	ComPtr<ID3D12Resource> _pTextureUploadHeap;

	_hr = DEVICE->CreateCommittedResource(
		&_heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&_tDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(_pTextureUploadHeap.GetAddressOf()));

	UpdateSubresources(Device::GetInst()->GetResCmdList(),
		m_pTexture.Get(),
		_pTextureUploadHeap.Get(),
		0, 0,
		static_cast<unsigned int>(_subResource.size()),
		_subResource.data());

	m_pTextureUploadHeap = _pTextureUploadHeap;
	Device::GetInst()->FlushResourceCommandQueue();

	CreateView();
	return _hr;
}

void Texture::UpdateData(int _resgisterNumber)
{
	Device::GetInst()->SetSRV(m_SRVHandle, _resgisterNumber);
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

void Texture::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC _tDesc = {};
	_tDesc.NumDescriptors = 1;
	_tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	_tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap(&_tDesc, IID_PPV_ARGS(&m_pSRV));

	m_SRVHandle = m_pSRV->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC _SRVDesc = {};
	_SRVDesc.Format = m_Image.GetMetadata().format;
	_SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	_SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	_SRVDesc.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView(m_pTexture.Get(), &_SRVDesc, m_SRVHandle);

}