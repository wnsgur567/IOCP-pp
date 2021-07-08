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
	// sInstance �� �ʱ�ȭ
	virtual bool StaticInitialize() = 0;
public:
	// class �ʱ�ȭ
	virtual bool Initialize() = 0;
	// class ����
	virtual void Finalize() = 0;
};

template <typename T>
class PSingleton : public Singleton<PSingleton<T>>
{
	using id_t = unsigned __int32;
protected:
	id_t m_id;

protected:
	PSingleton(id_t inID) : Singleton(), m_id(inID) {}
public:
	id_t GetID() const { return m_id; }
};
