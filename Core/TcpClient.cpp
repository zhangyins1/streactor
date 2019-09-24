#include <string>
#include "TcpClient.h"
#include "Buffer.h"
#include "../Util/Log.h"
#include "../Util/TypeDef.h"


CTcpClient::CTcpClient(const char *ip, unsigned short port) : m_fd(ip, port), m_isConnected(false), m_remainRetrys(TCP_CLIENT_CONNECT_RETRY_CNT)
{

}

CTcpClient::CTcpClient(const CSocket& s) : m_fd(s), m_isConnected(false), m_remainRetrys(TCP_CLIENT_CONNECT_RETRY_CNT)
{

}

int CTcpClient::StartConnect(bool inNonblock /* = false */)
{
	if (!m_fd.hasInit())
		m_fd.Init();
	if (inNonblock)
		m_fd.SetNonBlock(true);

	std::string ip = m_fd.ipstr();
	unsigned short port = m_fd.port(); 

	_LOG_INFO("CTcpClient::StartConnect", "start conn to: %s:%d.  ", ip.c_str(), port); // logtestt
	int ret = StConnect(m_fd, ip.c_str(), port);
	if (ret != 0)
	{
		if (errno != EINPROGRESS)
		{
			_LOG_ERROR("CTcpClient::StartConnect", "ret: %d, errno: %d", ret, errno); 
			return ST_ERR; 
		}
		_LOG_WARNING("CTcpClient::StartConnect", "errno is EINPROGRESS.");
		return 0;
	}

	m_isConnected = true; 
	return 0; 
}

int CTcpClient::Send(net::Buffer *pBuf)
{
	net::Buffer& buf = *pBuf;
	return ::send(m_fd, buf.peek(), buf.readableBytes(), 0);
}

void CTcpClient::OnTerminate()
{
	if (m_fd != INVALID_FD)
		StClose(m_fd);
}

void CTcpClient::Clean()
{
	m_isConnected = false;
	m_remainRetrys = TCP_CLIENT_CONNECT_RETRY_CNT;
	m_fd.Close();
}