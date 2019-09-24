#ifndef __LISTENER_H_H__
#define __LISTENER_H_H__
#include <string>
#include "Socket.h"
#include "../Event/Channel.h"
#include "../Util/TypeDef.h"

class CEventLoop;
namespace net
{
	class CListener
	{
	public:
		typedef std::function<void(int)> NewConnectionCallBack;
		CListener(CEventLoop *loop, const std::string& ip, st_port_t port);  // ip Ϊ 0.0.0.0 �� any_addr ʱ�Ż���������κε�ַ������
		~CListener();
	public:
		void						setNewConnectionCallBack(const NewConnectionCallBack& cb) { newConnectionCallBack_ = cb; }
	public:
		void						OnNewConnection();
		void						StartListening();
	private:
		NewConnectionCallBack		newConnectionCallBack_;
	private:
		CSocket						m_fd;
		std::string					m_ip;
		st_port_t					m_port;
		CChannel					*m_channel;
		CEventLoop					*m_loop;
	};
}

#endif  // !__LISTENER_H_H__
