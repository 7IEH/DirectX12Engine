#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CAnimComputeShader.h"

class Material;
class StructuredBuffer;
class Mesh;

class Animator3D :
	public Component
{
private:
	const vector<BoneInfo>*					m_vBoneInfoes;
	const vector<AnimClipInfo>*				m_vAnimClips;

	Ptr<AnimComputeShader>					m_pAnimComputeShader;
	StructuredBuffer*						m_pBoneMatBuffer;
	StructuredBuffer*						m_pBoneFrameBuffer;
	StructuredBuffer*						m_pBoneOffsetBuffer;

	MaterialInfo							m_pConst;

	// AnimInfo
	float							m_fUpdateTime = 0.f;
	int							    m_iClipIndex = 0;
	int							    m_iFrame = 0;
	int							    m_iNextFrame = 0;
	float							m_fFrameRatio = 0;

	bool							m_bBoneFinalUpdated;

public:
	void SetBones(const vector<BoneInfo>* _vBoneInfoes) { m_vBoneInfoes = _vBoneInfoes; }
	void SetAnimClip(const vector<AnimClipInfo>* _vAnimClips) { m_vAnimClips = _vAnimClips; }

	int GetAnimCount() { return static_cast<UINT>(m_vAnimClips->size()); }
	int GetCurrentClipIndex() { return m_iClipIndex; }
	void Play(UINT idx);

public:
	virtual void LateUpdate()override;
	void Render();

public:
	CLONE(Animator3D)
		Animator3D();
	virtual ~Animator3D();
};