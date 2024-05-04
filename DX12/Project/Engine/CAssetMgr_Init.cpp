#include "pch.h"
#include "CAssetMgr.h"

#include "CGraphicShader.h"
#include "CMesh.h"
#include "CMeshData.h"
#include "CMaterial.h"
#include "CFBXLoader.h"

AssetMgr::AssetMgr()
	:m_mAssets{}
{

}

AssetMgr::~AssetMgr()
{
}

void AssetMgr::Awake()
{
	CreateSound();
	CreateTexture();
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();
	CreateDefaultMeshData();
}

void AssetMgr::CreateTexture()
{
	Texture* _pTex = new Texture;
	_pTex->Load(L"\\Asset\\Resource\\3DTile\\TILE_01.tga");
	AddAsset(_pTex, L"3DTile_01");

	_pTex = new Texture;
	_pTex->Load(L"\\Asset\\Resource\\3DTile\\TILE_01_N.tga");
	AddAsset(_pTex, L"3DTileN_01");

	_pTex = new Texture;
	_pTex->Load(L"\\Asset\\Resource\\SkyBox\\Sky02.jpg");
	AddAsset(_pTex, L"SkyBox_01");

	_pTex = new Texture;
	_pTex->Load(L"\\Asset\\Resource\\FBXmodel\\Haruna_Original_Body.png");
	AddAsset(_pTex, L"HarunaBodyTex");

	_pTex = new Texture;
	_pTex->Load(L"\\Asset\\Resource\\FBXmodel\\Haruna_Original_Face.png");
	AddAsset(_pTex, L"HarunaFaceTex");

	_pTex = new Texture;
	_pTex->Load(L"\\Asset\\Resource\\FBXmodel\\Haruna_Original_EyeMouth.png");
	AddAsset(_pTex, L"HarunaEyeMouthTex");

	_pTex = new Texture;
	_pTex->Load(L"\\Asset\\Resource\\FBXmodel\\Haruna_Original_Hair.png");
	AddAsset(_pTex, L"HarunaHariTex");
}

void AssetMgr::CreateDefaultMesh()
{
	Mesh* _pMesh = nullptr;
	vector<VertexInfo> _vVtx = {};
	vector<UINT> _vIdx = {};

	VertexInfo _vTemp = {};
	/********************
	| Rectangle Mesh
	********************/
	_pMesh = new Mesh;

	_vTemp.Position = Vec3(-0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vVtx.push_back(_vTemp);

	// ÀÎµ¦½º
	_vIdx.push_back(0);
	_vIdx.push_back(1);
	_vIdx.push_back(2);

	_vIdx.push_back(0);
	_vIdx.push_back(2);
	_vIdx.push_back(3);

	_pMesh->Create(_vVtx, (UINT)_vVtx.size(), _vIdx, (UINT)_vIdx.size());
	AddAsset(_pMesh, L"RectMesh");

	_vTemp = {};
	_vVtx.clear();
	_vIdx.clear();

	///********************
	//| Circle Mesh
	//********************/

	///********************
	//| Cube Mesh
	//********************/
	_pMesh = new Mesh;

	// Front
	_vTemp.Position = Vec3(-0.5f, 0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vTemp.Normal = Vec3(0.f, 0.f, -1.f);
	_vTemp.Tangent = Vec3(0.f, 1.f, 0.f);
	_vTemp.Binormal = Vec3(1.f, 0.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, 0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, -0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(1.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, -0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 1.f);
	_vVtx.push_back(_vTemp);

	// Back
	_vTemp.Position = Vec3(-0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vTemp.Normal = Vec3(0.f, 0.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(1.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 1.f);
	_vVtx.push_back(_vTemp);

	// Right
	_vTemp.Position = Vec3(0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vTemp.Normal = Vec3(1.f, 0.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, 0.5f, -0.5f);
	_vTemp.Color = Vec4(0.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, -0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 1.f);
	_vVtx.push_back(_vTemp);

	// Left
	_vTemp.Position = Vec3(-0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vTemp.Normal = Vec3(-1.f, 0.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, 0.5f, -0.5f);
	_vTemp.Color = Vec4(0.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, -0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 1.f);
	_vVtx.push_back(_vTemp);

	// Up
	_vTemp.Position = Vec3(0.5f, 0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vTemp.Normal = Vec3(0.f, 1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, 0.5f, -0.5f);
	_vTemp.Color = Vec4(0.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, 0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 1.f);
	_vVtx.push_back(_vTemp);

	// Down
	_vTemp.Position = Vec3(0.5f, -0.5f, -0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 0.f);
	_vTemp.Normal = Vec3(0.f, -1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, -0.5f, -0.5f);
	_vTemp.Color = Vec4(0.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 0.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(-0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(1.f, 0.f, 1.f, 1.f);
	_vTemp.UV = Vec2(1.f, 1.f);
	_vVtx.push_back(_vTemp);

	_vTemp.Position = Vec3(0.5f, -0.5f, 0.5f);
	_vTemp.Color = Vec4(0.f, 1.f, 0.f, 1.f);
	_vTemp.UV = Vec2(0.f, 1.f);
	_vVtx.push_back(_vTemp);

	// Front
	_vIdx.push_back(0);
	_vIdx.push_back(1);
	_vIdx.push_back(2);

	_vIdx.push_back(0);
	_vIdx.push_back(2);
	_vIdx.push_back(3);

	// Back
	_vIdx.push_back(6);
	_vIdx.push_back(5);
	_vIdx.push_back(4);

	_vIdx.push_back(7);
	_vIdx.push_back(6);
	_vIdx.push_back(4);

	// Right
	_vIdx.push_back(10);
	_vIdx.push_back(9);
	_vIdx.push_back(8);

	_vIdx.push_back(11);
	_vIdx.push_back(10);
	_vIdx.push_back(8);

	// Left
	_vIdx.push_back(12);
	_vIdx.push_back(13);
	_vIdx.push_back(14);

	_vIdx.push_back(12);
	_vIdx.push_back(14);
	_vIdx.push_back(15);

	// Up
	_vIdx.push_back(16);
	_vIdx.push_back(17);
	_vIdx.push_back(18);

	_vIdx.push_back(16);
	_vIdx.push_back(18);
	_vIdx.push_back(19);

	// Down
	_vIdx.push_back(22);
	_vIdx.push_back(21);
	_vIdx.push_back(20);

	_vIdx.push_back(23);
	_vIdx.push_back(22);
	_vIdx.push_back(20);

	_pMesh->Create(_vVtx, (UINT)_vVtx.size(), _vIdx, (UINT)_vIdx.size());
	AddAsset(_pMesh, L"CubeMesh");

	_vTemp = {};
	_vVtx.clear();
	_vIdx.clear();

	///********************
	//| Sphere Mesh
	//********************/
	float fRadius = 0.5f;

	// Top
	_vTemp.Position = Vec3(0.f, fRadius, 0.f);
	_vTemp.UV = Vec2(0.5f, 0.f);
	_vTemp.Color = Vec4(1.f, 1.f, 1.f, 1.f);
	_vTemp.Normal = _vTemp.Position;
	_vTemp.Normal.Normalize();
	_vTemp.Tangent = Vec3(1.f, 0.f, 0.f);
	_vTemp.Binormal = Vec3(0.f, 0.f, -1.f);
	_vVtx.push_back(_vTemp);

	// Body
	UINT iStackCount = 40; // °¡·Î ºÐÇÒ °³¼ö
	UINT iSliceCount = 40; // ¼¼·Î ºÐÇÒ °³¼ö

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			_vTemp.Position = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

			_vTemp.UV = Vec2(fUVXStep * j, fUVYStep * i);
			_vTemp.Color = Vec4(1.f, 1.f, 1.f, 1.f);
			_vTemp.Normal = _vTemp.Position;
			_vTemp.Normal.Normalize();

			_vTemp.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
			_vTemp.Tangent.y = 0.f;
			_vTemp.Tangent.z = fRadius * sinf(phi) * cosf(theta);
			_vTemp.Tangent.Normalize();

			_vTemp.Normal.Cross(_vTemp.Tangent, _vTemp.Binormal);
			_vTemp.Binormal.Normalize();

			_vVtx.push_back(_vTemp);
		}
	}

	// Bottom
	_vTemp.Position = Vec3(0.f, -fRadius, 0.f);
	_vTemp.UV = Vec2(0.5f, 1.f);
	_vTemp.Color = Vec4(1.f, 1.f, 1.f, 1.f);
	_vTemp.Normal = _vTemp.Position;
	_vTemp.Normal.Normalize();

	_vTemp.Tangent = Vec3(1.f, 0.f, 0.f);
	_vTemp.Binormal = Vec3(0.f, 0.f, -1.f);
	_vVtx.push_back(_vTemp);

	// ÀÎµ¦½º
	// ºÏ±ØÁ¡
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		_vIdx.push_back(0);
		_vIdx.push_back(i + 2);
		_vIdx.push_back(i + 1);
	}

	// ¸öÅë
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			_vIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			_vIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			_vIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			_vIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			_vIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			_vIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// ³²±ØÁ¡
	UINT iBottomIdx = (UINT)_vVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		_vIdx.push_back(iBottomIdx);
		_vIdx.push_back(iBottomIdx - (i + 2));
		_vIdx.push_back(iBottomIdx - (i + 1));
	}

	_pMesh = new Mesh;
	_pMesh->Create(_vVtx, (UINT)_vVtx.size(), _vIdx, (UINT)_vIdx.size());
	AddAsset(_pMesh, L"SphereMesh");
	_vVtx.clear();
	_vIdx.clear();
}

void AssetMgr::CreateDefaultShader()
{
	GraphicShader* _pShader = nullptr;
	wstring _vsPath = {};
	wstring _psPath = {};
	string _vsEntry = {};
	string _psEntry = {};

	/********************
	| Default 2D Shader
	********************/
	_pShader = new GraphicShader;

	_vsPath = L"\\shader\\DefaultVS2D.hlsl";
	_psPath = L"\\shader\\DefaultPS2D.hlsl";

	_vsEntry = "VS_Default2D";
	_psEntry = "PS_Default2D";

	_pShader->SetBlendType(BLEND_TYPE::ALPHABLENDING);

	_pShader->Default_Create(_vsPath, _psPath, _vsEntry, _psEntry);

	AddAsset(_pShader, L"Default2DShader");

	/********************
	| Default 3D Shader
	********************/
	_pShader = new GraphicShader;

	_vsPath = L"\\shader\\DefaultVS3D.hlsl";
	_psPath = L"\\shader\\DefaultPS3D.hlsl";

	_vsEntry = "VS_Default3D";
	_psEntry = "PS_Default3D";

	_pShader->SetBlendType(BLEND_TYPE::DEFAULT);
	_pShader->SetCullType(CULL_TYPE::BACK);

	_pShader->Default_Create(_vsPath, _psPath, _vsEntry, _psEntry);

	AddAsset(_pShader, L"Default3DShader");

	/********************
	| SkyBox Shader
	********************/
	_pShader = new GraphicShader;

	_vsPath = L"\\shader\\SkyBoxVS.hlsl";
	_psPath = L"\\shader\\SkyBoxPS.hlsl";

	_vsEntry = "VS_SkyBox";
	_psEntry = "PS_SkyBox";

	_pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	_pShader->SetCullType(CULL_TYPE::FRONT);

	_pShader->Default_Create(_vsPath, _psPath, _vsEntry, _psEntry);

	AddAsset(_pShader, L"SkyBoxShader");
}

void AssetMgr::CreateDefaultMaterial()
{
	Material* _mat = nullptr;

	/**********************
	| Default Shader2D Mat
	***********************/
	_mat = new Material;
	_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"Default2DShader"));
	AddAsset<Material>(_mat, L"Default2DMat");

	/**********************
	| Default Shader3D Mat
	***********************/
	_mat = new Material;
	_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"Default3DShader"));
	AddAsset<Material>(_mat, L"Default3DMat");

	/**********************
	| SkyBox Mat
	***********************/
	_mat = new Material;
	_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"SkyBoxShader"));
	AddAsset<Material>(_mat, L"SkyBoxMat");

	/**********************
	| Haruna Mat
	***********************/
	_mat = new Material;
	_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"Default3DShader"));
	_mat->SetTexParam(TEX_0, FindAsset<Texture>(L"HarunaBodyTex"));
	AddAsset<Material>(_mat, L"HarunaBodyMtrl");

	_mat = new Material;
	_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"Default3DShader"));
	_mat->SetTexParam(TEX_0, FindAsset<Texture>(L"HarunaFaceTex"));
	AddAsset<Material>(_mat, L"HarunaFaceMtrl");

	_mat = new Material;
	_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"Default3DShader"));
	_mat->SetTexParam(TEX_0, FindAsset<Texture>(L"HarunaEyeMouthTex"));
	AddAsset<Material>(_mat, L"HarunaEyeMouthMtrl");

	_mat = new Material;
	_mat->SetGraphicShader(AssetMgr::GetInst()->FindAsset<GraphicShader>(L"Default3DShader"));
	_mat->SetTexParam(TEX_0, FindAsset<Texture>(L"HarunaHariTex"));
	AddAsset<Material>(_mat, L"HarunaHairMtrl");
}

void AssetMgr::CreateDefaultComputeShader()
{
}

void AssetMgr::CreateSound()
{
}

void AssetMgr::CreateDefaultMeshData()
{
	wstring _path = L"\\Asset\\Resource\\FBXmodel\\Haruna_Original_Mesh.fbx";

	Ptr<MeshData> _meshData = nullptr;
	_meshData = Load<MeshData>(_path, L"HarunaMeshData");
}