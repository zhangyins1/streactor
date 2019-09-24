#ifndef __ECHO_SESSION_H_H__
#define __ECHO_SESSION_H_H__

#include "../Core/TcpSession.h"

class CEchoSession : public CTcpSession
{
public: 
	CEchoSession(CTcpConnection *conn);
	~CEchoSession();
public:
	void OnRead(CTcpConnection *conn, net::Buffer *inbuf) override;
};

#endif // !__ECHO_SESSION_H_H__
