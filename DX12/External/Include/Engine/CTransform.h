#pragma once
#include "CComponent.h"
class Transform :
    public Component
{
private:
	TransformInfo	m_RelativeTransform;

	Matrix		m_WorldMatrix;

	bool			m_bAbsolute;

	Vec3			m_LocalDir[(UINT)DIRECTION_TYPE::END];
	Vec3			m_WorldDir[(UINT)DIRECTION_TYPE::END];

public:
	TransformInfo	GetRelativeTransform() { return m_RelativeTransform; }
	Matrix		GetMatWorld() { return m_WorldMatrix; }

	void SetRelativeScale(Vec3 _scale) { m_RelativeTransform.Scale = _scale; }
	void SetRelativeRotation(Vec3 _rotation) { m_RelativeTransform.Rotation = _rotation; }
	void SetRelativePosition(Vec3 _position) { m_RelativeTransform.Position = _position; }

	Vec3 GetRelativeScale() { return m_RelativeTransform.Scale; }
	Vec3 GetRelativeRotation() { return m_RelativeTransform.Rotation; }
	Vec3 GetRelativePosition() { return  m_RelativeTransform.Position; }

	Vec3 GetWorldScale();

	Vec3 GetLocalDir(DIRECTION_TYPE _type) { return m_LocalDir[(UINT)_type]; }
	void SetLocalDir(DIRECTION_TYPE _type, Vec3 _dir) { m_LocalDir[(UINT)_type] = _dir; }

	Vec3 GetWorldDir(DIRECTION_TYPE _type) { return m_WorldDir[(UINT)_type]; }
	void SetWorldDir(DIRECTION_TYPE _type, Vec3 _dir) { m_WorldDir[(UINT)_type] = _dir; }

	void InitializeDir();

	void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }

	void SetWorldMat(Matrix _WorldMatrix) { m_WorldMatrix = _WorldMatrix; }

public:
	virtual void LateUpdate() override;
	virtual void UpdateData() override;

	CLONE(Transform)
public:
	Transform();
	virtual ~Transform();
};

