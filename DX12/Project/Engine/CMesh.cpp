#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CAssetMgr.h"

#include "CFBXLoader.h"

Mesh::Mesh()
	:Asset(ASSET_TYPE::MESH)
	, m_VertexInfo(nullptr)
	, m_VBView{}
	, m_IBInfo{}
{
}

Mesh::~Mesh()
{
	if (nullptr != m_VertexInfo)
		delete m_VertexInfo;

	if (!m_IBInfo.empty())
	{
		for (size_t i = 0;i < m_IBInfo.size();i++)
		{
			if (nullptr != m_IBInfo[i].IndexInfo)
				delete m_IBInfo[i].IndexInfo;
		}
	}

	if (nullptr != m_pOffsetBuffer)
		delete m_pOffsetBuffer;

	ReleaseVector(m_pFrameBuffer);
}

void Mesh::Create(vector<VertexInfo>& _VBdata, UINT _VertexCount, vector<UINT>& _IBData, UINT _IndexCount)
{
	CreateBuffer(BUFFER_TYPE::VERTEX, _VertexCount, _VBdata, _IBData);
	CreateBuffer(BUFFER_TYPE::INDEX, _IndexCount, _VBdata, _IBData);

	// 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
	m_VertexInfo = new VertexInfo[_VertexCount];
	memcpy(m_VertexInfo, &_VBdata[0], sizeof(VertexInfo) * _VertexCount);
}

void Mesh::UpdateData(UINT _idx)
{
	SetBuffer(BUFFER_TYPE::VERTEX, _idx);
	SetBuffer(BUFFER_TYPE::INDEX, _idx);
}

Ptr<Mesh> Mesh::CreateFromFBX(const FBXMeshInfo* _meshInfo,FBXLoader& _loader)
{
	Ptr<Mesh> _mesh = new Mesh;
	vector<VertexInfo> _vertices = _meshInfo->vertices;
	vector<UINT> _indices;
	_mesh->CreateBuffer(BUFFER_TYPE::VERTEX, (UINT)_vertices.size(), _vertices, _indices);
	for (const vector<UINT>& _buffer : _meshInfo->indices)
	{
		if (_buffer.empty())
		{
			vector<UINT> _defaultBuffer{ 0 };
			_indices = _defaultBuffer;
		}
		else
		{
			_indices = _buffer;
		}

		_mesh->CreateBuffer(BUFFER_TYPE::INDEX, (UINT)_indices.size(), _vertices, _indices);
	}

	if (_meshInfo->hasAnimation)
	{
		_mesh->CreateBonesAndAnimation(_loader);
	}
	return _mesh;
}

void Mesh::Render(UINT _idx)
{
	Device::GetInst()->CommitTable();
	UpdateData(_idx);
	DrawIndexed(_idx);
}

void Mesh::CreateBuffer(BUFFER_TYPE _bufferType, UINT _count, vector<VertexInfo>& _vertexData, vector<UINT>& _indexData)
{
	D3D12_HEAP_PROPERTIES _heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	switch (_bufferType)
	{
	case BUFFER_TYPE::VERTEX:
	{
		D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexInfo) * _count);
		DEVICE->CreateCommittedResource(
			&_heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&_tDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_VB)
		);

		void* vertexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		m_VB->Map(0, &readRange, &vertexDataBuffer);
		::memcpy(vertexDataBuffer, &_vertexData[0], sizeof(VertexInfo) * _count);
		m_VB->Unmap(0, nullptr);

		m_VBView.BufferLocation = m_VB->GetGPUVirtualAddress();
		m_VBView.StrideInBytes = sizeof(VertexInfo);
		m_VBView.SizeInBytes = sizeof(VertexInfo) * _count;
	}
	break;
	case BUFFER_TYPE::INDEX:
	{
		IndexBufferInfo _temp;

		D3D12_RESOURCE_DESC _tDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT) * _count);
		DEVICE->CreateCommittedResource(
			&_heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&_tDesc,
			D3D12_RESOURCE_STATE_INDEX_BUFFER,
			nullptr,
			IID_PPV_ARGS(&_temp.IB)
		);

		void* IndexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		_temp.IB->Map(0, &readRange, &IndexDataBuffer);
		::memcpy(IndexDataBuffer, &_indexData[0], sizeof(UINT) * _count);
		_temp.IB->Unmap(0, nullptr);

		_temp.IBView.BufferLocation = _temp.IB->GetGPUVirtualAddress();
		_temp.IBView.Format = DXGI_FORMAT_R32_UINT;
		_temp.IBView.SizeInBytes = sizeof(UINT) * _count;

		_temp.format = _temp.IBView.Format;
		_temp.count = _count;
		_temp.IndexInfo = new UINT[_count];
		memcpy(_temp.IndexInfo, &_indexData[0], sizeof(UINT) * _count);
		m_IBInfo.push_back(_temp);
	}
	break;
	case BUFFER_TYPE::END:
		break;
	default:
		break;
	}
}

void Mesh::DrawIndexed(UINT _idx)
{
	CMDLIST->DrawIndexedInstanced(m_IBInfo[_idx].count, 1, 0, 0, 0);
}

void Mesh::Render_Instancing(UINT _particleCount)
{
}

void Mesh::SetBuffer(BUFFER_TYPE _bufferType, UINT _idx)
{
	switch (_bufferType)
	{
	case BUFFER_TYPE::VERTEX:
	{
		CMDLIST->IASetVertexBuffers(0, 1, &m_VBView);
	}
	break;
	case BUFFER_TYPE::INDEX:
	{
		CMDLIST->IASetIndexBuffer(&m_IBInfo[_idx].IBView);
	}
	break;
	case BUFFER_TYPE::END:
		break;
	default:
		break;
	}
}



void Mesh::CreateBonesAndAnimation(FBXLoader& _loader)
{
	UINT _iFrameCount = 0;
	vector<std::shared_ptr<FBXAnimClipInfo>>& _animClips = _loader.GetAnimClip();
	for (std::shared_ptr<FBXAnimClipInfo>& _ac : _animClips)
	{
		AnimClipInfo _info = {};

		_info.animName = _ac->name;
		_info.duration = _ac->endTime.GetSecondDouble() - _ac->startTime.GetSecondDouble();

		int _startFrame = static_cast<int>(_ac->startTime.GetFrameCount(_ac->mode));
		int _endFrame = static_cast<int>(_ac->endTime.GetFrameCount(_ac->mode));
		_info.frameCount = _endFrame - _startFrame;

		_info.KeyFrame.resize(_ac->keyFrames.size());

		const int _boneCount = static_cast<int>(_ac->keyFrames.size());
		for (int b = 0;b < _boneCount;b++)
		{
			auto& _vec = _ac->keyFrames[b];

			const int _size = static_cast<int>(_vec.size());
			_iFrameCount = max(_iFrameCount, static_cast<UINT>(_size));
			_info.KeyFrame[b].resize(_size);

			for (int f = 0;f < _size;f++)
			{
				FBXKeyFrameInfo& _kf = _vec[f];
				KeyFrameInfo& _kfInfo = _info.KeyFrame[b][f];

				_kfInfo.time = _kf.time;
				_kfInfo.frame = static_cast<int>(_size);
				_kfInfo.scale.x = static_cast<float>(_kf.matTransform.GetS().mData[0]);
				_kfInfo.scale.y = static_cast<float>(_kf.matTransform.GetS().mData[1]);
				_kfInfo.scale.z = static_cast<float>(_kf.matTransform.GetS().mData[2]);
				_kfInfo.rotation.x = static_cast<float>(_kf.matTransform.GetQ().mData[0]);
				_kfInfo.rotation.y = static_cast<float>(_kf.matTransform.GetQ().mData[1]);
				_kfInfo.rotation.z = static_cast<float>(_kf.matTransform.GetQ().mData[2]);
				_kfInfo.rotation.w = static_cast<float>(_kf.matTransform.GetQ().mData[3]);
				_kfInfo.translate.x = static_cast<float>(_kf.matTransform.GetT().mData[0]);
				_kfInfo.translate.y = static_cast<float>(_kf.matTransform.GetT().mData[1]);
				_kfInfo.translate.z = static_cast<float>(_kf.matTransform.GetT().mData[2]);
			}
		}
		m_vAnimClips.push_back(_info);
	}

	vector<std::shared_ptr<FBXBoneInfo>>& _bones = _loader.GetBones();
	for (std::shared_ptr<FBXBoneInfo>& _bone : _bones)
	{
		BoneInfo _boneInfo = {};
		_boneInfo.parentIdx = _bone->parentIndex;
		_boneInfo.matOffset = GetMatrix(_bone->matOffset);
		_boneInfo.boneName = _bone->boneName;
		m_vBones.push_back(_boneInfo);
	}

	if (!m_vAnimClips.empty())
	{
		const int _boneCount = static_cast<int>(m_vBones.size());
		vector<Matrix> _offsetVec(_boneCount);
		for (size_t b = 0;b < _boneCount;b++)
			_offsetVec[b] = m_vBones[b].matOffset;

		m_pOffsetBuffer = new StructuredBuffer;
		m_pOffsetBuffer->Create(sizeof(Matrix), static_cast<UINT>(_offsetVec.size()), SB_TYPE::READ_WRITE,TRUE, _offsetVec.data());

		const int _animCount = static_cast<int>(_animClips.size());
		for (int i = 0;i < _animCount;i++)
		{
			AnimClipInfo& _animClip = m_vAnimClips[i];

			vector<AnimFrameParams> _frameParams;
			_frameParams.resize(m_vBones.size() * _animClip.frameCount);

			for (int b = 0;b < _boneCount;b++)
			{
				const int _KeyFrameCount = static_cast<int>(_animClip.KeyFrame[b].size());
				for (int f = 0;f < _KeyFrameCount;f++)
				{
					int _idx = static_cast<int>(_boneCount * f + b);

					_frameParams[_idx] = AnimFrameParams
					{
						Vec4(_animClip.KeyFrame[b][f].scale),
						_animClip.KeyFrame[b][f].rotation,
						Vec4(_animClip.KeyFrame[b][f].translate)
					};
				}
			}
			m_pFrameBuffer.push_back(new StructuredBuffer);
			m_pFrameBuffer.back()->Create(sizeof(AnimFrameParams), static_cast<UINT>(_frameParams.size()), SB_TYPE::READ_WRITE, TRUE, _frameParams.data());
		}
	}
}

Matrix Mesh::GetMatrix(FbxAMatrix& _matrix)
{
	Matrix _mat;

	for (int y = 0;y < 4;y++)
	{
		for (int x = 0;x < 4;x++)
		{
			_mat.m[y][x] = static_cast<float>(_matrix.Get(y, x));
		}
	}

	return _mat;
}
