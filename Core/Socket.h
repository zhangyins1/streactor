#ifndef __SOCKET_H_H__
#define __SOCKET_H_H__
#include <string>
#include <arpa/inet.h>
#include "../Util/win32def.h"
#include "../Common/Platform.h"

enum class TSocketState
{
	SS_ERROR = -1,
	SS_NONE = 0,
	SS_CREATE = 1,
	SS_BIND,
	SS_LISTEN,
};

class CSocket
{
public:
	CSocket(const char *pszIp = "", unsigned short port = 0);
	CSocket(int fd);
	CSocket(const CSocket& other);
	CSocket& operator=(const CSocket& other);
	~CSocket();


	bool Init(); 
	bool Bind();
	bool Listen(int backls); 
	bool SetNonBlock(bool b = true); 
	void SetReuseAddr(bool b = true);
	void SetReusePort(bool b = true); 
	void Close(void);

	bool hasInit(void) const {
		return m_state >= TSocketState::SS_CREATE;
	}
	TSocketState state(void) const { return m_state; }
	operator int() {
		return m_fd; 
	}
public:
	int fd(void) const { return m_fd; }
	unsigned short port(void) const { return m_port; }
	std::string ipstr(void) const { return m_ip; }
private:
	int						m_fd; 
	std::string				m_ip;
	unsigned short			m_port;
	TSocketState			m_state;
};

namespace net
{
	namespace sockets
	{
		///
		/// Creates a socket file descriptor,
		/// abort if any error.
		SOCKET createOrDie();
		SOCKET createNonblockingOrDie();

		void setNonBlockAndCloseOnExec(SOCKET sockfd);

		void setReuseAddr(SOCKET sockfd, bool on);
		void setReusePort(SOCKET sockfd, bool on);

		int  connect(SOCKET sockfd, const struct sockaddr_in& addr);
		void bindOrDie(SOCKET sockfd, const struct sockaddr_in& addr);
		void listenOrDie(SOCKET sockfd);
		int  accept(SOCKET sockfd, struct sockaddr_in* addr);
		int32_t read(SOCKET sockfd, void *buf, int32_t count);
#ifndef WIN32
		ssize_t readv(SOCKET sockfd, const struct iovec *iov, int iovcnt);
#endif
		int32_t write(SOCKET sockfd, const void *buf, int32_t count);
		void close(SOCKET sockfd);
		void shutdownWrite(SOCKET sockfd);

		void toIpPort(char* buf, size_t size, const struct sockaddr_in& addr);
		void toIp(char* buf, size_t size, const struct sockaddr_in& addr);
		void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

		int getSocketError(SOCKET sockfd);

		const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
		struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);
		const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
		struct sockaddr_in* sockaddr_in_cast(struct sockaddr* addr);

		struct sockaddr_in getLocalAddr(SOCKET sockfd);
		struct sockaddr_in getPeerAddr(SOCKET sockfd);
		bool isSelfConnect(SOCKET sockfd);
	}
}

#endif // !__SOCKET_H_H__
