#ifndef __TCP_SERVER_H_H__
#define __TCP_SERVER_H_H__

#include <map>
#include <vector>
#include <string>
#include "../Event/EventLoop.h"
#include "../Util/TypeDef.h"
#include "TcpConnection.h"

namespace net
{
	class CListener;
	class CTcpServer
	{
	public:
		using DailyResCleanCallBack = std::function<void()>;
		CTcpServer(CEventLoop *loop, const std::string& ip, st_port_t port);
		~CTcpServer();

	public:
		void					start();
		void					stop();
		void					closeConnection(const TcpConnectionPtr_t& c);
		void					OnNewConnection(int fd); 
		void					OnConnectionClose(const TcpConnectionPtr_t& c);
		bool					AddConnection(const TcpConnectionPtr_t& c);
		void					RemoveConnetion(const TcpConnectionPtr_t& c);
		void					HandleDailyResCleanUp(CEventLoop *loop); // 处理需要关闭的连接等
	public:
		void					setConnectionCallBack(const ConnectionCallBack& cb) { connectionCallBack_ = cb; }
		void					setMessageCallBack(const MessageCallBack& cb) { messageCallBack_ = cb; }
		void					setWriteCompleteCallBack(const WriteCompleteCallBack& cb) { writeCompleteCallBack_ = cb; }
		void					setDailyCleanUpCallBack(const DailyResCleanCallBack& cb) { dailyResCleanUpCallBack_ = cb; }
	private:
		ConnectionCallBack		connectionCallBack_;
		MessageCallBack			messageCallBack_;
		WriteCompleteCallBack	writeCompleteCallBack_;
		CloseCallBack			closeCallBack_;
		DailyResCleanCallBack	dailyResCleanUpCallBack_;
	private:
		typedef std::map<int, TcpConnectionPtr_t> ConnectionMap;
		ConnectionMap			m_connections;
		CEventLoop				*m_loop;
		bool					m_stoped;
		std::shared_ptr<CListener> m_listener;
	};
}

#endif  // !__TCP_SERVER_H_H__