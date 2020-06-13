#include "TcpSession.h"

CTcpSession::CTcpSession(const TcpConnectionWeakPtr_t& conn) : m_connection(conn)
{
	static st_sessionid_t sessionId_Alloc = 1;
	m_id = sessionId_Alloc++;
}

CTcpSession::~CTcpSession()
{

}

void CTcpSession::OnRead(const TcpConnectionPtr_t& c, Buffer *inbuf)
{
}

void CTcpSession::OnClose(const TcpConnectionPtr_t& c)
{
	m_connection.reset();
}

void CTcpSession::OnWriteComplete(const TcpConnectionPtr_t& c)
{

}

void CTcpSession::OnConnectDestroyed()
{
	
}

void CTcpSession::Send(const char *buf, int len)
{
	if (buf && len > 0) {
		auto c = getConnectionPtr();
		if (c)
			c->send(buf, len);
	}
}

