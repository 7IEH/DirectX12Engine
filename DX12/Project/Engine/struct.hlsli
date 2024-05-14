#ifndef _STRUCT
#define _STRUCT

struct LightInfo
{
    float4 Color;
    float4 Ambient;
    float4 Specular;
    float4 Diffuse;

    float3 Position;
    float Radius;

    float3 LightDir;
    int LightType;
	
    float3 Att;
    float Spot;

    float Angle;
    float3 pad;
};

Texture2D ST0 : register(t0);
Texture2D ST1 : register(t1);
Texture2D ST2 : register(t2);
Texture2D ST3 : register(t3);
Texture2D ST4 : register(t4);
Texture2D ST5 : register(t5);

Texture2D CUBE6 : register(t6);
Texture2D CUBE7 : register(t7);

Texture2D STARR8 : register(t8);
Texture2D STARR9 : register(t9);

SamplerState sampler1 : register(s0);

struct LightMaterial
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Reflect;
    float4 Color;
};

struct Material
{
    int _int0;
    int _int1;
    int _int2;
    int _int3;
    
    float _float0;
    float _float1;
    float _float2;
    float _float3;
    
    float2 _vec0;
    float2 _vec1;
    float2 _vec2;
    float2 _vec3;
   
    LightMaterial _LightMat;
    
    matrix _matrix0;
    matrix _matrix1;
    matrix _matrix2;
    matrix _matrix3;
    
    int spriteCheck0;
    int spriteCheck1;
    int spriteCheck2;
    int spriteCheck3;
    int spriteCheck4;
    int spriteCheck5;

    int TEXCUBE_0;
    int TEXCUBE_1;

    int TEXARR_0;
    int TEXARR_1;
    
    int2 padding;
};

struct tTileInfo
{
    float2 vLeftTopUV;
    int bRender;
    int padding;
};

struct tParticle
{
    float4 _LocalPos;
    float4 _WorldPos;
    float4 _WorldInitScale;
    float4 _WorldScale;
    float4 _WorldRotation;
    float3 _Velocity;
    float4 _Color;
    float4 _Force;
    float3 _NoiseForce;
    float _NoiseForceTime;
    
    int _iFadeVariable;
    float _NomarlizedAge;
    float _Mass;
    float _Age;
    float _Life;
    int _Active;
    int _padding[3];
};

struct tParticleModule
{
	// Sapwn 모듈
    float4 _vSpawnColor; // 초기 컬러
    float4 _vSpawnMinScale; // 초기 최소 크기
    float4 _vSpawnMaxScale; // 초기 최대 크기
    
    int _iLoop;
    
    float MinLife; // 최소 수명
    float MaxLife; // 최대 수명
    float MinMass;
    float MaxMass;
    int SpawnRate; // 초당 생성 개수
    int SpaceType; // 좌표계(0 : LocalSpace, 1 : WorldSpace)
    
    int spawnShape;
    float Radius;
    float4 _spawnBoxScale;
    
    float4 _vScaleRatio;
    
    float _noiseForceScale;
    float NoiseForceTerm;
    
    int _AddVelocityType;
    float _MinSpeed;
    float _MaxSpeed;
    float _FixedAngle;
    float4 _FixedDirection;
    
    int _ColorType;
    
    int _arrModuleCheck[7];
    int _padding;
};

struct tSpawnCount
{
    int iSpawnCount;
    float3 iPadding;
};


cbuffer Worldspcae : register(b1)
{
    matrix World;
    matrix matWorldInv;
    
    matrix View;
    matrix matViewInv;
    
    matrix Projection;
    matrix matProjInv;
    
    matrix WV;
    matrix WVP;
};

cbuffer Material : register(b2)
{
    Material gMatrial;
}

cbuffer tAnimationInfo : register(b3)
{
    float2 gOffsetSize;
    float2 gLeftTop;
    float2 gSliceSize;
    float2 gBackground;
    int gAnimUse;
    float3 gPadding;
}

cbuffer tGlobalData : register(b0)
{
    float2 gRenderResolution;
    float gDt;
    float gAccTime;
    float gAccTime2;
    float gAccTime3;
    int gLight2DSize;
    int gLight3DSize;
}

StructuredBuffer<Matrix> g_mat_bone : register(t10);
StructuredBuffer<LightInfo> gLightInfo2D : register(t11);
StructuredBuffer<LightInfo> gLightInfo3D : register(t12);
#endif