/******************
universal function
******************/

class GameObject;

namespace Object
{
	//void Instantiate(GameObject* _target, int _LayerIdx);
	//void Destroy(GameObject* _target);

	//void FadeIn(GameObject* _target, float _time);
	//void FadeOut(GameObject* _target, float _time);

	//void FadeInLightAmbient(GameObject* _target, float _time);
	//void FadeOutLightAmbient(GameObject* _target, float _time);

	//void FadeInLightColor(GameObject* _target, float _time);
	//void FadeOutLightColor(GameObject* _target, float _time);

	//void FadeInLightRadius(GameObject* _target, float _radius, float _time);
	//void FadeOutLightRadius(GameObject* _target, float _radius, float _time);

	//void FadeInText(GameObject* _target, float _time);
	//void FadeOutText(GameObject* _target, float _time);

	//void MoveUp(GameObject* _target, float _dest, float _speed);
	//void MoveDown(GameObject* _target, float _dest, float _speed);
	//void MoveRight(GameObject* _target, float _dest, float _speed);
	//void MoveLeft(GameObject* _target, float _dest, float _speed);

	//void GrowEffect(GameObject* _target, float _scale, float _speed);
	//void DecreaseEffect(GameObject* _target, float _scale, float _speed);

	//void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	//void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	//void DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	//void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	//void ShakingEffect(float _fTime, float _fSpeed, float _fDistance);
	////void DrawDebugCube();
	////void DrawDebugSphere();

	//void Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap = true);
	//void Play2DBGM(const wstring& _SoundPath, float _Volume);

	//void Stop2DSound(const wstring& _SoundPath);
}

namespace SceneManager
{
	void LoadScene(const wstring& _sceneName);

	void SelectScene(const wstring& _sceneName);
}

namespace EH
{
	string ConvertString(const wstring& _wstr);

	wstring ConvertWstring(const string& _str);

	//string ConvertString(const std::filesystem::path::string_type _str);

	void InputVector2(string _line, OUT Vec2& _output);

	// 메모장에서 가져온 숫자 변환
	void InputVector3(string _line, OUT Vec3& _output);

	// 메모장에서 가져온 숫자 변환
	void InputVector3(string _line, OUT Vec4& _output);

	// 메모장에서 가져온 숫자 변환
	void InputVector4(string _line, OUT Vec4& _output);

	// 메모장에 들어갈 숫자 형식 변환
	string WriteVector2(Vec2 _data);

	string WriteVector3(Vec3 _data);

	string WriteVector4(Vec4 _data);

	wstring wWriteVector2(Vec2 _data);

	wstring wWriteVector3(Vec3 _data);

	wstring wWriteVector4(Vec4 _data);

	void InputVector2(wstring _line, OUT Vec2& _output);

	// 메모장에서 가져온 숫자 변환
	void InputVector3(wstring _line, OUT Vec3& _output);

	// 메모장에서 가져온 숫자 변환
	void InputVector3(wstring _line, OUT Vec4& _output);

	// 메모장에서 가져온 숫자 변환
	void InputVector4(wstring _line, OUT Vec4& _output);
}

// 메모리 해제 ( 배열용 )
template <typename T, int SIZE>
void ReleaseArray(T* (&arr)[SIZE])
{
	for (int _idx = 0;_idx < SIZE;_idx++)
	{
		if (arr[_idx] != nullptr)
		{
			delete arr[_idx];
			arr[_idx] = nullptr;
		}
	}
}

// 메모리 해제 ( 벡터용 )
template <typename T>
void ReleaseVector(vector<T*>& vec)
{
	for (size_t _idx = 0;_idx < vec.size();_idx++)
	{
		if (vec[_idx] != nullptr)
		{
			delete vec[_idx];
			vec[_idx] = nullptr;
		}
	}

	vec.clear();
}

// 메모리 해제 ( Map용 ) 
template<typename T, typename Y>
void ReleaseMap(map<T, Y>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
}

// 한면의 법선 벡터 구하기
//void ComputeNormal(const XMVECTOR& p0, const XMVECTOR& p1, const XMVECTOR& p2, OUT XMVECTOR& out)
//{
//	XMVECTOR u = p1 - p0;
//	XMVECTOR v = p2 - p0;
//
//	out = DirectX::XMVector3Cross(u, v);
//	out = DirectX::XMVector3Normalize(out);
//}