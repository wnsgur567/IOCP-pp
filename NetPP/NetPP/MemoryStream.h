#pragma once

// recv buffer 용
class InputMemoryStream
{
private:
	char* m_buffer;				// 버퍼
	bool  m_isBufferOwner;		// 버퍼의 소유권이 있을 경우에만 해제하도록								

	size_t m_head;				// 읽기 시작할 현제 head 의 위치
	size_t m_capacity;			// 최대 용량
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


// send buffer 용
class OutputMemoryStream
{
private:
	char* m_buffer;				// 버퍼
	size_t m_head;				// 쓰기 시작할 현제 head의 위치
	size_t m_capacity;			// 최대 용량

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
