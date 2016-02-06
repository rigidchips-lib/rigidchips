// GSocket class

#include <winsock2.h>
#include <string>
using namespace std;

class GSocket {
private:
	WSADATA m_wsadata;
	SOCKET  m_socket;
	SOCKET  m_socket_client;

public:
	GSocket();
	virtual ~GSocket();

protected:
	int Init();
	int Term();
	int Bind(u_short family, char *data, int namelen);
	int Listen(int backlog = 3);
	int Accept(u_short *family, char *data, int *addrlen);
	int Connect(u_short family, char *data, int namelen);
	int Open(int af, int type, int protocol);
	int Close();
	int Shutdown(int how);

public:
	int Send(const char *buf, int len, int flags);
	int Recv(char *buf, int len, int flags);
};

class HttpSocket : public GSocket {
public:
	HttpSocket();
	virtual ~HttpSocket();

public:
	int Init();
	int Term();
	int Connect(string servername);
	int Shutdown();
};
