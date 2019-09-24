#include "TcpConnection.h"
#include "../Event/Channel.h"
#include "TcpSession.h"
using namespace net;

CTcpConnection::CTcpConnection(CEventLoop *loop, int fd) : m_fd(fd), m_channel(new CChannel(loop, fd)), m_state(StateE::kConnecting)
{
	m_channel->setReadCallBack(std::bind(&CTcpConnection::handleRead, this));
	m_channel->setWriteCallBack(std::bind(&CTcpConnection::handleWrite, this));
	m_channel->setcloseCallBack(std::bind(&CTcpConnection::handleClose, this));
	m_channel->setErrorCallBack(std::bind(&CTcpConnection::handleError, this));
}

CTcpConnection::~CTcpConnection()
{
	if (m_outBuffer.readableBytes() > 0 && m_channel->hasWriteEvent())
		handleWrite();

	for (SessionMap::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it)
	{
		CTcpSession *session = it->second;
		if (session)
			session->OnConnectDestroyed();
	}

	delete m_channel;
	m_channel = nullptr;
	m_fd.Close();
	setState(StateE::kDisconnected);
}

void CTcpConnection::connectEstablished()
{
	m_channel->enableReading();
	setState(StateE::kConnected);
}

void CTcpConnection::send(const char* buf, int len)
{
	m_outBuffer.append(buf, len);
	if (!m_channel->hasWriteEvent())
		m_channel->enableWriting();
}

void CTcpConnection::handleRead()
{
	int savedErrno = 0;
	int n = m_inBuffer.readFd(m_fd.fd(), &savedErrno);
	
	if (n > 0) {
		if (messageCallBack_)
			messageCallBack_(this, &m_inBuffer);
	}
	else if (n == 0) {
		handleClose();
	}
	else {
		errno = savedErrno;
		handleError();
	}
}

void CTcpConnection::handleWrite()
{
	if (m_outBuffer.readableBytes() > 0) {
		int n = sockets::write(m_fd.fd(), m_outBuffer.peek(), m_outBuffer.readableBytes());
		if (n > 0) {
			m_outBuffer.retrieve(n);
			if (m_outBuffer.readableBytes() == 0) {
				m_channel->disableWriting();
				if (writeCompleteCallBack_)
					writeCompleteCallBack_(this);
			}
		}
		else {
			handleClose();
		}
	}
}

void CTcpConnection::handleClose()
{
	if (closeCallBack_)
		closeCallBack_(this);
	setState(StateE::kDisconnecting);
}

void CTcpConnection::handleError()
{
	handleClose();
}

int CTcpConnection::fd()
{
	if (m_fd.hasInit())
		return m_fd.fd();
	return INVALID_FD;
}

void CTcpConnection::addSession(CTcpSession *session)
{
	st_sessionid_t id = session->id();
	if (m_sessions.find(id) != m_sessions.end())
		return;
	m_sessions[id] = session;
}