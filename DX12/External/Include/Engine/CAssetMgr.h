#pragma once

#include "CTexture.h"
#include "CMesh.h"
#include "CGraphicShader.h"
#include "CMaterial.h"

#include "CPathMgr.h"

#ifdef DX11
class AssetMgr
    :public Singleton<AssetMgr>
{
    Single(AssetMgr)
private:
    map<wstring, Ptr<Asset>>	m_mAssets[(UINT)ASSET_TYPE::END];

public:
    map<wstring, Ptr<Asset>> GetSprite() { return m_mAssets[(UINT)ASSET_TYPE::SPRITE]; }
    map<wstring, Ptr<Asset>> GetMesh() { return m_mAssets[(UINT)ASSET_TYPE::MESH]; }
    map<wstring, Ptr<Asset>> GetMaterial() { return m_mAssets[(UINT)ASSET_TYPE::MATERIAL]; }
    map<wstring, Ptr<Asset>> GetGraphicShader() { return m_mAssets[(UINT)ASSET_TYPE::GRAPHIC_SHADER]; }

public:
    template <typename T>
    void AddAsset(T* _asset, const wstring& _name);

    template<typename T>
    Ptr<T> FindAsset(const wstring& _name);

    template<typename T>
    Ptr<T> Load(const wstring& _strFilePath, const wstring& _name);

   // Ptr<Texture> CreateResoruceTexture(const wstring& _label, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _flag, D3D11_USAGE _usage = D3D11_USAGE_DEFAULT);
   // Ptr<Texture> CreateResoruceTexture(const wstring& _label, ComPtr<ID3D11Texture2D> _tex2D);

private:
    void CreateTexture();
    void CreateDefaultMesh();
    void CreateDefaultShader();
    void CreateDefaultMaterial();
    void CreateDefaultComputeShader();

    void CreateSound();

public:
    void Awake();
};

template<typename T>
ASSET_TYPE GetAssetType()
{
    ASSET_TYPE Type = ASSET_TYPE::END;

    if constexpr (std::is_same_v<Mesh, T>)
        Type = ASSET_TYPE::MESH;
    if constexpr (std::is_same_v<Texture, T>)
        Type = ASSET_TYPE::TEXTURE;
    if constexpr (std::is_same_v<GraphicShader, T>)
        Type = ASSET_TYPE::GRAPHIC_SHADER;
    if constexpr (std::is_same_v<Material, T>)
        Type = ASSET_TYPE::MATERIAL;

    return Type;
}

//template<typename T>
//inline const map<wstring, Ptr<Asset>>& AssetMgr::GetAssets()
//{
//    ASSET_TYPE Type = GetAssetType<T>();
//
//    return m_mapAsset[(UINT)Type];
//}

template<typename T>
inline void AssetMgr::AddAsset(T* _asset, const wstring& _name)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, Ptr<Asset>>::iterator iter = m_mAssets[(UINT)Type].find(_name);
    assert(iter == m_mAssets[(UINT)Type].end());

    _asset->SetName(_name);
    m_mAssets[(UINT)Type].insert(make_pair(_name, _asset));
}

template<typename T>
Ptr<T> AssetMgr::FindAsset(const wstring& _name)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, Ptr<Asset>>::iterator iter = m_mAssets[(UINT)Type].find(_name);

    if (iter == m_mAssets[(UINT)Type].end())
    {
        return nullptr;
    }

    return (T*)iter->second.Get();
}


template<typename T>
Ptr<T> AssetMgr::Load(const wstring& _strFilePath, const wstring& _name)
{
    Ptr<T> pAsset = FindAsset<T>(_name);

    // 로딩할 때 사용할 키로 이미 다른 에셋이 있다면
    if (nullptr != pAsset)
    {
        return (T*)pAsset.Get();
    }

    wstring strFilePath = PATH;
    strFilePath += _strFilePath;

    pAsset = new T;
    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 실패", MB_OK);
        pAsset = nullptr;
        return nullptr;
    }

    pAsset->SetName(_name);
    pAsset->SetKey(_name);
    pAsset->SetRelativePath(_strFilePath);

    AddAsset<T>((T*)pAsset.Get(), _name);

    return (T*)pAsset.Get();
}

//template<typename T>
//inline void AssetMgr::DeleteAsset(const wstring& _strKey)
//{
//    ASSET_TYPE AssetType = GetAssetType<T>();
//
//    map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)AssetType].find(_strKey);
//
//    assert(!(iter == m_mapAsset[(UINT)AssetType].end()));
//
//    m_mapAsset[(UINT)AssetType].erase(iter);
//}
#else

#endif 

