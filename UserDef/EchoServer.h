#ifndef __ECHO_SERVER_H_H__
#define __ECHO_SERVER_H_H__
#include <string>
#include <map>
#include "../Core/TcpServer.h"

using namespace net;

class CEventLoop;
class CEchoSession;
class CEchoServer
{
public:
	using EchoSessionPtr_t = std::shared_ptr<CEchoSession>;
public:
	CEchoServer();
	~CEchoServer();
public:
	void	Init(CEventLoop *loop, const std::string& ip, unsigned short port);
	void	Start();
	void	Stop();
public:
	void	OnNewConnection(const TcpConnectionPtr_t& c);
	void	OnConnectionClose(const TcpConnectionPtr_t& c);
	void	HandleDailyResCleanUp() { }
private:
	void	addSession(const TcpConnectionPtr_t& c, const EchoSessionPtr_t& s) {
		m_sessions.insert(std::make_pair(c->fd(), s));
	}
	void	delSession(const TcpConnectionPtr_t& c) {
		m_sessions.erase(c->fd());
	}
private:
	net::CTcpServer				*m_server;
	std::map<int, EchoSessionPtr_t> m_sessions;
};

#endif
