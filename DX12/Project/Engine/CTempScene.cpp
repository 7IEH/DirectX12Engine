#include "pch.h"
#include "CTempScene.h"

#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CMeshRenderer.h"
#include "CCamera.h"

#include "CKeyMgr.h"

TempScene::TempScene()
{
}

TempScene::~TempScene()
{
}

void TempScene::Awake()
{
	// Camera
	GameObject* _pCamera = new GameObject;
	_pCamera->SetName(L"Camera");
	Camera* _pMainCam = _pCamera->AddComponent<Camera>();
	Transform* _pCamTr = _pCamera->AddComponent<Transform>();
	_pCamTr->SetRelativePosition(Vec3(0.f, 0.f, -500.f));

	_pMainCam->AllVisibleSet(TRUE);
	_pMainCam->SetCameraType(CAMERA_TYPE::MAIN_CAMERA);
	AddObject(_pCamera, LAYER_TYPE::CAMERA);

	// Player
	GameObject* _pCube = new GameObject;
	_pCube->SetName(L"CUBE");
	Transform* _pTr = _pCube->AddComponent<Transform>();
	_pTr->SetRelativePosition(Vec3(500.f, 0.f, 0.f));
	_pTr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	_pTr->SetRelativeScale(Vec3(500.f, 500.f, 500.f));

	MeshRenderer* _pRenderer = _pCube->AddComponent<MeshRenderer>();
	_pRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"CubeMesh"));
	_pRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Default3DMat"));
	AssetMgr::GetInst()->FindAsset<Material>(L"Default3DMat")->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Texture>(L"3DTile_01"));
	AssetMgr::GetInst()->FindAsset<Material>(L"Default3DMat")->SetTexParam(TEX_1, AssetMgr::GetInst()->FindAsset<Texture>(L"3DTileN_01"));

	AddObject(_pCube, LAYER_TYPE::PLAYER);

	// FBX
	MeshData* _meshData = AssetMgr::GetInst()->FindAsset<MeshData>(L"HarunaMeshData").Get();

	vector<GameObject*> _vObjs = _meshData->Instantiate();

	for (size_t i = 0;i < _vObjs.size();i++)
	{
		_vObjs[i]->SetName(L"Haruna");
		Transform* _pTr = _vObjs[i]->GetComponent<Transform>();
		_pTr->SetRelativePosition(Vec3(-500.f, 0.f, 0.f));
		_pTr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		_pTr->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
		AddObject(_vObjs[i], LAYER_TYPE::PLAYER);
	}	

	// SkyBox
	GameObject* _pSkyBox = new GameObject;
	_pSkyBox->SetName(L"SkyBox");
	_pTr = _pSkyBox->AddComponent<Transform>();

	SkyBox* _pSkyBoxRender = _pSkyBox->AddComponent<SkyBox>();
	_pSkyBoxRender->SetType(SkyBoxType::Sphere);
	_pSkyBoxRender->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"SkyBoxMat"));
	_pSkyBoxRender->SetSphereTex(AssetMgr::GetInst()->FindAsset<Texture>(L"SkyBox_01"));

	AddObject(_pSkyBox, LAYER_TYPE::PLAYER);
}

void TempScene::Update()
{
	Scene::Update();

	/*if (KEY_TAP(KEY::Q))
	{
		if (m_iNum == 0)
		{
			m_iNum = 1;
		}
		else
		{
			m_iNum = 0;
		}
	}

	m_pTestObject->GetComponent<MeshRenderer>()->GetMaterial()->SetMaterialParam(INT_0, m_iNum);
	m_pTestObject2->GetComponent<MeshRenderer>()->GetMaterial()->SetMaterialParam(INT_0, m_iNum);*/
}