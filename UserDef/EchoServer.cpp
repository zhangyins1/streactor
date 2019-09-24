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

void CEchoServer::OnNewConnection(net::CTcpConnection *c)
{
	CEchoSession * session = new CEchoSession(c);
	session->setSessionCloseCallBack(std::bind(&CEchoServer::OnConnectionClose, this, std::placeholders::_1));
	this->addSession(session);
	m_server->BindSession(session, c);
}

void CEchoServer::OnConnectionClose(CTcpSession *session)
{
	delSession((CEchoSession*)session);
	if (!is_int_in_vec((CEchoSession*)session, m_needDelSessions))
		m_needDelSessions.push_back((CEchoSession*)session);
}

void CEchoServer::HandleDailyResCleanUp()
{
	size_t len = m_needDelSessions.size();
	if (len) {
		for (size_t i = 0; i < len; ++i)
		{
			CEchoSession *s = m_needDelSessions.back();
			m_needDelSessions.pop_back();
			SAFE_DEL(s);
		}
	}
	if (len > 0) {
#if IS_DEBUG
	_LOG_INFO("CEchoServer::HandleDailyResCleanUp", "clean %d sessions.", len);
#endif
	}
}

void CEchoServer::addSession(CEchoSession *s)
{
	if (!is_int_in_vec(s, m_sessions))
		m_sessions.push_back(s);
}

void CEchoServer::delSession(CEchoSession *s)
{
	auto it = std::find(m_sessions.begin(), m_sessions.end(), s);
	if (it != m_sessions.end())
		m_sessions.erase(it);
}