#pragma once
#include "CAsset.h"

#include "CTexture.h"
#include "CGraphicShader.h"

class Material
	:public Asset
{
private:
	MaterialInfo							m_tMaterial;
	Ptr<GraphicShader>					m_GraphicShader;

	Ptr<Texture>						m_pTexture[(UINT)TEX_PARAM::END];

public:
	virtual void UpdateData();
	virtual void Render();

public:
	Ptr<GraphicShader> GetGraphicShader() { return m_GraphicShader; }
	void SetGraphicShader(Ptr<GraphicShader> _graphicShader) { m_GraphicShader = _graphicShader; }

	template<typename T>
	void SetMaterialParam(SCALAR_PARAM _ParamType, const T& _value);

	void SetTexParam(TEX_PARAM _Param, Ptr<Texture> _texture);

	void operator = (const Material& _OtherMtrl)
	{
		SetName(_OtherMtrl.GetName());

		m_tMaterial = _OtherMtrl.m_tMaterial;

		for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
		{
			m_pTexture[i] = _OtherMtrl.m_pTexture[i];
		}

		m_GraphicShader = _OtherMtrl.m_GraphicShader;
	}

	CLONE(Material)
public:
	Material();
	virtual ~Material();
};

template<typename T>
void Material::SetMaterialParam(SCALAR_PARAM _paramType, const T& _value)
{
	const T* pValue = &_value;

	switch (_paramType)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		m_tMaterial.iArr[_paramType] = *((int*)pValue);
		break;

	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_tMaterial.fArr[_paramType - FLOAT_0] = *((float*)pValue);
		break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_tMaterial.v2Arr[_paramType - VEC2_0] = *((Vec2*)pValue);
		break;

	case AMBIENT:
		m_tMaterial.vLightMat.Ambient = *((Vec4*)pValue);
		break;
	case DIFFUSE:
		m_tMaterial.vLightMat.Diffuse = *((Vec4*)pValue);
		break;
	case SPECULAR:
		m_tMaterial.vLightMat.Specular = *((Vec4*)pValue);
		break;
	case REFLECT:
		m_tMaterial.vLightMat.Reflect = *((Vec4*)pValue);
		break;
	case COLOR:
		m_tMaterial.vLightMat.Color = *((Vec4*)pValue);
		break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_tMaterial.MatArr[_paramType - MAT_0] = *((Matrix*)pValue);
		break;
	}
}