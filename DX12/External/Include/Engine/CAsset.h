#pragma once

#include "CEntity.h"
#include "Ptr.h"

class Asset :
    public Entity
{
private:
    wstring         m_Key;
    wstring         m_RelativeFilePath;
    ASSET_TYPE      m_Type;

    int             m_RefCount;

public:
    ASSET_TYPE GetType() { return m_Type; }
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativeFilePath; }

private:
    void SetKey(const wstring& _key) { m_Key = _key; }
    void SetRelativePath(const wstring& _relativeFilePath) { m_RelativeFilePath = _relativeFilePath; }

    void AddRef()
    {
        ++m_RefCount;
    }

    void Release()
    {
        --m_RefCount;
        if (0 >= m_RefCount)
        {
            delete this;
        }
    }

    int GetRefCount() { return m_RefCount; }

private:
    virtual HRESULT Load(const wstring& _strFilePath) { return E_FAIL; };

public:
    Asset(ASSET_TYPE _type);
    virtual ~Asset();

    virtual Asset* Clone() = 0;

    friend class AssetMgr;

    template<typename T>
    friend class Ptr;
};