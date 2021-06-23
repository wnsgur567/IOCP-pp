#pragma once

// �̱������� �����ϱ�
class PacketManager
{
public:
	enum class E_PacketState
	{
		Error = -1,
		Idle = 0,
		InComplete,		// �̿ϼ�, ������ �޴�(����) ��
		Completed,		// �ϼ�
	};
private:
	unsigned __int32 m_curRecvPacketID;		// �ֱٿ� ���� recv packet id
	unsigned __int32 m_newSendPacketID;		// ���� ���� send packet id
public:
	void Packing(const void* inData,const size_t inSize);	// ���� data �� ��Ŷid �� size�� �ο��ؼ� ��ŷ
	void UnPacking(void* outData,size_t* outSize);	
};
