#ifndef __CALL_BACK_TYPE_DEF_H_H__
#define __CALL_BACK_TYPE_DEF_H_H__

#include <functional>
#include <memory>

namespace net
{
	class Buffer;
	class CTcpConnection;

	using TcpConnectionPtr_t = std::shared_ptr<CTcpConnection>;

	using ConnectionCallBack = std::function<void(const TcpConnectionPtr_t& conn)>;
	using MessageCallBack = std::function<void(const TcpConnectionPtr_t& conn, Buffer *inbuf)>;
	using WriteCompleteCallBack = std::function<void(const TcpConnectionPtr_t& conn)>;
	using CloseCallBack = std::function<void(const TcpConnectionPtr_t& conn)>;
	using ReadEventCallBack = std::function<void(const TcpConnectionPtr_t& conn)>;
	using WriteEventCallBack = std::function<void(const TcpConnectionPtr_t& conn)>;
}

#endif // !__CALL_BACK_TYPE_DEF_H_H__