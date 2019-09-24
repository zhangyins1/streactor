#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>  // bzero
#include <stdio.h>  // snprintf

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  // snprintf
#include <strings.h>  // bzero
#include <sys/socket.h>
#include <unistd.h>
//for ubuntu readv not found
#ifdef __UBUNTU
#include <sys/uio.h>
#endif 

#include "Socket.h"
#include "Endian.h"
#include "../Util/TypeDef.h"
#include "../Util/win32def.h"
#include "../Util/NetUtil.h"
#include "../Util/Log.h"

using namespace net;

CSocket::CSocket(const char *pszIp, unsigned short port) : m_fd(INVALID_FD), m_ip(pszIp), m_port(port), m_state(TSocketState::SS_NONE)
{
}

CSocket::CSocket(int fd) : m_fd(fd), m_state(TSocketState::SS_CREATE), m_ip(""), m_port(INVALID_PORT)
{
}

CSocket::CSocket(const CSocket& other)
{
	m_fd = other.fd();
	m_ip = other.ipstr();
	m_port = other.port();
	m_state = other.state();
}

CSocket& CSocket::operator=(const CSocket& other)
{
	m_fd = other.fd();
	m_ip = other.ipstr();
	m_port = other.port();
	m_state = other.state();
}

CSocket::~CSocket()
{
}

bool CSocket::Init()
{
	if (m_fd != INVALID_FD) {
		m_state = TSocketState::SS_CREATE;
		return true;
	}
	m_fd = ::StSocket(SOCK_STREAM); 
	if (m_fd < 0)
	{
		LOG_DEBUG("error on ::socket().", errno);
		return false; 
	}
	m_state = TSocketState::SS_CREATE;
	return true; 
}

bool CSocket::Bind()
{
	if (m_ip.empty())
		return false; 
	_LOG_INFO("CSocket::Bind", "start bind in: %s:%d.  ", m_ip.c_str(), m_port); // logtestt
	if (StBind(m_fd, m_ip.c_str(), m_port) != 0)
	{
		LOG_DEBUG("error on ::StBind().", errno);
		return false;
	}
	m_state = TSocketState::SS_BIND;
	return true;
}

bool CSocket::Listen(int backls)
{
	if (StListen(m_fd, backls) != 0)
	{
		LOG_DEBUG("error on ::StListen().", errno);
		return false;
	}
	m_state = TSocketState::SS_LISTEN;
	return true; 
}

bool CSocket::SetNonBlock(bool b /* = true */)
{
	if (m_fd < 0)
		return false;
	if (b)
		return StSetNonBlock(m_fd);
	return true; 
}

void CSocket::SetReuseAddr(bool b /* = true */)
{
	StSetReuseAddr(m_fd, b);
}

void CSocket::SetReusePort(bool b /* = true */)
{
	StSetReusePort(m_fd, b);
}

void CSocket::Close()
{
	if (m_fd != INVALID_FD)
	{
		::StClose(m_fd);
		m_fd = INVALID_FD;
	}
	m_ip.clear();
	m_port = INVALID_PORT;
	m_state = TSocketState::SS_NONE;
}


const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
	return static_cast<const struct sockaddr*>((const void*)(addr));
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in* addr)
{
	return static_cast<struct sockaddr*>((void*)(addr));
}

const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr* addr)
{
	return static_cast<const struct sockaddr_in*>((const void*)(addr));
}

struct sockaddr_in* sockets::sockaddr_in_cast(struct sockaddr* addr)
{
	return static_cast<struct sockaddr_in*>((void*)(addr));
}

SOCKET sockets::createOrDie()
{
#ifdef WIN32
	SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
	}
#else
	SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	if (sockfd < 0)
	{
	}
#endif

	return sockfd;
}

SOCKET sockets::createNonblockingOrDie()
{
#ifdef WIN32
	SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
	}
#else
	SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	if (sockfd < 0)
	{
	}
#endif

	setNonBlockAndCloseOnExec(sockfd);
	return sockfd;
}

void sockets::setNonBlockAndCloseOnExec(SOCKET sockfd)
{
#ifdef WIN32
	//将socket设置成非阻塞的
	unsigned long on = 1;
	::ioctlsocket(sockfd, FIONBIO, &on);
#else
	// non-block
	int flags = ::fcntl(sockfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	int ret = ::fcntl(sockfd, F_SETFL, flags);
	// FIXME check

	// close-on-exec
	flags = ::fcntl(sockfd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	ret = ::fcntl(sockfd, F_SETFD, flags);
	// FIXME check

	(void)ret;
#endif       
}

void sockets::bindOrDie(SOCKET sockfd, const struct sockaddr_in& addr)
{
	int ret = ::bind(sockfd, sockaddr_cast(&addr), static_cast<socklen_t>(sizeof addr));
	if (ret < 0)
	{
	}
}

void sockets::listenOrDie(SOCKET sockfd)
{
	int ret = ::listen(sockfd, SOMAXCONN);
	if (ret < 0)
	{
	}
}

int sockets::accept(SOCKET sockfd, struct sockaddr_in* addr)
{
	socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
#ifdef WIN32
	SOCKET connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
	setNonBlockAndCloseOnExec(connfd);
#else  
	SOCKET connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
	if (connfd < 0)
	{
#ifdef WIN32
		int savedErrno = ::WSAGetLastError();
		if (savedErrno != WSAEWOULDBLOCK) {
		}
#else
		int savedErrno = errno;
		//LOGSYSE("Socket::accept");
		switch (savedErrno)
		{
		case EAGAIN:
		case ECONNABORTED:
		case EINTR:
		case EPROTO: // ???
		case EPERM:
		case EMFILE: // per-process lmit of open file desctiptor ???
			// expected errors
			errno = savedErrno;
			break;
		case EBADF:
		case EFAULT:
		case EINVAL:
		case ENFILE:
		case ENOBUFS:
		case ENOMEM:
		case ENOTSOCK:
		case EOPNOTSUPP:
			// unexpected errors
			//LOGF("unexpected error of ::accept %d", savedErrno);
			break;
		default:
			//LOGF("unknown error of ::accept %d", savedErrno);
			break;
		}

#endif
	}

	return connfd;
}

void sockets::setReuseAddr(SOCKET sockfd, bool on)
{
	int optval = on ? 1 : 0;
#ifdef WIN32
	::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)& optval, sizeof(optval));
#else
	::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
#endif
	// FIXME CHECK
}

void sockets::setReusePort(SOCKET sockfd, bool on)
{
	//Windows 系统没有 SO_REUSEPORT 选项
#ifndef WIN32
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
	if (ret < 0 && on)
	{
		//LOGSYSE("SO_REUSEPORT failed.");
	}
#endif
}

int sockets::connect(SOCKET sockfd, const struct sockaddr_in& addr)
{
	return ::connect(sockfd, sockaddr_cast(&addr), static_cast<socklen_t>(sizeof addr));
}

int32_t sockets::read(SOCKET sockfd, void *buf, int32_t count)
{
#ifdef WIN32
	return ::recv(sockfd, (char*)buf, count, 0);
#else
	return ::read(sockfd, buf, count);
#endif
}

#ifndef WIN32
ssize_t sockets::readv(SOCKET sockfd, const struct iovec *iov, int iovcnt)
{
	return ::readv(sockfd, iov, iovcnt);
}
#endif

int32_t sockets::write(SOCKET sockfd, const void *buf, int32_t count)
{
#ifdef WIN32
	return ::send(sockfd, (const char*)buf, count, 0);
#else
	return ::write(sockfd, buf, count);
#endif

}

void sockets::close(SOCKET sockfd)
{
#ifdef WIN32   
	if (::closesocket(sockfd) < 0)
#else
	if (::close(sockfd) < 0)
#endif
	{
		//LOGSYSE("sockets::close, fd=%d, errno=%d, errorinfo=%s", sockfd, errno, strerror(errno));
	}
}

void sockets::shutdownWrite(SOCKET sockfd)
{
#ifdef WIN32
	if (::shutdown(sockfd, SD_SEND) < 0)
#else
	if (::shutdown(sockfd, SHUT_WR) < 0)
#endif        
	{
		//LOGSYSE("sockets::shutdownWrite");
	}
}

void sockets::toIpPort(char* buf, size_t size, const struct sockaddr_in& addr)
{
	//if (size >= sizeof(struct sockaddr_in))
	//    return;

	::inet_ntop(AF_INET, &addr.sin_addr, buf, static_cast<socklen_t>(size));
	size_t end = ::strlen(buf);
	uint16_t port = sockets::networkToHost16(addr.sin_port);
	//if (size > end)
	//    return;

	snprintf(buf + end, size - end, ":%u", port);
}

void sockets::toIp(char* buf, size_t size, const struct sockaddr_in& addr)
{
	if (size >= sizeof(struct sockaddr_in))
		return;

	::inet_ntop(AF_INET, &addr.sin_addr, buf, static_cast<socklen_t>(size));
}

void sockets::fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	//TODO: 校验下写的对不对
#ifdef WIN32
	addr->sin_port = htons(port);
#else
	addr->sin_port = htobe16(port);
#endif
	if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
	{
		//LOGSYSE("sockets::fromIpPort");
	}
}

int sockets::getSocketError(SOCKET sockfd)
{
	int optval;
#ifdef WIN32
	int optvallen;
	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&optval, &optvallen) < 0)
		return ::WSAGetLastError();
#else
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);

	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
		return errno;
#endif
	return optval;
}

struct sockaddr_in sockets::getLocalAddr(SOCKET sockfd)
{
	struct sockaddr_in localaddr = { 0 };
	memset(&localaddr, 0, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen);
	//if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
	//{
	//	LOG_SYSERR << "sockets::getLocalAddr";
	//  return 
	//}
	return localaddr;
}

struct sockaddr_in sockets::getPeerAddr(SOCKET sockfd)
{
	struct sockaddr_in peeraddr = { 0 };
	memset(&peeraddr, 0, sizeof peeraddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
	::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen);
	//if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0)
	//{
	//	LOG_SYSERR << "sockets::getPeerAddr";
	//}
	return peeraddr;
}

bool sockets::isSelfConnect(SOCKET sockfd)
{
	struct sockaddr_in localaddr = getLocalAddr(sockfd);
	struct sockaddr_in peeraddr = getPeerAddr(sockfd);
	return localaddr.sin_port == peeraddr.sin_port && localaddr.sin_addr.s_addr == peeraddr.sin_addr.s_addr;
}