#pragma once

class StructuredBuffer;
class RenderMgr
	:public Singleton<RenderMgr>
{
	Single(RenderMgr)
private:
	GameObject*			m_pCams[(UINT)CAMERA_TYPE::END];

	vector<GameObject*> m_vLights2D;
	vector<GameObject*> m_vLights3D;

	StructuredBuffer* m_pLight2DBuffer;
	StructuredBuffer* m_pLight3DBuffer;

public:
	void Awake();
	void Update();

public:
	void RegisterCamera(CAMERA_TYPE _type, GameObject* _cam) { m_pCams[(UINT)_type] = _cam; }
	void RegisterLight2D(GameObject* _light2D) { m_vLights2D.push_back(_light2D); }
	void RegisterLight3D(GameObject* _light3D) { m_vLights3D.push_back(_light3D); }

private:
	void UpdateData();
	void Render();
	// LightBufferUpdate
	void Clear();
};

