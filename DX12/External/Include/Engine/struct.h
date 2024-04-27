#pragma once

/******************
Struct Type
******************/

#include "CTexture.h"

// Task Sturct
struct EHTask
{
	TASK_TYPE _Type;
	UINT_PTR  Param_1;
	UINT_PTR  Param_2;
	UINT_PTR  Param_3;
	INT_PTR	  Param_4;
};

// Vertex Struct
struct VertexInfo
{
	Vec3 Position;
	Vec4 Color;
	//Vec2 UV;
	//Vec3 Tangent;
	//Vec3 Normal;
	//Vec3 Binormal;
};

// SRT Struct
struct TransformInfo
{
	Vec3 Scale;
	Vec3 Position;
	Vec3 Rotation;
};

// MVP Struct
struct MatrixInfo
{
	Matrix	World;
	Matrix	WorldInv;

	Matrix	View;
	Matrix	ViewInv;

	Matrix	Projection;
	Matrix	ProjInv;

	Matrix	WV;
	Matrix	WVP;
};

// Key Struct
struct FKeyData
{
	KEY			eKey;
	KEY_STATE	eState;
	bool		bPressed;
};

struct LightMateiral
{
	Vec4	Ambient;
	Vec4	Diffuse;
	Vec4	Specular;
	Vec4	Reflect;
	Vec4	Color;
};

// �ֺ���(global illmination�� ǥ���ϱ� ���� ������ ������ ǥ��), ǥ���� ��ħ(���ݻ�), �ݻ���
struct MaterialInfo
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	LightMateiral vLightMat;
	Matrix MatArr[4];

	int	Sprite[(UINT)TEX_PARAM::END];
	int padding[2];
};

// Light Parameter
struct LightInfo
{
	Vec4	Color;
	Vec4	Ambient;
	Vec4	Specular;
	Vec4	Diffuse;

	Vec3	Position;
	float	Radius;

	Vec3	LightDir;
	int		LightType;

	Vec3	Att;
	float	Spot;

	float	Angle;
	Vec3	Padding;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE	eShape;

	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRot;
	Matrix		matWorld;

	Vec3		vColor;
	float		fDuration;
	float		fLifeTime;
	bool		bDepthTest;
};

struct AnimationInfo
{
	Vec2	OffsetSize;
	Vec2	LeftTop;
	Vec2	SliceSize;
	Vec2	BackGround;
	int		AnimUse;
	Vec3	Padding;
};

struct GlobalDataInfo
{
	Vec2	RenderResolution;
	float	Dt;
	float	AccTime;
	float	AccTime2;
	float	AccTime3;
	int		Light2DSize;
	int		Light3DSize;
};

// TilePaletteInfo
struct tTilePaletteInfo
{
	Ptr<Texture> atlas;
	Vec2		Face;
	Vec2		RenderSize;
	Vec2		PixelSize;

	int			TileIdx;
	int			Render;
	int			ColliderEnabled;
};

// Particle System
struct tParticle
{
	Vec4	LocalPos;
	Vec4	WorldPos;		// ��ġ
	Vec4	InitScale;
	Vec4	WorldScale;	// ũ��
	Vec4	WorldRotation;	// ȸ����
	Vec3	Velocity;		// �ӵ�
	Vec4	Color;			// ����
	Vec4	Force;
	Vec3	vNoiseForce;	// NoiseForce ���� ���� ���� ��
	float	NoiseForceTime;

	int		FadeVariable;
	float	normalizeAge;
	float	Mass;			// ����
	float	Age;			// ���� ����
	float	Life;			// ����
	int		Active;		// Ȱ��ȭ, ��Ȱ��ȭ ����
	int     padding[3];
};

struct tParticleModule
{
	// Sapwn ���
	Vec4	SpawnColor;	// �ʱ� �÷�
	Vec4	SpawnMinScale;	// �ʱ� �ּ� ũ��
	Vec4	SpawnMaxScale;	// �ʱ� �ִ� ũ��

	int     iLoop;
	float	MinLife;		// �ּ� ����
	float	MaxLife;		// �ִ� ����
	float	MinMass;

	float	MaxMass;
	int		SpawnRate;		// �ʴ� ���� ����
	int		SpaceType;		// ��ǥ��(0 : LocalSpace, 1 : WorldSpace)
	int		SpawnShape;

	float	Radius;
	Vec4	SpawnBoxScale;

	// Scale
	Vec4	ScaleRatio;

	// Noise Force
	float	NoiseForceScale;
	float	NoiseForceTerm;
	int		AddVelocityType;

	float	MinSpeed;
	float	MaxSpeed;
	float	FixedAngle;
	Vec4	FixedDirection;

	int		ColorType;

	int		ArrModuleCheck[(UINT)PARTICLE_MODULE::END];
	int		padding;
};

struct tSpawnCount
{
	int		SpawnCount;
	Vec3	Padding;
};

struct WindowInfo
{
	HWND Hwnd;
	Vec2 Res;
	bool Windowed;
};