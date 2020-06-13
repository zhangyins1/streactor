#include <stdio.h>
#include "EchoSession.h"
#include "../Util/Log.h"

CEchoSession::CEchoSession(const TcpConnectionWeakPtr_t& c) : CTcpSession(c)
{

}

CEchoSession::~CEchoSession()
{
#if IS_DEBUG
	auto c = getConnectionPtr();
	int fd = -2;
	if (c)
		fd = c->fd();
	_LOG_INFO("CEchoSession::~CEchoSession", "fd: %d", fd);
#endif
}

void CEchoSession::OnRead(const TcpConnectionPtr_t& c, Buffer *inbuf)
{
	if (inbuf->readableBytes() > 0) {
		std::string msg(inbuf->peek(), inbuf->readableBytes());
		inbuf->retrieve(msg.size());
#if IS_DEBUG
		printf("recved: %s(%d bytes)\n", msg.c_str(), msg.size());
#endif
		bool needClose = false; 
		if (msg[msg.size() - 1] == '\n')
			msg.assign(msg.begin(), msg.end() - 1);
		if (msg == "bye" || msg == "exit" || msg == "quit") {
			msg = "s: bye!\n";
			needClose = true;
		}
		else
			msg = std::string("s: ") + msg + std::string("\n");
		Send(msg.c_str(), msg.size());
		if (needClose)
			c->handleClose();
	}
}