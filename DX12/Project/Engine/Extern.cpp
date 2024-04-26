#include "pch.h"
#include "Extern.h"

#include "CEngine.h"

MatrixInfo e_MatrixData = {};

GlobalDataInfo e_Global = {};

wstring e_Font = {};

Vec2 e_vRatio = Vec2(Engine::GetInst()->GetRes().x / 1600.f, Engine::GetInst()->GetRes().y / 900.f);

wstring e_sNxtScene = {};

const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END]
{
	"MESH",
	"SHADER",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
	"MATERIAL",
	"TEXTURE",
	"SPRITE",
	"SOUND"
};