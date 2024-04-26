#include "pch.h"
#include "CDevice.h"

#include "CConstantBuffer.h"

Device::Device()
	: m_WindowInfo{}
	, m_i4MSAAQuality(0)
	, m_pConstantBuffer{}
	, m_iFenceVal(0)
	, m_pFenceEvent(INVALID_HANDLE_VALUE)
	, m_iBackBufferIndex(0)
{

}

Device::~Device()
{
	::CloseHandle(m_pFenceEvent);
	ReleaseArray<ConstantBuffer, int(CONSTANT_TYPE::END)>(m_pConstantBuffer);
}

int Device::Awake(const WindowInfo& _windowInfo)
{
	m_WindowInfo = _windowInfo;

#ifdef _DEBUG
	// IID_PPV_ARGS -> ID�� �����͸� ������
	::D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugContoller));
	m_pDebugContoller->EnableDebugLayer();
#endif
	CreateDevice();

	CreateCommandQueue();

	CreateSwapChain();

	CreateDecriptionHeap();

	CreateRTView();

	CreateDSView();

	CreateBlendState();
	CreateSamplerState();

	if (FAILED(CreateConstantBuffer()))
	{
		HandleError(L"Failed Create ConstantBuffer", 0);
	}

	return 0;
}

HRESULT Device::CreateDevice()
{
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));

	CreateDXGIFactory(IID_PPV_ARGS(&m_pDxgi));

	return S_OK;
}

HRESULT Device::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC _tDesc = {};
	_tDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	_tDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	HRESULT _hr = m_pDevice->CreateCommandQueue(&_tDesc, IID_PPV_ARGS(&m_pCmdQueue));

	// D3D12_COMMAND_LIST_TYPE_DIRECT : GPU�� ���� �����ϴ� ���� ���
	_hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAlloc));

	// GPU�� �ϳ��� �ý��ۿ����� 0����
	// DIREC or BUNDLE
	// Allocator
	// �ʱ� ���� ( �׸��� ������ nullptr  ����)
	// m_pCmdAlloc-> list���� ���ɵ��� ������ �ش� �޸� �ڸ��� ���ִ°� �ƴ϶� ����ΰ� �����ϴµ� �� �� ������ ������ ��
	_hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&m_pCmdList));

	// CommandList�� Close/open ���°� �ִµ�
	// Open ���¿��� Command�� �ִٰ� Close�� ���� �����ϴ� ����
	_hr = m_pCmdList->Close();

	_hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
	m_pFenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return _hr;
}

HRESULT Device::CreateSwapChain()
{
	m_pSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC _tDesc = {};
	_tDesc.BufferDesc.Width = static_cast<UINT>(m_WindowInfo.Res.x);
	_tDesc.BufferDesc.Height = static_cast<UINT>(m_WindowInfo.Res.y);
	_tDesc.BufferDesc.RefreshRate.Numerator = 60;
	_tDesc.BufferDesc.RefreshRate.Denominator = 1;
	_tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	_tDesc.SampleDesc.Count = 1;
	_tDesc.SampleDesc.Quality = 0;
	_tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �ĸ� ���ۿ� ������ �Ұ�
	_tDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // ���� �ĸ� ���� 2��
	_tDesc.OutputWindow = m_WindowInfo.Hwnd;	  // ��� ������ ����
	_tDesc.Windowed = m_WindowInfo.Windowed;	  // â ���
	_tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
	_tDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT _hr = m_pDxgi->CreateSwapChain(m_pCmdQueue.Get(), &_tDesc, m_pSwapChain.GetAddressOf());

	for (UINT i = 0;i < SWAP_CHAIN_BUFFER_COUNT;i++)
	{
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRTT[i]));
	}

	return _hr;
}

HRESULT Device::CreateDecriptionHeap()
{
	// View ��ü�� �ش� texture���� flag�� �ٸ��� ������ �־��µ�
	// �̸� descriptionheap���� ������
	m_iRTVHeapSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC _tDesc = {};
	_tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	_tDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	_tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	_tDesc.NodeMask = 0;

	m_pDevice->CreateDescriptorHeap(&_tDesc, IID_PPV_ARGS(&m_pRTV));

	D3D12_CPU_DESCRIPTOR_HANDLE	_heapBegin = m_pRTV->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0;i < SWAP_CHAIN_BUFFER_COUNT;i++)
	{
		m_pRTVHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(_heapBegin, i * m_iRTVHeapSize);
		m_pDevice->CreateRenderTargetView(m_pRTT[i].Get(), nullptr, m_pRTVHandle[i]);
	}

	return E_NOTIMPL;
}

HRESULT Device::CreateRTView()
{
	return S_OK;
}

HRESULT Device::CreateDSView()
{
	return S_OK;
}

HRESULT Device::CreateConstantBuffer()
{
	HRESULT _result = S_OK;
	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::TRANSFORM, sizeof(MatrixInfo), 1)))
	{
		_result = E_FAIL;
	}

	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::MATERIAL, sizeof(MaterialInfo), 1)))
	{
		_result = E_FAIL;
	}

	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::ANIMATION, sizeof(AnimationInfo), 1)))
	{
		_result = E_FAIL;
	}

	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::GLOBAL, sizeof(GlobalDataInfo), 1)))
	{
		_result = E_FAIL;
	}

	return _result;
}

HRESULT Device::CreateConstantBufferIndividual(CONSTANT_TYPE _type, UINT _elementSize, UINT _elementCount)
{
	m_pConstantBuffer[(UINT)_type] = new ConstantBuffer();
	HRESULT _result = m_pConstantBuffer[(UINT)_type]->Create(_elementSize, _elementCount, _type);

	return _result;
}

void Device::CreateViewPort(Vec2 _pos, Vec2 _scale)
{
	D3D12_VIEWPORT _vDesc = {};

	_vDesc.MinDepth = 0.f;
	_vDesc.MaxDepth = 1.f;

	_vDesc.TopLeftX = _pos.x;
	_vDesc.TopLeftY = _pos.y;
	_vDesc.Width = _scale.x;
	_vDesc.Height = _scale.y;

	D3D12_RECT _vRect = {};
	_vRect.left = 0;
	_vRect.right = (int)_scale.x;
	_vRect.top = 0;
	_vRect.bottom = (int)_scale.y;

	m_pCmdList->RSSetViewports(1, &_vDesc);
	m_pCmdList->RSSetScissorRects(1, &_vRect);
}

void Device::OMSetRT()
{
	m_pCmdList->OMSetRenderTargets(1, &m_pRTVHandle[m_iBackBufferIndex], FALSE, nullptr);
}

void Device::ClearRenderTarget(float(&Color)[4])
{
	D3D12_CPU_DESCRIPTOR_HANDLE	backBufferView = m_pRTVHandle[m_iBackBufferIndex];
	m_pCmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
}

void Device::WaitSync()
{
	m_iFenceVal++;

	m_pCmdQueue->Signal(m_pFence.Get(), m_iFenceVal);

	if (m_pFence->GetCompletedValue() < m_iFenceVal)
	{
		m_pFence->SetEventOnCompletion(m_iFenceVal, m_pFenceEvent);

		::WaitForSingleObject(m_pFenceEvent, INFINITE);
	}
}

void Device::RenderBegin()
{
	m_pCmdAlloc->Reset();
	m_pCmdList->Reset(m_pCmdAlloc.Get(), nullptr);

	D3D12_RESOURCE_BARRIER	_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pRTT[m_iBackBufferIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, //before
		D3D12_RESOURCE_STATE_RENDER_TARGET); // after

	m_pCmdList->ResourceBarrier(1, &_barrier);

	CreateViewPort(Vec2(0.f, 0.f), m_WindowInfo.Res);

	float _fClear[4] = { 0.f,0.f,0.f,1.f };
	ClearRenderTarget(_fClear);
	OMSetRT();
}

void Device::RenderEnd()
{
	D3D12_RESOURCE_BARRIER	_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pRTT[m_iBackBufferIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	m_pCmdList->ResourceBarrier(1, &_barrier);
	m_pCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { m_pCmdList.Get() };
	m_pCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	Present();
	WaitSync();
	SwapIndex();
}

void Device::Present()
{
	m_pSwapChain->Present(0, 0);
}

void Device::SwapIndex()
{
	m_iBackBufferIndex = (m_iBackBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

HRESULT Device::CreateBlendState()
{
	return S_OK;
}

HRESULT Device::CreateSamplerState()
{
	return S_OK;
}

void Device::SetSamplerState()
{
}