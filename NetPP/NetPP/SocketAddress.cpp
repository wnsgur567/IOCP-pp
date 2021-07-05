#include "base.h"

sockaddr_in& SocketAddress::GetSockAddr()
{
	return m_sockaddr;
}
const sockaddr_in& SocketAddress::GetSockAddr() const
{
	return m_sockaddr;
}
int SocketAddress::GetAddrSize()
{
	return sizeof(m_sockaddr);
}
int SocketAddress::GetAddrSize() const
{
	return sizeof(m_sockaddr);
}

SocketAddress::SocketAddress()
{
	ZeroMemory(&m_sockaddr, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
}
// port �� htons ó���� ���� �ֱ� (���μ� ���� ó�� ����)
SocketAddress::SocketAddress(const u_long inAddress, const u_short inPort)
{
	ZeroMemory(&m_sockaddr, sizeof(m_sockaddr));
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = inPort;
	m_sockaddr.sin_addr.s_addr = inAddress;
}
SocketAddress::SocketAddress(const SocketAddress& inOtherAddr)
{
	ZeroMemory(&m_sockaddr, sizeof(m_sockaddr));
	m_sockaddr.sin_family = inOtherAddr.m_sockaddr.sin_family;
	m_sockaddr.sin_port = inOtherAddr.m_sockaddr.sin_port;
	m_sockaddr.sin_addr.s_addr = inOtherAddr.m_sockaddr.sin_addr.s_addr;
}

// addr �� port ���
// ip �ּҺο� %s
// port �� %d �� �־� format �����
void SocketAddress::Print(const char* format) const
{
	char ip_buf[20];
	PCSTR retval = inet_ntop(m_sockaddr.sin_family, &m_sockaddr.sin_addr, ip_buf, 20);
	if (retval == nullptr)
		return;

	printf(format, ip_buf, ntohs(m_sockaddr.sin_port));
}