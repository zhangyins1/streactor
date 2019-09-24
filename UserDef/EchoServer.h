#ifndef __ECHO_SERVER_H_H__
#define __ECHO_SERVER_H_H__
#include <string>
#include <vector>
#include "../Core/TcpServer.h"

class CEventLoop;
class CEchoSession;
class CEchoServer
{
public:
	CEchoServer();
	~CEchoServer();
public:
	void	Init(CEventLoop *loop, const std::string& ip, unsigned short port);
	void	Start();
	void	Stop();
public:
	void	OnNewConnection(net::CTcpConnection *c);
	void	OnConnectionClose(CTcpSession *session);
	void	HandleDailyResCleanUp();
private:
	void	addSession(CEchoSession *s);
	void	delSession(CEchoSession *s);
private:
	net::CTcpServer				*m_server;
	std::vector<CEchoSession*>	m_sessions;
	std::vector<CEchoSession*>  m_needDelSessions;
};

#endif
