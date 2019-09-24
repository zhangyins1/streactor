#include "TcpSession.h"

CTcpSession::CTcpSession(CTcpConnection *conn) : m_connection(conn), 
	m_connState(conn ? ConnectionState::valid : ConnectionState::invalid)
{
	static st_sessionid_t sessionId_Alloc = 1;
	m_id = sessionId_Alloc++;
}

CTcpSession::~CTcpSession()
{

}

void CTcpSession::OnRead(CTcpConnection *conn, Buffer *inbuf)
{

}

void CTcpSession::OnClose(CTcpConnection *conn)
{

}

void CTcpSession::OnWriteComplete(CTcpConnection *conn)
{

}

void CTcpSession::OnConnectDestroyed()
{
	if (sessionCloseCallBack_)
		sessionCloseCallBack_(this);
	m_connection = nullptr;
	m_connState = ConnectionState::invalid;
}

void CTcpSession::Send(const char *buf, int len)
{
	if (buf && len > 0) {
		if (m_connState == ConnectionState::valid && m_connection)
			m_connection->send(buf, len);
	}
}

