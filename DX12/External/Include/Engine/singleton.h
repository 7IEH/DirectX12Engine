#pragma once

template<typename T>
class Singleton
{
private:
	static T* m_pThis;
	
public:
	static T* GetInst()
	{
		if (nullptr == m_pThis)
		{
			m_pThis = new T;
		}
		return m_pThis;
	}

	static void Destroy()
	{
		if (nullptr != m_pThis)
		{
			delete m_pThis;
			m_pThis = nullptr;
		}
	}

public:
	Singleton()
	{
		typedef void(*FUNC_TYPE)(void);
		atexit((FUNC_TYPE)(&Singleton::Destroy));
	}
	
	~Singleton()
	{

	}
};

template<typename T>
T* Singleton<T>::m_pThis = nullptr;