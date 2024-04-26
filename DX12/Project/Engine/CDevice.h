#pragma once

class ConstantBuffer;
class Device
	:public Singleton<Device>
{
	Single(Device)
private:
	// Device
	ComPtr<ID3D12Debug>						m_pDebugContoller;
	ComPtr<IDXGIFactory>					m_pDxgi;
	ComPtr<ID3D12Device>					m_pDevice;

	// Command Queue
	// GPU���� CPU���� ���� ��û�ϴµ� ���� ��û�� �Ѳ����� �ֱ� ����
	// ��� ���� ť
	ComPtr<ID3D12CommandQueue>				m_pCmdQueue;
	ComPtr<ID3D12CommandAllocator>			m_pCmdAlloc;
	// Context����
	ComPtr<ID3D12GraphicsCommandList>		m_pCmdList;

	// CPU�� GPU�� ����ȭ�� ���� ��ġ(��Ƽ ������ ����)
	ComPtr<ID3D12Fence>						m_pFence;
	UINT									m_iFenceVal;
	HANDLE									m_pFenceEvent;

	// SwapChain
	ComPtr<IDXGISwapChain>					m_pSwapChain;
	ComPtr<ID3D12Resource>					m_pRTT[SWAP_CHAIN_BUFFER_COUNT];
	UINT									m_iBackBufferIndex;

	// DescriptionHeap
	// ǥ�� �԰� ���
	// DX11�� View
	ComPtr<ID3D12DescriptorHeap>			m_pRTV;
	UINT									m_iRTVHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE				m_pRTVHandle[SWAP_CHAIN_BUFFER_COUNT];

	WindowInfo								m_WindowInfo;
	UINT									m_i4MSAAQuality;

	ConstantBuffer* m_pConstantBuffer[(UINT)CONSTANT_TYPE::END];

public:
	ID3D12Device*					GetDevice() { return m_pDevice.Get(); }
	IDXGIFactory*					GetDXGI() { return m_pDxgi.Get(); }
	ID3D12CommandQueue*				GetCmdQueue() { return m_pCmdQueue.Get(); }
	
	Vec2							GetRes() { return m_WindowInfo.Res; }
	HWND							GetHwnd() { return m_WindowInfo.Hwnd; }

	// SwapChain Access Func
	IDXGISwapChain*					GetSwapChain() { return m_pSwapChain.Get(); }
	ID3D12Resource*					GetRTT(int _idx) { return m_pRTT[_idx].Get(); }

	UINT							GetCurBackBuffer() { return m_iBackBufferIndex; }
	ID3D12Resource*					GetCurRTT() { return m_pRTT[m_iBackBufferIndex].Get(); }

	// HeapDescrition Func
	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int _idx) { return m_pRTVHandle[_idx]; }
	D3D12_CPU_DESCRIPTOR_HANDLE		GetBufferView() { return GetRTV(m_iBackBufferIndex); }
	
public:
	// CommandQueue Func
	void							WaitSync();

	void							RenderBegin();
	void							RenderEnd();
	// SwapChain Func
	void							ClearRenderTarget(float(&Color)[4]);
	void							Present();
	void							SwapIndex();

public:
	int							Awake(const WindowInfo& _windowInfo);
	void						OMSetRT();
	void						CreateViewPort(Vec2 _pos, Vec2 _scale);

private:
	HRESULT						CreateDevice();
	HRESULT						CreateCommandQueue();
	HRESULT						CreateSwapChain();
	HRESULT						CreateDecriptionHeap();

	HRESULT						CreateRTView();
	HRESULT						CreateDSView();
	HRESULT						CreateConstantBuffer();
	HRESULT						CreateConstantBufferIndividual(CONSTANT_TYPE _type, UINT _elementSize, UINT _elementCount);
	//HRESULT						CreateRasterizerState();
	//HRESULT						CreateDepthStencilState();
	HRESULT						CreateBlendState();
	HRESULT						CreateSamplerState();
	void						SetSamplerState();

};

