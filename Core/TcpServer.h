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
		typedef std::function<void(CTcpConnection *)> ConnectionCallBack;
		typedef std::function<void()> DailyResCleanCallBack;
		CTcpServer(CEventLoop *loop, const std::string& ip, st_port_t port);
		~CTcpServer();

	public:
		void					start();
		void					stop();
		void					closeConnection(CTcpConnection *c);
		void					OnNewConnection(int fd); 
		void					OnConnectionClose(CTcpConnection *c);
		bool					AddConnection(CTcpConnection *c);
		void					RemoveConnetion(CTcpConnection *c);
		void					BindSession(CTcpSession *session, CTcpConnection *c);
		void					HandleDailyResCleanUp(CEventLoop *loop); // 处理需要关闭的连接等
	public:
		void					setConnectionCallBack(const ConnectionCallBack& cb) { connectionCallBack_ = cb; }
		void					setDailyCleanUpCallBack(const DailyResCleanCallBack& cb) { dailyResCleanUpCallBack_ = cb; }
	private:
		ConnectionCallBack		connectionCallBack_;
		DailyResCleanCallBack	dailyResCleanUpCallBack_;
	private:
		typedef std::map<int, CTcpConnection*> ConnectionMap;
		typedef std::vector<CTcpConnection *> ConnectionList;
		ConnectionMap			m_connections;
		ConnectionList			m_needDelConnections;
		CEventLoop				*m_loop;
		bool					m_stoped;
		CListener				*m_listener;
	};
}

#endif  // !__TCP_SERVER_H_H__