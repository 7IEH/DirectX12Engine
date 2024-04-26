#include "pch.h"
#include "CAsset.h"

Asset::Asset(ASSET_TYPE _type)
	:m_Type(_type)
	, m_RefCount(0)
{
}

Asset::~Asset()
{
}