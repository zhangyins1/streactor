#ifndef __TCP_SESSION_H_H__
#define __TCP_SESSION_H_H__
#include <functional>
#include "TcpConnection.h"
#include "../Util/TypeDef.h"
using namespace net;

class CTcpSession
{
public:
	// ˵�� m_connection�Ƿ���Ч��session�ǽ����� connetion�Ļ����ϵģ����������л�ȡ connetion����ʱ����Ҫ���� connection(void) �����������л�����Ӷ������ж�
	enum ConnectionState {	valid = 0,	invalid = 1	};
	CTcpSession(CTcpConnection *conn);
	virtual ~CTcpSession();
	CTcpSession(const CTcpSession&) = delete;
public:
	typedef std::function<void(CTcpSession*)> SessionCloseCallBack;
	virtual void OnRead(CTcpConnection *conn, Buffer *inbuf);
	virtual void OnClose(CTcpConnection *conn);
	virtual void OnWriteComplete(CTcpConnection *conn);
	virtual void OnConnectDestroyed();  // �� m_connection ������ʱ �ú���������
	void Send(const char *buf, int len);
	st_sessionid_t id(void) { return m_id; }
public:
	ConnectionState				connectionState(void) { return m_connState; }
	void						setSessionCloseCallBack(const SessionCloseCallBack& cb) { sessionCloseCallBack_ = cb; }
protected:
	CTcpConnection*				connection(void) { return m_connState == ConnectionState::valid ? m_connection : nullptr; }
private:
	CTcpConnection				*m_connection;
	ConnectionState				m_connState;
	st_sessionid_t				m_id;
	SessionCloseCallBack		sessionCloseCallBack_;
};


#endif  // !__TCP_SESSION_H_H__