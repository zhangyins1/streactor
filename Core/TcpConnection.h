#ifndef __TCP_CONNECTION_H_H__
#define __TCP_CONNECTION_H_H__
#include <map>
#include "../Core/Socket.h"
#include "../Core/Buffer.h"
#include "../Util/TypeDef.h"
#include "CallBackTypeDef.h"

using std::map;
class CTcpSession;
class CChannel;
class CEventLoop;
namespace net
{
	class CTcpConnection
	{
	public:
		CTcpConnection(CEventLoop *loop, int fd);
		~CTcpConnection();
	public:
		void				send(const char* buf, int len);
		void				handleRead();
		void				handleWrite();
		void				handleClose();
		void				handleError();
	public:
		// called when TcpServer accepts a new connection
		void				connectEstablished();   // should be called only once
		// called when TcpServer has removed me from its map
		void				connectDestroyed();  // should be called only once
	public:
		void				addSession(CTcpSession *session);
	private:
		enum StateE {kDisconnected, kConnecting, kConnected, kDisconnecting};
		void				setState(StateE state) { m_state = state; }
	public:
		int					fd();
		bool				isConnected(void) { return m_state == kConnected; }
	public:
		void				setMessageCallBack(const MessageCallBack& cb) { messageCallBack_ = cb; }
		void				setWriteCompleteCallBack(const WriteCompleteCallBack& cb) { writeCompleteCallBack_ = cb; }
		void				setCloseCallBack(const CloseCallBack& cb) { closeCallBack_ = cb; }
	private:
		MessageCallBack		messageCallBack_;
		WriteCompleteCallBack writeCompleteCallBack_;
		CloseCallBack		closeCallBack_;
	private:
		typedef std::map<st_sessionid_t, CTcpSession *> SessionMap;
		CSocket				m_fd;
		Buffer				m_inBuffer;
		Buffer				m_outBuffer;
		CChannel			*m_channel;
		SessionMap			m_sessions;
		StateE				m_state;
	};
}

#endif  // !__TCP_CONNECTION_H_H__
