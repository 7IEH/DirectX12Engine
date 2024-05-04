/******************
enum class Type
******************/

// Task Type
enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Adress
	CREATE_OBJECT,

	// Param1 : Object Adress
	DELETE_OBJECT,

	// Param1 : LEVEL_TYPE
	LEVEL_CHANGE,

	// Param1 : Parent Object, Param2 : Child Object
	ADD_CHILD,

	// Param1 : Parent Object, Param2 : Child Object
	DISCONNECT_PARENT,

	// Param1 : Level , Param2 : Path
	LOAD_LEVEL,

	// Param1 : Object Address Param2 : Time
	FADE_IN,

	// Param1 : Object Address Param2 : Time
	FADE_OUT,

	// Param1 : Object Address Param2 : Time
	FADE_IN_LIGHT_AMBIENT,

	// Param1 : Object Address Param2 : Time
	FADE_OUT_LIGHT_AMBIENT,

	// Param1 : Object Address Param2 : Time
	FADE_IN_LIGHT_COLOR,

	// Param1 : Object Address Param2 : Time
	FADE_OUT_LIGHT_COLOR,

	// Param1 : Object Address Param2 : Time
	FADE_IN_LIGHT_RADIUS,

	// Param1 : Object Address Param2 : Time
	FADE_OUT_LIGHT_RADIUS,

	// Param1 : Object Address Param2 : Time
	FADE_IN_TEXT_COLOR,

	// Param1 : Object Address Param2 : Time
	FADE_OUT_TEXT_COLOR,

	// Param1 : Object Address Param2 : Dest Param3 : Speed
	MOVE_UP,

	// Param1 : Object Address Param2 : Dest Param3 : Speed
	MOVE_DOWN,

	// Param1 : Object Address Param2 : Dest Param3 : Speed
	MOVE_RIGHT,

	// Param1 : Object Address Param2 : Dest Param3 : Speed
	MOVE_LEFT,

	// Param1 : Object Address
	SHAKE_EFFECT,

	// Param1 : Object Address Param2 : Scale Param3 : Speed
	GROW_EFFECT,

	// Param1 : Object Address Param2 : Scale Param3 : Speed
	DECREASE_EFFECT,
};

// Key Type
enum KEY
{
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,

	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,

	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,

	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	TAB,
	LSHIFT,
	LALT,
	LCTRL,
	SPACE,

	ESC,
	ENTER,
	BACK,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	LBTN,
	RBTN,
	MBTN,

	KEY_END,
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2
};

// Key State Type
enum KEY_STATE
{
	NONE,
	TAP,
	PRESSED,
	RELEASED,
};

// Component Type
enum class COMPONENT_TYPE
{
	TRANSFORM,
	LIGHT2D,
	LIGHT3D,
	COLLIDER2D,
	ANIMATOR2D,
	RENDERER,
	CAMERA,
	RIGIDBODY,
	BUTTON,
	TEXT,
	TILEMAP,
	TILEMAPCOLLIDER,
	END,

	SCRIPT
};

// Renderer Type
enum class RENDERER_TYPE
{
	MESHRENDERER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	CANVAS,
	END
};

// Buffer Type
enum class BUFFER_TYPE
{
	VERTEX,
	INDEX,
	END
};

// Asset Type
enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	SHADER,
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	MATERIAL,
	TEXTURE,
	SPRITE,
	SOUND,
	END,
};

// Shader Type
enum class SHADER_TYPE
{
	VERTEX,
	PIXEL,
	HULL,
	DOMAlN,
	GEOMETRY,
	END,
};

// Constant Buffer Type
enum class CONSTANT_TYPE
{
	GLOBAL,
	TRANSFORM,
	MATERIAL,
	ANIMATION,
	END,
};

// Layer Type
// ImGuI
// 1. Inspector 확인
// 2. CollisionMatrix 확인
// 3. Camera Visible 확인
// 4. Scene 다시 체크
enum class LAYER_TYPE
{
	CAMERA,
	LIGHT2D,
	PLAYER,
	DEFAULT,
	END,
};

// Projection Type
enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
	END
};

// Direction Type
enum class DIRECTION_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END
};

// Culling Type
enum class CULL_TYPE
{
	BACK,
	FRONT,
	NONE,
	WIRE,
	END
};

// DepthStencil State Type
enum class DS_TYPE
{
	LESS,					// 작은게 통과			깊이 기록 0
	LESS_EQUAL,				// 작거나 같으면 통과	깊이 기록 0

	GREATER,				// 큰게 통과			깊이 기록 0
	GREATER_EQUAL,			// 크거나 같으면 통과	깊이 기록 0

	NO_TEST,				// 깊이 테스트X			깊이 기록 0
	N0_WRITE,				// 깊이 테스트O			깊이 기록 X

	NO_TEST_NO_WRITE,		// 깊이 테스트X			깊이 기록X
	END
};

// BlendState Type
enum class BLEND_TYPE
{
	DEFAULT,
	ALPHABLENDING,
	ONE_ONE,
	END
};

// SamplerState Type
enum class  SAMPLER_TYPE
{
	Default,
	POINT,
	END,
};

// Camera Type
enum class CAMERA_TYPE
{
	MAIN_CAMERA,
	DEBUG_CAMERA,
	UI_CAMERA,
	WORLD_CAMERA,
	END,
	NON_TYPE
};

// Scalar Param
enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	AMBIENT,
	DIFFUSE,
	SPECULAR,
	REFLECT,
	COLOR,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

// Texture Param
enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};

// Debug Shape
enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,

	CUBE,
	SPHERE,
};

// Map Generator Room Direction
enum class DIR_ROOM
{
	RIGHT,
	LEFT,
	TOP,
	BOTTOM,
	CLOSED,
	END
};

// IMGUI FONT TYPE
enum class FONT_TYPE
{
	Default,
	Maple,
	END,
};

// COLLIDER TYPE
enum class COLLIDER_TYPE
{
	BOXCOLLIDER2D,
	CIRCLECOLLDIER2D,
	BOXCOLLIDER,
	SPHERECOLLDIER,
	END,
};

// STRUCTURED BUFFER TYPE
enum class STRUCTURED_TYPE
{
	READ_ONLY,
	READ_WRITE,
	END
};

// LIGHT TYPE
enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
	END,
};

// SHADER DOMAIN TYPE
enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,
	DOMAIN_MASKED,
	DOMAIN_TRANSPARENT,
	DOMAIN_POSTPROCESS,

	DOMAIN_DEBUG,
	END,
};

// ANIMATION CREATE UI
enum class ANIMATION_UI
{
	CREATE
	, EDIT
	, END
};

// Particle Module
enum class PARTICLE_MODULE
{
	SPAWN,	// 파티클 생성 관련
	DRAG,	// 감속 관련 모듈
	SCALE,
	ADD_VELOCITY,
	NOISE_FORCE,
	CALCULATE_FORCE,
	COLOR_OVER_LIFETIME,

	END,
};

enum class Dir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	None,
};