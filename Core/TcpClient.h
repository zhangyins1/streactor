#ifndef _TCP_CLIENT_H_H__
#define _TCP_CLIENT_H_H__
#include "../Util/NetUtil.h"
#include "../Core//Socket.h"
#include "Buffer.h"

class CSocket;
class CTcpClient
{
public:
	CTcpClient(const char *ip, unsigned short port);
	CTcpClient(const CSocket& s);
	~CTcpClient() {}

public: 
	int StartConnect(bool inNonblock = false);
	int Send(net::Buffer *pBuf);
	virtual void OnTerminate();
	void Clean(void);
	int getFd(void) const { return m_fd.fd(); }
	bool IsConnected(void) { return m_isConnected; }
	void SetIsConnected(bool b) { m_isConnected = b; }
private:
	CSocket			m_fd;
	bool			m_isConnected;   // 是否已经连接
	int				m_remainRetrys;	 // 重试连接剩余次数
};

#endif // !_TCP_CLIENT_H_H__
