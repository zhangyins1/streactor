#ifndef __TCP_SESSION_H_H__
#define __TCP_SESSION_H_H__
#include <functional>
#include "TcpConnection.h"
#include "../Util/TypeDef.h"
using namespace net;

class CTcpSession
{
public:
	// 说明 m_connection是否有效，session是建立在 connetion的基础上的，当在子类中获取 connetion对象时，需要调用 connection(void) 函数，函数中会对连接对象做判断
	enum ConnectionState {	valid = 0,	invalid = 1	};
	CTcpSession(CTcpConnection *conn);
	virtual ~CTcpSession();
	CTcpSession(const CTcpSession&) = delete;
public:
	typedef std::function<void(CTcpSession*)> SessionCloseCallBack;
	virtual void OnRead(CTcpConnection *conn, Buffer *inbuf);
	virtual void OnClose(CTcpConnection *conn);
	virtual void OnWriteComplete(CTcpConnection *conn);
	virtual void OnConnectDestroyed();  // 当 m_connection 被销毁时 该函数被调用
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