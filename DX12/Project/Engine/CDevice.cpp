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
	, m_pRTVHandle{}
	, m_iHandleSize(0)
	, m_iGroupSize(0)
	, m_iGroupCount(0)
	, m_iCurGroupIdx(0)
	, m_Sampler{}
	, m_DSVHandle{}
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
	// IID_PPV_ARGS -> ID와 포인터를 가져옴
	::D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugContoller));
	m_pDebugContoller->EnableDebugLayer();
#endif

	/****************
	| DX12 Initialize
	****************/
	CreateDevice();

	CreateCommandQueue();

	CreateSwapChain();

	CreateDecriptionHeap();

	CreateBlendState();

	CreateSamplerState();

	if (FAILED(CreateRootSignature()))
	{
		HandleError(L"Failed Create RootSignature", 0);
	}

	if (FAILED(CreateConstantBuffer()))
	{
		HandleError(L"Failed Create ConstantBuffer", 0);
	}

	CreateTableDescriptorHeap(256);

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

	// D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	_hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAlloc));

	// GPU가 하나인 시스템에서는 0으로
	// DIREC or BUNDLE
	// Allocator
	// 초기 상태 ( 그리기 명령은 nullptr  지정)
	// m_pCmdAlloc-> list에서 명령들을 보내면 해당 메모리 자리를 없애는게 아니라 비워두고 관리하는데 이 때 관리자 역할을 함
	_hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&m_pCmdList));


	// CommandList는 Close/open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	_hr = m_pCmdList->Close();

	_hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pResCmdAlloc));
	_hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pResCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&m_pResCmdList));

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
	_tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 후면 버퍼에 렌더링 할것
	_tDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 전면 후면 버퍼 2개
	_tDesc.OutputWindow = m_WindowInfo.Hwnd;	  // 출력 윈도우 지정
	_tDesc.Windowed = m_WindowInfo.Windowed;	  // 창 모드
	_tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	_tDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT _hr = m_pDxgi->CreateSwapChain(m_pCmdQueue.Get(), &_tDesc, m_pSwapChain.GetAddressOf());

	for (UINT i = 0;i < SWAP_CHAIN_BUFFER_COUNT;i++)
	{
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRTT[i]));
	}

	return _hr;
}

void Device::CreateDecriptionHeap()
{
	// View 객체는 해당 texture마다 flag를 다르게 가지고 있었는데
	// 이를 descriptionheap으로 통일함
	CreateRTView();
	CreateDSView();
}

HRESULT Device::CreateRootSignature()
{
	HRESULT _hr = E_FAIL;

	CD3DX12_DESCRIPTOR_RANGE _ranges[] =
	{
		// Descriptor heap의 용도, 몇 개 사용할 지, 시작할 값
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,(UINT)CONSTANT_TYPE::END - 1,1),
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,(UINT)TEX_PARAM::END,0),
	};

	CD3DX12_ROOT_PARAMETER _param[2] = {};
	_param[0].InitAsConstantBufferView(0);
	_param[1].InitAsDescriptorTable(_countof(_ranges), _ranges);

	D3D12_ROOT_SIGNATURE_DESC _tDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(_param), _param, 1, &m_Sampler[0]);
	_tDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> _blobSignature;
	ComPtr<ID3DBlob> _blobError;
	::D3D12SerializeRootSignature(&_tDesc, D3D_ROOT_SIGNATURE_VERSION_1, &_blobSignature, &_blobError);
	_hr = m_pDevice->CreateRootSignature(0, _blobSignature->GetBufferPointer(), _blobSignature->GetBufferSize(), IID_PPV_ARGS(&m_pSignature));

	return _hr;
}

void Device::CreateTableDescriptorHeap(UINT _count)
{
	m_iGroupCount = _count;

	UINT64 _iRegisterNum = ((UINT64)(CONSTANT_TYPE::END)-(UINT64)1) + (UINT64)TEX_PARAM::END;
	D3D12_DESCRIPTOR_HEAP_DESC _tDesc = {};
	_tDesc.NumDescriptors = _iRegisterNum * m_iGroupCount;
	_tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	_tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	DEVICE->CreateDescriptorHeap(&_tDesc, IID_PPV_ARGS(&m_pGPUHeap));

	m_iHandleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_iGroupSize = m_iHandleSize * _iRegisterNum;
}

void Device::CreateRTView()
{
	UINT _iRTVHeapSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC _tDesc = {};
	_tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	_tDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	_tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	_tDesc.NodeMask = 0;

	m_pDevice->CreateDescriptorHeap(&_tDesc, IID_PPV_ARGS(&m_pRTV));

	D3D12_CPU_DESCRIPTOR_HANDLE	_heapBegin = m_pRTV->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0;i < SWAP_CHAIN_BUFFER_COUNT;i++)
	{
		m_pRTVHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(_heapBegin, i * _iRTVHeapSize);
		m_pDevice->CreateRenderTargetView(m_pRTT[i].Get(), nullptr, m_pRTVHandle[i]);
	}
}

void Device::CreateDSView()
{
	D3D12_HEAP_PROPERTIES _property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D24_UNORM_S8_UINT
		, static_cast<UINT64>(m_WindowInfo.Res.x), static_cast<UINT64>(m_WindowInfo.Res.y));
	_tDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D24_UNORM_S8_UINT, 1.f, 0);

	DEVICE->CreateCommittedResource(&_property,
		D3D12_HEAP_FLAG_NONE,
		&_tDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&m_pDST));

	D3D12_DESCRIPTOR_HEAP_DESC _tDesc2 = {};
	_tDesc2.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	_tDesc2.NumDescriptors = 1;
	_tDesc2.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	DEVICE->CreateDescriptorHeap(&_tDesc2, IID_PPV_ARGS(&m_pDSV));

	m_DSVHandle = m_pDSV->GetCPUDescriptorHandleForHeapStart();
	DEVICE->CreateDepthStencilView(m_pDST.Get(), nullptr, m_DSVHandle);
}

HRESULT Device::CreateConstantBuffer()
{
	HRESULT _result = S_OK;
	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::GLOBAL, sizeof(GlobalDataInfo), 1)))
	{
		_result = E_FAIL;
	}

	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::TRANSFORM, sizeof(MatrixInfo), 256)))
	{
		_result = E_FAIL;
	}

	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::MATERIAL, sizeof(MaterialInfo), 256)))
	{
		_result = E_FAIL;
	}

	if (FAILED(CreateConstantBufferIndividual(CONSTANT_TYPE::ANIMATION, sizeof(AnimationInfo), 256)))
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
	m_pCmdList->OMSetRenderTargets(1, &m_pRTVHandle[m_iBackBufferIndex], FALSE, &m_DSVHandle);
}

void Device::ClearRenderTarget(float(&Color)[4])
{
	D3D12_CPU_DESCRIPTOR_HANDLE	backBufferView = m_pRTVHandle[m_iBackBufferIndex];
	m_pCmdList->ClearRenderTargetView(backBufferView, Colors::Black, 0, nullptr);
	m_pCmdList->ClearDepthStencilView(m_DSVHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, nullptr);
}

void Device::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, CONSTANT_TYPE _type)
{
	D3D12_CPU_DESCRIPTOR_HANDLE _destHandle = m_pGPUHeap->GetCPUDescriptorHandleForHeapStart();
	_destHandle.ptr += m_iCurGroupIdx * m_iGroupSize;
	_destHandle.ptr += ((UINT)_type - 1) * m_iHandleSize;

	UINT _iDestRange = 1;
	UINT _iSrcRange = 1;
	DEVICE->CopyDescriptors(1, &_destHandle, &_iDestRange, 1, &_srcHandle, &_iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Device::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE _srcHandle, TEX_PARAM _type)
{
	D3D12_CPU_DESCRIPTOR_HANDLE _destHandle = m_pGPUHeap->GetCPUDescriptorHandleForHeapStart();
	_destHandle.ptr += m_iCurGroupIdx * m_iGroupSize;
	_destHandle.ptr += (((UINT)CONSTANT_TYPE::END - 1) + (UINT)_type) * m_iHandleSize;

	UINT _iDestRange = 1;
	UINT _iSrcRange = 1;
	DEVICE->CopyDescriptors(1, &_destHandle, &_iDestRange, 1, &_srcHandle, &_iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Device::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE	_handle = m_pGPUHeap->GetGPUDescriptorHandleForHeapStart();
	_handle.ptr += m_iCurGroupIdx * m_iGroupSize;
	CMDLIST->SetGraphicsRootDescriptorTable(1, _handle);

	m_iCurGroupIdx++;
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

	m_pCmdList->SetGraphicsRootSignature(m_pSignature.Get());
	ClearConstantBuffer();
	TableClear();

	CMDLIST->SetDescriptorHeaps(1, m_pGPUHeap.GetAddressOf());

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
	m_Sampler[(UINT)SAMPLER_TYPE::Default] = CD3DX12_STATIC_SAMPLER_DESC(0);
	return S_OK;
}

void Device::ClearConstantBuffer()
{
	for (UINT i = 0;i < (UINT)CONSTANT_TYPE::END;i++)
	{
		if (nullptr != m_pConstantBuffer[i])
			m_pConstantBuffer[i]->Clear();
	}
}

void Device::FlushResrouceCommandQueue()
{
	m_pResCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { m_pResCmdList.Get() };
	m_pCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	m_pResCmdAlloc->Reset();
	m_pResCmdList->Reset(m_pResCmdAlloc.Get(), nullptr);
}