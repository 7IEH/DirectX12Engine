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

// 주변광(global illmination을 표현하기 위해 일정한 값으로 표현), 표면의 거침(난반사), 반사율
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
	Vec4	WorldPos;		// 위치
	Vec4	InitScale;
	Vec4	WorldScale;	// 크기
	Vec4	WorldRotation;	// 회전값
	Vec3	Velocity;		// 속도
	Vec4	Color;			// 색상
	Vec4	Force;
	Vec3	vNoiseForce;	// NoiseForce 모듈로 인한 랜덤 힘
	float	NoiseForceTime;

	int		FadeVariable;
	float	normalizeAge;
	float	Mass;			// 질량
	float	Age;			// 현재 나이
	float	Life;			// 수명
	int		Active;		// 활성화, 비활성화 여부
	int     padding[3];
};

struct tParticleModule
{
	// Sapwn 모듈
	Vec4	SpawnColor;	// 초기 컬러
	Vec4	SpawnMinScale;	// 초기 최소 크기
	Vec4	SpawnMaxScale;	// 초기 최대 크기

	int     iLoop;
	float	MinLife;		// 최소 수명
	float	MaxLife;		// 최대 수명
	float	MinMass;

	float	MaxMass;
	int		SpawnRate;		// 초당 생성 개수
	int		SpaceType;		// 좌표계(0 : LocalSpace, 1 : WorldSpace)
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