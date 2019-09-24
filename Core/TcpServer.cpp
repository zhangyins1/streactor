#include "TcpServer.h"
#include "TcpConnection.h"
#include "TcpSession.h"
#include "Listener.h"
#include "../Util/Misc.h"
#include "../Util/Log.h"

using namespace net;
CTcpServer::CTcpServer(CEventLoop *loop, const std::string& ip, st_port_t port) : 
	m_loop(loop), m_listener(new CListener(loop, ip, port))
{
	m_listener->setNewConnectionCallBack(std::bind(&CTcpServer::OnNewConnection, this, std::placeholders::_1));
	m_loop->setRuntimeCallBack(std::bind(&CTcpServer::HandleDailyResCleanUp, this, std::placeholders::_1));
}

CTcpServer::~CTcpServer()
{

}

void CTcpServer::start()
{
	m_stoped = false;
	m_listener->StartListening();
}

void CTcpServer::stop()
{
	m_stoped = true;
	if (m_loop)
		m_loop->stop();
}

void CTcpServer::closeConnection(CTcpConnection *c)
{
	c->handleClose();
}

void CTcpServer::OnNewConnection(int fd)
{
	CTcpConnection *conn = new CTcpConnection(m_loop, fd);
	if (!AddConnection(conn)) {
		return;
	}
	conn->setCloseCallBack(std::bind(&CTcpServer::OnConnectionClose, this, std::placeholders::_1));

	CSocket s(fd);
	s.SetNonBlock(true);

	conn->connectEstablished();
	if (connectionCallBack_)
		connectionCallBack_(conn);
}

void CTcpServer::OnConnectionClose(CTcpConnection *c)
{
	RemoveConnetion(c);
	if (std::find(m_needDelConnections.begin(), m_needDelConnections.end(), c) == m_needDelConnections.end())
		m_needDelConnections.push_back(c);
}

bool CTcpServer::AddConnection(CTcpConnection *c)
{
	int fd = c->fd();
	auto it = m_connections.find(fd);
	if (it != m_connections.end())
		return false;
	m_connections[fd] = c;
	return true;
}

void CTcpServer::RemoveConnetion(CTcpConnection *c)
{
	int fd = c->fd();
	auto it = m_connections.find(fd);
	if (it != m_connections.end())
	{
		if (it->second == c)
			m_connections.erase(it);
	}
}

void CTcpServer::BindSession(CTcpSession *session, CTcpConnection *c)
{
	c->setMessageCallBack(std::bind(&CTcpSession::OnRead, session, std::placeholders::_1, std::placeholders::_2));
	c->setWriteCompleteCallBack(std::bind(&CTcpSession::OnWriteComplete, session, std::placeholders::_1));
	c->addSession(session);
}

void CTcpServer::HandleDailyResCleanUp(CEventLoop *loop)
{
	if (dailyResCleanUpCallBack_)
		dailyResCleanUpCallBack_();

	size_t len = m_needDelConnections.size();
	if (len) {
		for (size_t i = 0; i < len; ++i)
		{
			CTcpConnection *c = m_needDelConnections.back();
			m_needDelConnections.pop_back();
			SAFE_DEL(c);
		}
	}
	if (len > 0) {
#if IS_DEBUG
	_LOG_INFO("CTcpServer::HandleDailyResCleanUp", "clean %d conns.", len);
#endif
	}

}