#pragma once

template <typename T>
class Singleton
{
public:
	static std::unique_ptr<T> sInstance;
protected:
	Singleton()
	{

	}	
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	static bool StaticInit()
	{		
		if (false == sInstance->StaticInitialize())
			return false;
		return sInstance->Initialize();
	}
	virtual ~Singleton()
	{

	}
protected:
	virtual bool StaticInitialize() = 0;
public:
	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;
};

