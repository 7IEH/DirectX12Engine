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
	// GPU에게 CPU에서 일을 요청하는데 외주 요청을 한꺼번에 주기 위해
	// 모아 놓는 큐
	ComPtr<ID3D12CommandQueue>				m_pCmdQueue;
	ComPtr<ID3D12CommandAllocator>			m_pCmdAlloc;
	// Context개념
	ComPtr<ID3D12GraphicsCommandList>		m_pCmdList;

	// Reosurce 용
	ComPtr<ID3D12CommandAllocator>			m_pResCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>		m_pResCmdList;

	// CPU와 GPU의 동기화를 위한 장치(멀티 스레딩 개념)
	ComPtr<ID3D12Fence>						m_pFence;
	UINT									m_iFenceVal;
	HANDLE									m_pFenceEvent;

	// GPGPU 용
	ComPtr<ID3D12CommandQueue>			m_pComCmdQueue;
	ComPtr<ID3D12CommandAllocator>		m_pComCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	m_pComCmdList;

	ComPtr<ID3D12Fence>					m_pComFence;
	UINT								m_iComFenceValue;
	HANDLE								m_pComFenceEvent;

	// SwapChain
	ComPtr<IDXGISwapChain>					m_pSwapChain;
	ComPtr<ID3D12Resource>					m_pRTT[SWAP_CHAIN_BUFFER_COUNT];
	UINT									m_iBackBufferIndex;

	// DescriptionHeap
	// 표준 규격 양식
	// DX11에 View
	ComPtr<ID3D12DescriptorHeap>			m_pRTV;
	D3D12_CPU_DESCRIPTOR_HANDLE				m_pRTVHandle[SWAP_CHAIN_BUFFER_COUNT];

	// Depth-Stencil
	ComPtr<ID3D12Resource>					m_pDST;
	ComPtr<ID3D12DescriptorHeap>			m_pDSV;
	D3D12_CPU_DESCRIPTOR_HANDLE				m_DSVHandle;

	// RootSignature
	ComPtr<ID3D12RootSignature>				m_pSignature;

	// ComputeSignature 
	ComPtr<ID3D12RootSignature>				m_pComSignature;

	WindowInfo								m_WindowInfo;
	UINT									m_i4MSAAQuality;

	// TableDescriptorHeap
	ComPtr<ID3D12DescriptorHeap>			m_pGPUHeap;
	UINT64									m_iHandleSize;
	UINT64									m_iGroupSize;
	UINT64									m_iGroupCount;

	UINT									m_iCurGroupIdx;

	ComPtr<ID3D12DescriptorHeap>			m_pComputeGPUHeap;
	UINT64									m_iComputeHandleSize;

	// SamplerState
	D3D12_STATIC_SAMPLER_DESC				m_Sampler[(UINT)SAMPLER_TYPE::END];

	// ConstantBuffer
	ConstantBuffer* m_pConstantBuffer[(UINT)CONSTANT_TYPE::END];

public:
	ID3D12Device* GetDevice() { return m_pDevice.Get(); }
	IDXGIFactory* GetDXGI() { return m_pDxgi.Get(); }
	ID3D12CommandQueue* GetCmdQueue() { return m_pCmdQueue.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return m_pCmdList.Get(); }
	ID3D12GraphicsCommandList* GetResCmdList() { return m_pResCmdList.Get(); }
	ID3D12GraphicsCommandList* GetComCmdList() { return m_pComCmdList.Get(); }

	Vec2							GetRes() { return m_WindowInfo.Res; }
	HWND							GetHwnd() { return m_WindowInfo.Hwnd; }

	// SwapChain Access Func
	IDXGISwapChain* GetSwapChain() { return m_pSwapChain.Get(); }
	ID3D12Resource* GetRTT(int _idx) { return m_pRTT[_idx].Get(); }

	UINT							GetCurBackBuffer() { return m_iBackBufferIndex; }
	ID3D12Resource* GetCurRTT() { return m_pRTT[m_iBackBufferIndex].Get(); }

	// HeapDescrition Func
	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int _idx) { return m_pRTVHandle[_idx]; }
	D3D12_CPU_DESCRIPTOR_HANDLE		GetBufferView() { return GetRTV(m_iBackBufferIndex); }

	ID3D12RootSignature* GetSignature() { return m_pSignature.Get(); }
	ID3D12RootSignature* GetComSignature() { return m_pComSignature.Get(); }

	ConstantBuffer* GetConstantBuffer(CONSTANT_TYPE _type) { return m_pConstantBuffer[(UINT)_type]; }

	// TableDescriptorHeap
	void							TableClear() { m_iCurGroupIdx = 0; }
	void							SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, CONSTANT_TYPE _type);
	void							SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, TEX_PARAM _type);
	void							SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, UINT _register);
	void							SetUAV(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, UINT _register);

	// ComputeDescriptorHeap
	void							SetCBV_CS(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, CONSTANT_TYPE _type);
	void							SetSRV_CS(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, UINT _register);
	void							SetUAV_CS(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, UINT _register);


	void							CommitTable();
	void							ComCommitTable();

	ID3D12DescriptorHeap* GetDescHeap() { return m_pGPUHeap.Get(); }

public:
	// CommandQueue Func
	void							WaitSync();

	void							RenderBegin();
	void							RenderEnd();
	// SwapChain Func
	void							ClearRenderTarget(float(&Color)[4]);
	void							Present();
	void							SwapIndex();

	// GPGPU CommandQueue Func
	void							ComWaitSync();
	void							FlushComputeCommandQueue();

public:
	int							Awake(const WindowInfo& _windowInfo);
	void						OMSetRT();
	void						CreateViewPort(Vec2 _pos, Vec2 _scale);

	void						FlushResourceCommandQueue();

private:
	HRESULT						CreateDevice();
	HRESULT						CreateCommandQueue();
	HRESULT						CreateSwapChain();
	void						CreateDecriptionHeap();
	HRESULT						CreateRootSignature();
	HRESULT						CreateComputeRootSignature();
	void						CreateTableDescriptorHeap(UINT _count);
	void						CreateComputeTableDescriptorHeap();

	void						CreateRTView();
	void						CreateDSView();
	HRESULT						CreateConstantBuffer();
	HRESULT						CreateConstantBufferIndividual(CONSTANT_TYPE _type, UINT _elementSize, UINT _elementCount);
	HRESULT						CreateSamplerState();
	void						ClearConstantBuffer();
};

