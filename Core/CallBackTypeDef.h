#ifndef __CALL_BACK_TYPE_DEF_H_H__
#define __CALL_BACK_TYPE_DEF_H_H__

#include <functional>

namespace net
{
	class Buffer;
	class CTcpConnection;
	typedef	std::function<void(CTcpConnection  *conn, net::Buffer *inbuf)> MessageCallBack;
	typedef std::function<void(CTcpConnection  *conn)> WriteCompleteCallBack;
	typedef std::function<void(CTcpConnection  *conn)> CloseCallBack;
}

#endif // !__CALL_BACK_TYPE_DEF_H_H__