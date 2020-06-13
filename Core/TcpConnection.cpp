#include "TcpConnection.h"
#include "../Event/Channel.h"
#include "TcpSession.h"
#include "../Util/Log.h"
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
	_LOG_INFO("CTcpConnection::~CTcpConnection", "fd: %d", m_fd.fd());
	m_fd.Close();
}

void CTcpConnection::connectEstablished()
{
	setState(StateE::kConnected);
	m_channel->enableReading();
}

void CTcpConnection::connectDestroyed()
{
	if (m_state == StateE::kConnected) {
		m_channel->disableAllEvent();
		setState(StateE::kDisconnected);

		connectionCallBack_(shared_from_this());
	}
	setState(StateE::kDisconnected);
	m_channel->remove();
}

int CTcpConnection::recv(char* buf, int size)
{
	return sockets::read(m_fd.fd(), buf, size);
}

int CTcpConnection::send(const char* buf, int len)
{
	m_outBuffer.append(buf, len);
	if (!m_channel->hasWriteEvent())
		m_channel->enableWriting();
	return len;
}

void CTcpConnection::handleRead()
{
	int savedErrno = 0;
	int n = m_inBuffer.readFd(m_fd.fd(), &savedErrno);
	
	if (n > 0) {
		if (messageCallBack_)
			messageCallBack_(shared_from_this(), &m_inBuffer);
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
					writeCompleteCallBack_(shared_from_this());
			}
		}
		else {
			handleClose();
		}
	}
}

void CTcpConnection::handleClose()
{
	setState(StateE::kDisconnecting);
	TcpConnectionPtr_t guardThis(shared_from_this());
	connectionCallBack_(guardThis);
	if (closeCallBack_)
		closeCallBack_(guardThis);

	if (m_outBuffer.readableBytes() > 0) {
		handleWrite();
	}
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
