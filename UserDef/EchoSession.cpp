#include <stdio.h>
#include "EchoSession.h"
#include "../Util/Log.h"

CEchoSession::CEchoSession(CTcpConnection *conn) : CTcpSession(conn)
{

}

CEchoSession::~CEchoSession()
{

}

void CEchoSession::OnRead(CTcpConnection *conn, net::Buffer *inbuf)
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
			conn->handleClose();
	}
}