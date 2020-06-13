#include "EchoServer.h"
#include "EchoSession.h"
#include "../Util/Misc.h"
#include "../Util/Log.h"

using namespace net;
CEchoServer::CEchoServer()
{

}

CEchoServer::~CEchoServer()
{

}

void CEchoServer::Init(CEventLoop *loop, const std::string& ip, unsigned short port)
{
	m_server = new CTcpServer(loop, ip, port);
	m_server->setConnectionCallBack(std::bind(&CEchoServer::OnNewConnection, this, std::placeholders::_1));
	m_server->setDailyCleanUpCallBack(std::bind(&CEchoServer::HandleDailyResCleanUp, this));
}

void CEchoServer::Start()
{
	m_server->start();
}

void CEchoServer::Stop()
{
	m_server->stop();
}

void CEchoServer::OnNewConnection(const TcpConnectionPtr_t& c)
{
	if (c->isConnected()) {
		std::shared_ptr<CEchoSession> pSession(new CEchoSession(c));
		this->addSession(c, pSession);
		_LOG_INFO("CEchoServer::OnNewConnection", "sessions.size: %d", m_sessions.size());
		c->setMessageCallBack(std::bind(&CEchoSession::OnRead, pSession.get(), std::placeholders::_1, std::placeholders::_2));
	}
	else {
		OnConnectionClose(c);
	}
}

void CEchoServer::OnConnectionClose(const TcpConnectionPtr_t& c)
{
	delSession(c);
	_LOG_INFO("CEchoServer::OnConnectionClose", "sessions.size: %d", m_sessions.size());
}
