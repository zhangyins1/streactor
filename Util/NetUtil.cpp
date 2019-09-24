#include "win32def.h"
#include "NetUtil.h"

int StSocket(int type)
{
	return socket(PF_INET, type, 0);
}

int StBind(int fd, const char *pszAddr, unsigned short port)
{
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	if (!pszAddr || strcmp(pszAddr, "") == 0 || strcmp(pszAddr, "addr_any") == 0){
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else {
		addr.sin_addr.s_addr = inet_addr(pszAddr);
	}
	int f = -1;
	int len = sizeof(int);
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &f, len);
	return bind(fd, (struct sockaddr *)&addr, sizeof addr);
}

int StListen(int fd, int backlog)
{
	return listen(fd, backlog);
}

bool StSetNonBlock(int fd)
{
	int flags = fcntl(fd, F_GETFL) | O_NONBLOCK;
	return fcntl(fd, F_SETFL, flags) == 0;
}

void StSetReuseAddr(int sockfd, bool on)
{
	int optval = on ? 1 : 0;
#ifdef WIN32
	::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)& optval, sizeof(optval));
#else
	::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
#endif
	// FIXME CHECK
}

void StSetReusePort(int sockfd, bool on)
{
	//Windows 系统没有 SO_REUSEPORT 选项
#ifndef WIN32
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
	if (ret < 0 && on)
	{
	}
#endif
}

void StSetSocketBuffSize(int fd, int nRcvBuffSize, int nSndBuffSize)
{
	setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const int *)&nRcvBuffSize, sizeof(int));
	setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const int *)&nSndBuffSize, sizeof(int));
}

int StConnect(int fd, const char *pszAddr, unsigned short port)
{
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof addr);
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(pszAddr);

	return connect(fd, (struct sockaddr *)&addr, sizeof addr);
}

int StAccept(int fd)
{
	struct sockaddr_in other_addr;
	socklen_t len;
	return accept(fd, (struct sockaddr *)&other_addr, &len);
}

void StClose(int fd)
{
	::close(fd);
}