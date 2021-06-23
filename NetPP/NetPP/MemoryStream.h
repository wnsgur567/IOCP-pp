#pragma once

// recv buffer ��
class InputMemoryStream
{
private:
	char* m_buffer;				// ����
	bool  m_isBufferOwner;		// ������ �������� ���� ��쿡�� �����ϵ���								

	size_t m_head;				// �б� ������ ���� head �� ��ġ
	size_t m_capacity;			// �ִ� �뷮
public:
	InputMemoryStream(char* inBuffer, size_t inByteCount, bool inIsOwner);
	InputMemoryStream(const InputMemoryStream& inOther);
	InputMemoryStream& operator=(const InputMemoryStream& inOther);
	~InputMemoryStream();
public:
	size_t GetRemainDataSize() const;
	void Read(void* outData, size_t inByteCount);
	template<typename T> void Read(T& outData);
};

template<typename T>
inline void InputMemoryStream::Read(T& outData)
{
	static_assert(
		std::is_arithmetic<T>::value ||
		std::is_enum<T>::value,
		"Generi Write only supports primitive data type"
		);

	Read(&outData, sizeof(T));
}


// send buffer ��
class OutputMemoryStream
{
private:
	char* m_buffer;				// ����
	size_t m_head;				// ���� ������ ���� head�� ��ġ
	size_t m_capacity;			// �ִ� �뷮

	void ReAllocBuffer(size_t inNewLength);
public:
	OutputMemoryStream();
	OutputMemoryStream(const OutputMemoryStream& inOther);
	OutputMemoryStream& operator=(const OutputMemoryStream& inOther);
	~OutputMemoryStream();
public:
	const char* GetBufferPtr();
	size_t GetLength() const;

	void Write(const void* inData, size_t inByteCount);
	template<typename T> void Write(const T& inData);
	void Wirte(const std::string& inString);
};

// only primitive type
template<typename T>
inline void OutputMemoryStream::Write(const T& inData)
{
	static_assert(
		std::is_arithmetic<T>::value ||
		std::is_enum<T>::value,
		"Generi Write only supports primitive data type"
		);

	Write(&inData, sizeof(inData));
}
