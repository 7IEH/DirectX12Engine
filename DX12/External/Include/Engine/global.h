#pragma once

#include <stdlib.h>
#include <Windows.h>
#include <string>
#include <typeinfo>

#include <vector>
#include <queue>
#include <list>
#include <map>

#include <fstream>
#include <iostream>
#include <filesystem>

using std::vector;
using std::list;
using std::map;
using std::queue;
using std::make_pair;

using std::wstring;
using std::string;

#include <wrl.h>
using namespace Microsoft::WRL;
#include <d3dcompiler.h>

// Directx 12 헤더 및 라이브러리 참조

#include "d3dx12.h"
#include <d3d12.h>
#include <dxgi.h>
#include <DirectXColors.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")


#include <DirectXTex.h>
#include <DirectXMath.h>
using namespace DirectX;

#ifdef _DEBUG
#pragma comment(lib,"DirectXTex_debug")
#else
#pragma comment(lib,"DirectXTex")
#endif

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"

void HandleError(wstring _content, int flag);

//// ImGUI Header
//#include "imgui_stdlib.h"
//#include "imgui_impl_dx11.h"
//#include "imgui_impl_win32.h"

// Extern
#include "Extern.h"