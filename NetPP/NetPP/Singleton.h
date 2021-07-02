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
		sInstance.reset(new T());
		return sInstance->Init();
	}
	virtual Init() = 0;
	virtual ~Singleton()
	{

	}
};