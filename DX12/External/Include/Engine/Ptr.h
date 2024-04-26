#pragma once

template <typename T>
class Ptr
{
private:
	T* m_Asset;

public:
	T* Get()const { return m_Asset; }
	T** GetAddressOf()const { return &m_Asset; }
	T* operator->()const { return m_Asset; }

public:
	void operator = (const Ptr& _ptr)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _ptr.m_Asset;

		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	void operator = (T* _Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _Asset;

		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	bool operator ==(const Ptr<T>& _Other) const
	{
		if (m_Asset == _Other.Get())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator != (const Ptr<T>& _Other) const
	{
		return !(*this == _Other);
	}

	bool operator ==(const T* _Asset) const
	{
		if (m_Asset == _Asset)
			return true;
		else
			return false;
	}

	bool operator != (const T* _Asset) const
	{
		return !(*this == _Asset);
	}

public:
	Ptr()
		:m_Asset(nullptr)
	{

	}

	// ASSET�� �߰��ϸ鼭 ������ �����Ͱ� ASSET�� ����Ű�� �����Ƿ�
	// REFCOUNT 1����
	Ptr(const Ptr<T>& _other)
		:m_Asset(_other.m_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	// ASSET�� �߰��ϸ鼭 ������ �����Ͱ� ASSET�� ����Ű�� �����Ƿ�
	// REFCOUNT 1����
	Ptr(T* _asset)
		:m_Asset(_asset)
	{
		if (nullptr != m_Asset)
		{
			m_Asset->AddRef();
		}
	}

	// �ش� ����Ʈ �����͸� RELEASE�Ҷ��� �翬�� �̸� ����Ű�� �ִ� ���� ��ü ���� ������� ����
	~Ptr()
	{
		if (m_Asset)
		{
			m_Asset->Release();
		}
	}
};

template<typename T>
bool operator == (const void* _Asset, const Ptr<T>& _Ptr)
{
	if (_Asset == _Ptr.Get())
		return true;
	else
		return false;
}

template<typename T>
bool operator != (const void* _Asset, const Ptr<T>& _Ptr)
{
	return !(_Asset == _Ptr);
}