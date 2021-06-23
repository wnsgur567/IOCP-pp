#pragma once

// 싱글톤으로 수정하기
class PacketManager
{
public:
	enum class E_PacketState
	{
		Error = -1,
		Idle = 0,
		InComplete,		// 미완성, 데이터 받는(전송) 중
		Completed,		// 완성
	};
private:
	unsigned __int32 m_curRecvPacketID;		// 최근에 받은 recv packet id
	unsigned __int32 m_newSendPacketID;		// 새로 만들 send packet id
public:
	void Packing(const void* inData,const size_t inSize);	// 받은 data 에 패킷id 와 size를 부여해서 패킹
	void UnPacking(void* outData,size_t* outSize);	
};
