#ifndef __TCP_CONNECTION_H_H__
#define __TCP_CONNECTION_H_H__
#include <map>
#include <memory>
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
	class CTcpConnection : public std::enable_shared_from_this<CTcpConnection>
	{
	public:
		CTcpConnection(CEventLoop *loop, int fd);
		~CTcpConnection();
	public:
		int					send(const char* buf, int len);
		int					recv(char* buf, int size);
		void				handleRead();
		void				handleWrite();
		void				handleClose();
		void				handleError();
	public:
		// called when TcpServer accepts a new connection
		void				connectEstablished();   // should be called only once
		// called when TcpServer has removed me from its map
		void				connectDestroyed();  // should be called only once
	private:
		enum StateE {kDisconnected, kConnecting, kConnected, kDisconnecting};
		void				setState(StateE state) { m_state = state; }
	public:
		int					fd();
		bool				isConnected(void) { return m_state == kConnected; }
	public:
		void				setConnectionCallBack(const ConnectionCallBack& cb) { connectionCallBack_ = cb; }
		void				setMessageCallBack(const MessageCallBack& cb) { messageCallBack_ = cb; }
		void				setWriteCompleteCallBack(const WriteCompleteCallBack& cb) { writeCompleteCallBack_ = cb; }
		void				setCloseCallBack(const CloseCallBack& cb) { closeCallBack_ = cb; }
	private:
		ConnectionCallBack	connectionCallBack_;
		MessageCallBack		messageCallBack_;
		WriteCompleteCallBack writeCompleteCallBack_;
		CloseCallBack		closeCallBack_;
	private:
		CSocket				m_fd;
		Buffer				m_inBuffer;
		Buffer				m_outBuffer;
		std::shared_ptr<CChannel> m_channel;
		StateE				m_state;
	};
}

#endif  // !__TCP_CONNECTION_H_H__
