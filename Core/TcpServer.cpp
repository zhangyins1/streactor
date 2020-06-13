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

void CTcpServer::closeConnection(const TcpConnectionPtr_t& c)
{
	c->handleClose();
}

void CTcpServer::OnNewConnection(int fd)
{
	TcpConnectionPtr_t conn(new CTcpConnection(m_loop, fd));
	if (!AddConnection(conn)) {
		return;
	}
	conn->setConnectionCallBack(connectionCallBack_);
	conn->setMessageCallBack(messageCallBack_);
	conn->setWriteCompleteCallBack(writeCompleteCallBack_);
	conn->setCloseCallBack(std::bind(&CTcpServer::OnConnectionClose, this, std::placeholders::_1));

	CSocket s(fd);
	s.SetNonBlock(true);

	conn->connectEstablished();
	if (connectionCallBack_)
		connectionCallBack_(conn);
}

void CTcpServer::OnConnectionClose(const TcpConnectionPtr_t& c)
{
	RemoveConnetion(c);
	if (closeCallBack_)
		closeCallBack_(c);

	m_loop->addPendingFunctor(std::bind(&CTcpConnection::connectDestroyed, c));
}

bool CTcpServer::AddConnection(const TcpConnectionPtr_t& c)
{
	int fd = c->fd();
	auto it = m_connections.find(fd);
	if (it != m_connections.end())
		return false;
	m_connections[fd] = c;
	return true;
}

void CTcpServer::RemoveConnetion(const TcpConnectionPtr_t& c)
{
	m_connections.erase(c->fd());
}

void CTcpServer::HandleDailyResCleanUp(CEventLoop *loop)
{
	if (dailyResCleanUpCallBack_)
		dailyResCleanUpCallBack_();
}