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

	// ASSET을 추가하면서 생성한 포인터가 ASSET을 가리키고 있으므로
	// REFCOUNT 1증가
	Ptr(const Ptr<T>& _other)
		:m_Asset(_other.m_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	// ASSET을 추가하면서 생성한 포인터가 ASSET을 가리키고 있으므로
	// REFCOUNT 1증가
	Ptr(T* _asset)
		:m_Asset(_asset)
	{
		if (nullptr != m_Asset)
		{
			m_Asset->AddRef();
		}
	}

	// 해당 스마트 포인터를 RELEASE할때는 당연히 이를 가리키고 있던 원본 객체 역시 사라지게 만듬
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