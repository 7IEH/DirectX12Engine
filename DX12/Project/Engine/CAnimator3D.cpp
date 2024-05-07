#include "pch.h"
#include "CAnimator3D.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"
#include "CConstantBuffer.h"
#include "CGameObject.h"

#include "CTimeMgr.h"

Animator3D::Animator3D()
	:Component(COMPONENT_TYPE::ANIMATOR3D)
	, m_fUpdateTime(0.f)
	, m_iClipIndex(0)
	, m_iFrame(0)
	, m_iNextFrame(0)
	, m_fFrameRatio(0.f)
	, m_bBoneFinalUpdated(FALSE)
	, m_vBoneInfoes{}
	, m_vAnimClips{}
	, m_pBoneMatBuffer(nullptr)
	, m_pBoneFrameBuffer(nullptr)
	, m_pBoneOffsetBuffer(nullptr)
	, m_pConst{}
{
	m_pAnimComputeShader = dynamic_cast<AnimComputeShader*>(
		AssetMgr::GetInst()->FindAsset<ComputeShader>(L"AnimComputeShader").Get());

	m_pBoneMatBuffer = new StructuredBuffer;

	m_pBoneFrameBuffer = new StructuredBuffer;

	m_pBoneOffsetBuffer = new StructuredBuffer;
}

Animator3D::~Animator3D()
{
	if (nullptr != m_pBoneMatBuffer)
		delete m_pBoneMatBuffer;

	if (nullptr != m_pBoneFrameBuffer)
		delete m_pBoneFrameBuffer;

	if (nullptr != m_pBoneOffsetBuffer)
		delete m_pBoneFrameBuffer;
}

void Animator3D::Play(UINT _idx)
{
	assert(_idx < m_vAnimClips->size());
	m_iClipIndex = _idx;
	m_fUpdateTime = 0.f;
}

void Animator3D::LateUpdate()
{
	m_fUpdateTime += DT;

	const AnimClipInfo& animClip = m_vAnimClips->at(m_iClipIndex);
	if (m_fUpdateTime >= animClip.duration)
		m_fUpdateTime = 0.f;

	const int ratio = static_cast<int>(animClip.frameCount / animClip.duration);
	m_iFrame = static_cast<int>(m_fUpdateTime * ratio);
	m_iFrame = min(m_iFrame, animClip.frameCount - 1);
	m_iNextFrame = min(m_iFrame + 1, animClip.frameCount - 1);
	m_fFrameRatio = static_cast<float>(m_iFrame - m_iFrame);
}

void Animator3D::Render()
{
	UINT _iBoneCount = static_cast<UINT>(m_vBoneInfoes->size());
	if (m_pBoneMatBuffer->GetElementCount() < _iBoneCount)
	{
		delete m_pBoneMatBuffer;
		m_pBoneMatBuffer = nullptr;
		m_pBoneMatBuffer->Create(sizeof(Matrix), _iBoneCount, SB_TYPE::READ_WRITE, TRUE);
	}

	if (nullptr != m_pAnimComputeShader)
		return;

	m_pConst.iArr[0] = _iBoneCount;
	m_pConst.iArr[1] = m_iFrame;
	m_pConst.iArr[2] = m_iNextFrame;
	m_pConst.fArr[0] = m_fFrameRatio;

	Ptr<Mesh> _mesh = GetOwner()->GetComponent<MeshRenderer>()->GetMesh();
	Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL)->UpdateData_CS(&m_pConst, sizeof(m_pConst), 1);

	m_pAnimComputeShader->SetBoneOffsetBuffer(_mesh->GetOffsetBuffer());
	m_pAnimComputeShader->SetBoneFrameBuffer(_mesh->GetFrameBuffer(m_iClipIndex));
	m_pAnimComputeShader->SetBoneBuffer(m_pBoneMatBuffer);

	m_pAnimComputeShader->Execute();

	m_pBoneMatBuffer->UpdateData(10);
}