#include "pch.h"
#include "CMaterial.h"

#include "CTexture.h"
#include "CDevice.h"

#include "CConstantBuffer.h"

Material::Material()
	:Asset(ASSET_TYPE::MATERIAL)
	, m_tMaterial{}
{
	m_tMaterial.vLightMat.Color = Vec4(1.f, 1.f, 1.f, 1.f);
}

Material::~Material()
{
}

void Material::SetTexParam(TEX_PARAM _Param, Ptr<Texture> _texture)
{
	m_pTexture[_Param] = _texture;
}

void Material::UpdateData()
{
	if (GetGraphicShader().Get() == nullptr)
	{
		HandleError(L"MeshRenderShader Shader is Nullptr Error!", 2);
		return;
	}
	GetGraphicShader()->UpdateData();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr != m_pTexture[i].Get())
		{
			m_pTexture[i]->UpdateData(i);
			m_tMaterial.Sprite[i] = 1;
		}
		else
		{
			Texture::Clear(i);
			m_tMaterial.Sprite[i] = 0;
		}
	}

	//Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->SetData(&m_tMaterial, sizeof(MaterialInfo), 1);
	//Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->UpdateData();
}

void Material::Render()
{
	UpdateData();
}