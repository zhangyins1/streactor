#ifndef __TCP_SESSION_H_H__
#define __TCP_SESSION_H_H__
#include <functional>
#include "TcpConnection.h"
#include "../Util/TypeDef.h"
using namespace net;

class CTcpSession
{
public:
	using TcpConnectionWeakPtr_t = std::weak_ptr<CTcpConnection>;
	using SessionId_t = st_sessionid_t;

	// ˵�� m_connection�Ƿ���Ч��session�ǽ����� connetion�Ļ����ϵģ����������л�ȡ connetion����ʱ����Ҫ���� connection(void) �����������л�����Ӷ������ж�
	enum ConnectionState {	valid = 0,	invalid = 1	};
	CTcpSession(const TcpConnectionWeakPtr_t& conn);
	virtual ~CTcpSession();
	CTcpSession(const CTcpSession&) = delete;
public:
	virtual void OnRead(const TcpConnectionPtr_t& c, Buffer *inbuf);
	virtual void OnClose(const TcpConnectionPtr_t& c);
	virtual void OnWriteComplete(const TcpConnectionPtr_t& c);
	virtual void OnConnectDestroyed();  // �� m_connection ������ʱ �ú���������
	void Send(const char *buf, int len);
	st_sessionid_t id(void) { return m_id; }
public:
protected:
	std::shared_ptr<CTcpConnection> getConnectionPtr(void) {
		if (m_connection.expired())
			return nullptr;
		return m_connection.lock();
	}
protected:
	Buffer m_inBuffer;
	Buffer m_outBuffer;
	SessionId_t m_id;
	TcpConnectionWeakPtr_t m_connection;
};


#endif  // !__TCP_SESSION_H_H__