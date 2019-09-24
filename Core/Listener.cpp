#include "Listener.h"
#include "../Util/TypeDef.h"
#include "../Event/EventLoop.h"
#include "../Util/NetUtil.h"
#include "../Event/Channel.h"

using namespace net;

CListener::CListener(CEventLoop *loop, const std::string& ip, st_port_t port) : 
m_fd(ip.c_str(), port), m_ip(ip), m_port(port), m_loop(loop)
{
	m_fd.Init();
	m_channel = new CChannel(loop, m_fd.fd());
	m_fd.SetNonBlock(true);
	m_fd.SetReuseAddr(true);
	m_fd.SetReusePort(true);
	m_fd.Bind();
	m_channel->setReadCallBack(std::bind(&CListener::OnNewConnection, this));
}

CListener::~CListener()
{

}

void CListener::OnNewConnection()
{
	int fd = StAccept(m_fd.fd());
	if (fd <= 0) {
		return;
	}
	if (newConnectionCallBack_)
		newConnectionCallBack_(fd);
}

void CListener::StartListening()
{
	m_fd.Listen(LISTEN_BACKLOG);
	m_channel->enableReading();
}