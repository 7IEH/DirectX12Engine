#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"

#include "CSceneMgr.h"
#include "CRenderMgr.h"

#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CMaterial.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

extern MatrixInfo e_MatrixData;

Camera::Camera()
	:Component(COMPONENT_TYPE::CAMERA)
	, m_Projection(PROJECTION_TYPE::PERSPECTIVE)
	, m_FOV(XM_PI / 4.f)
	, m_Width(1600.f)
	, m_Height(900.f)
	, m_Scale(1.f)
	, m_AspectRatio(ASPECT_RATIO)
	, m_Far(100000.f)
	, m_Type(CAMERA_TYPE::NON_TYPE)
	, m_LayerVisible(0)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (CAMERA_TYPE::MAIN_CAMERA == m_Type)
	{
		Transform* _pTr = GetOwner()->GetComponent<Transform>();

		Vec3 _vRight = _pTr->GetWorldDir(DIRECTION_TYPE::RIGHT);
		Vec3 _vUp = _pTr->GetWorldDir(DIRECTION_TYPE::UP);
		Vec3 _vFront = _pTr->GetWorldDir(DIRECTION_TYPE::FRONT);

		Vec3 _vPos = _pTr->GetRelativePosition();
		Vec3 _vRot = _pTr->GetRelativeRotation();
		float _fCamSpeed = 10.f;

		if (KEY_PRESSED(KEY::A))
		{
			_vPos -= DT * _vRight * _fCamSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			_vPos += DT * _vRight * _fCamSpeed;
		}

		if (KEY_PRESSED(KEY::UP))
		{
			_vPos += DT * _vUp * _fCamSpeed;
		}

		if (KEY_PRESSED(KEY::DOWN))
		{
			_vPos -= DT * _vUp * _fCamSpeed;
		}

		if (KEY_PRESSED(KEY::W))
		{
			_vPos += DT * _vFront * _fCamSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			_vPos -= DT * _vFront * _fCamSpeed;
		}

		if (KEY_PRESSED(KEY::O))
		{
			_vRot.y -= DT * _fCamSpeed;
		}

		if (KEY_PRESSED(KEY::P))
		{
			_vRot.y += DT * _fCamSpeed;
		}

		_pTr->SetRelativePosition(_vPos);
		_pTr->SetRelativeRotation(_vRot);
	}
}

/*********************************
|	Camera View Projection Part
**********************************/
void Camera::LateUpdate()
{
	if (!GetOwner()->GetComponent<Transform>())
		return;

	Vec3 _pos = GetOwner()->GetComponent<Transform>()->GetRelativePosition();
	// W = (RT)^-1 -> T^-1 * R^-1
	Matrix _reverseTransform = XMMatrixTranslation(-_pos.x, -_pos.y, -_pos.z);

	Vec3 _Right = GetOwner()->GetComponent<Transform>()->GetWorldDir(DIRECTION_TYPE::RIGHT);
	Vec3 _UP = GetOwner()->GetComponent<Transform>()->GetWorldDir(DIRECTION_TYPE::UP);
	Vec3 _Front = GetOwner()->GetComponent<Transform>()->GetWorldDir(DIRECTION_TYPE::FRONT);

	// 해당 행렬은 직교행렬로 역행렬이 전치행렬과 같다.
	Matrix _reverseRotation = { _Right.x,_UP.x,_Front.x,0,
								_Right.y,_UP.y,_Front.y,0,
								_Right.z,_UP.z,_Front.z,0,
								0,0,0,1 };


	// 1. View Matrix
	m_ViewMat = XMMatrixMultiply(_reverseTransform, _reverseRotation);

	// 2. Projection Matrix
	switch (m_Projection)
	{
	case PROJECTION_TYPE::PERSPECTIVE:
	{
		ProjectiveView();
	}
	break;
	case PROJECTION_TYPE::ORTHOGRAPHIC:
	{
		OrthographicView();
	}
	break;
	case PROJECTION_TYPE::END:
		break;
	default:
		break;
	}
}

void Camera::InitializeDir()
{
	GetOwner()->GetComponent<Transform>()->InitializeDir();
}

void Camera::ProjectiveView()
{
	// Projection(Projection)
	m_ProjMat = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
}

void Camera::OrthographicView()
{
	// Projection(Orthographic)
	m_ProjMat = XMMatrixOrthographicLH(m_Width * m_Scale, m_Height * m_Scale, 1.f, m_Far);
}

/************************
|	Camera Render Part
************************/

void Camera::SortObject()
{
	Scene* _curLevel = SceneMgr::GetInst()->GetCurScene();

	for (int i = 0;i < (UINT)LAYER_TYPE::END;i++)
	{
		if (false == (m_LayerVisible & (1 << i)))
			continue;

		Layer* _layer = _curLevel->GetLayer(LAYER_TYPE(i));
		const vector<GameObject*>& _vObjects = _layer->GetLayerObjs();

		for (size_t _idx = 0;_idx < _vObjects.size();_idx++)
		{
			Renderer* _renderer = _vObjects[_idx]->GetComponent<Renderer>();

			if (_renderer == nullptr)
				continue;

			Mesh* _mesh = _renderer->GetMesh().Get();
			Material* _mat = _renderer->GetMaterial().Get();

			if (!(_mesh && _mat))
			{
				continue;
			}

			GraphicShader* _shader = _mat->GetGraphicShader().Get();

			if (nullptr == _shader)
				continue;

			SHADER_DOMAIN _domain = _renderer->GetMaterial()->GetGraphicShader()->GetDomain();

			// Sorting
			m_DomainSortingObjects[(UINT)_domain].push_back(_vObjects[_idx]);
		}
	}
}

void Camera::Render()
{
	// Cameara 시점 넣고 들어가기
	e_MatrixData.View = m_ViewMat;
	e_MatrixData.Projection = m_ProjMat;

	SortObject();

	for (int _idx = 0;_idx < (UINT)SHADER_DOMAIN::END;_idx++)
	{
		if (_idx == (UINT)SHADER_DOMAIN::DOMAIN_POSTPROCESS)
		{
			//PostRender(m_DomainSortingObjects[_idx]);
		}
		else
		{
			Render(m_DomainSortingObjects[_idx]);
		}
	}
}

void Camera::Render(vector<GameObject*>& _vecObj)
{
	for (size_t _idx = 0;_idx < _vecObj.size();_idx++)
	{
		_vecObj[_idx]->Render();
	}
	_vecObj.clear();
}

//void Camera::PostRender(vector<GameObject*>& _vecObj)
//{
//	for (size_t _idx = 0;_idx < _vecObj.size();_idx++)
//	{
//		RenderMgr::GetInst()->CopyResourceView();
//
//		Ptr<Sprite> pPostProcessTex = RenderMgr::GetInst()->GetPostProcessTexture2D();
//		pPostProcessTex->UpdateData(13);
//
//		_vecObj[_idx]->Render();
//	}
//	_vecObj.clear();
//}

void Camera::LayerVisibleSet(LAYER_TYPE _type, bool _visible)
{
	if (_visible)
	{
		m_LayerVisible |= (1 << (UINT)_type);
	}
	else
	{
		m_LayerVisible &= ~(1 << (UINT)_type);
	}
}

void Camera::AllVisibleSet(bool _visible)
{
	if (_visible)
	{
		m_LayerVisible = 0xffffffff;
	}
	else
	{
		m_LayerVisible = 0;
	}
}

void Camera::SetCameraType(CAMERA_TYPE _type)
{
	m_Type = _type;
	RenderMgr::GetInst()->RegisterCamera(_type, GetOwner());
}
