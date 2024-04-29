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
	_pCamTr->SetRelativePosition(Vec3(0.f, 0.f, 0.f));

	_pMainCam->AllVisibleSet(TRUE);
	_pMainCam->SetCameraType(CAMERA_TYPE::MAIN_CAMERA);
	AddObject(_pCamera, LAYER_TYPE::CAMERA);

	// Player
	GameObject* _pCube = new GameObject;
	_pCube->SetName(L"CUBE");
	Transform* _pTr = _pCube->AddComponent<Transform>();
	_pTr->SetRelativePosition(Vec3(1.f, 0.f, 0.f));
	_pTr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	MeshRenderer* _pRenderer = _pCube->AddComponent<MeshRenderer>();
	_pRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"RectMesh"));
	_pRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Default2DMat"));
	//AssetMgr::GetInst()->FindAsset<Material>(L"Default3DMat")->SetTexParam(TEX_0, AssetMgr::GetInst()->FindAsset<Texture>(L"3DTile_01"));
	//AssetMgr::GetInst()->FindAsset<Material>(L"Default3DMat")->SetTexParam(TEX_1, AssetMgr::GetInst()->FindAsset<Texture>(L"3DTileN_01"));

	AddObject(_pCube, LAYER_TYPE::PLAYER);

	// Sphere
	//GameObject* _pSphere = new GameObject;
	//_pTr = _pSphere->AddComponent<Transform>();
	//_pTr->SetRelativePosition(Vec3(-1.f, 0.f, 0.f));
	//_pTr->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	//_pRenderer = _pSphere->AddComponent<MeshRenderer>();
	//_pRenderer->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"SphereMesh"));
	//_pRenderer->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"Default3DMat"));

	//AddObject(_pSphere, LAYER_TYPE::PLAYER);
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