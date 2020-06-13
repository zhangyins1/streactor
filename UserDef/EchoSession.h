#ifndef __ECHO_SESSION_H_H__
#define __ECHO_SESSION_H_H__

#include "../Core/TcpSession.h"

class CEchoSession : public CTcpSession
{
public: 
	CEchoSession(const TcpConnectionWeakPtr_t& c);
	~CEchoSession();
public:
	void OnRead(const TcpConnectionPtr_t& c, Buffer *inbuf) override;
};

#endif // !__ECHO_SESSION_H_H__
