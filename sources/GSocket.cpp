// GSocket class


//#include <iostream>
#include "GSocket.hpp"
using namespace std;

// GSocket class
GSocket::GSocket() 
{
	Init();
}

GSocket::~GSocket()
{
	Close();
	Term();
//	cerr << "termination" << endl;
}

int GSocket::Init()
{
	int ret = ::WSAStartup(2, &m_wsadata);

//	if (ret != 0)
//		cerr << "WinSock initilization error: " << ret << endl;

	return ret;
}

int GSocket::Term()
{
	return ::WSACleanup();
}

int GSocket::Bind(u_short family,char *data, int namelen)
{
	struct sockaddr sa;
	sa.sa_family=family;
	memcpy(sa.sa_data,data,12);
	int ret = ::bind(m_socket, &sa, namelen);
	
//	if (ret == SOCKET_ERROR)
//		cerr << "Bind error: " << ::WSAGetLastError() << endl;
	
	return ret;
}

int GSocket::Listen(int backlog)
{
	int ret = ::listen(m_socket, backlog);
	
//	if (ret == SOCKET_ERROR)
//		cerr << "Listen error: " << ::WSAGetLastError() << endl;
	
	return ret;
}

int GSocket::Accept(u_short *family, char *data, int *addrlen)
{
	struct sockaddr sa;
	m_socket_client = ::accept(m_socket, &sa, addrlen);
	
	if (m_socket_client == INVALID_SOCKET) {
//		cerr << "Accept error: " << ::WSAGetLastError() << endl;
		return -1;
	}
	*family=sa.sa_family;
	memcpy(data,sa.sa_data,12);
	
	return 0;
}

int GSocket::Connect(u_short family, char *data, int namelen)
{
	struct sockaddr sa;
	sa.sa_family=family;
	memcpy(sa.sa_data,data,12);
	int ret = ::connect(m_socket, &sa, namelen);
	
//	if (ret == SOCKET_ERROR)
//		cerr << "Connect error: " << ::WSAGetLastError() << endl;
	
	return ret;
}

int GSocket::Open(int af, int type, int protocol)
{
	m_socket = ::socket(af, type, protocol);
	
	if (m_socket == INVALID_SOCKET) {
//		cerr << "WinSock socket error: " << ::WSAGetLastError() << endl;
		return -1;
	}
	
	return 0;
}

int GSocket::Close()
{
	return ::closesocket(m_socket);
}

int GSocket::Shutdown(int how)
{
	return ::shutdown(m_socket, how);
}

int GSocket::Send(const char *buf, int len, int flags)
{
	int size = ::send(m_socket, buf, len, flags);
	
//	if (size == SOCKET_ERROR) cerr << "send error: " << ::WSAGetLastError() << endl;
	
	return size;
}

int GSocket::Recv(char *buf, int len, int flags)
{
	int size = ::recv(m_socket, buf, len, flags);
	
//	if (size == SOCKET_ERROR) cerr << "recv error: " << ::WSAGetLastError() << endl;
	
	return size;
}

// HttpSocket class
HttpSocket::HttpSocket()
{
	Init();
}

HttpSocket::~HttpSocket()
{
}

int HttpSocket::Init()
{
	int ret = GSocket::Init();

	Open(AF_INET, SOCK_STREAM, 0);

	return ret;
}

int HttpSocket::Term()
{
	GSocket::Close();
	return GSocket::Term();
}

int HttpSocket::Connect(string servername)
{
	HOSTENT *host = ::gethostbyname(servername.data());
	if (host == NULL) {
//		cerr << "gethostbyname error: " << ::WSAGetLastError() << endl;
		return -1;
	}

	IN_ADDR in_addr = *((IN_ADDR*)*host->h_addr_list);

	SOCKADDR_IN sockaddr_in;
	::memset(&sockaddr_in, 0, sizeof(sockaddr_in));

	sockaddr_in.sin_family = AF_INET;
	sockaddr_in.sin_port = ::htons(80);
	sockaddr_in.sin_addr = in_addr;

	return GSocket::Connect(sockaddr_in.sin_family,(char*)(&sockaddr_in.sin_port), sizeof(sockaddr_in));
}

int HttpSocket::Shutdown()
{
	return GSocket::Shutdown(SD_BOTH);
}
