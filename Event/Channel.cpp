#include "Channel.h"
#include "../Common/Platform.h"
#include "../Util/TypeDef.h"
#include "EventLoop.h"

const int CChannel::kNoneEvent = 0;
const int CChannel::kReadEvent = XPOLLIN | XPOLLPRI;
const int CChannel::kWriteEvent = XPOLLOUT;

CChannel::CChannel(CEventLoop *loop, int fd) : m_events(0), m_revents(0), m_fd(fd), m_loop(loop), m_state(-1)
{

}

CChannel::~CChannel()
{
	this->removeAllEvent();
}

void CChannel::handleEvents()
{
	if ((m_revents & XPOLLHUP) && !(m_revents & XPOLLIN))
	{
		if (closeCallBack_)
			closeCallBack_();
	}

	if (m_revents & (XPOLLERR | XPOLLNVAL))
	{
		if (errorCallBack_)
			errorCallBack_();
	}

	if (m_revents & (XPOLLIN | XPOLLPRI | XPOLLRDHUP))
	{
		if (readCallBack_)
			readCallBack_();
	}

	if (m_revents & XPOLLOUT)
	{
		if (writeCallBack_)
			writeCallBack_();
	}
}

bool CChannel::update()
{
	return m_loop->updateChannel(this);
}

bool CChannel::enableReading()
{
	m_events |= kReadEvent;
	return update();
}

bool CChannel::disableReading()
{
	m_events &= ~kReadEvent;
	return update();
}

bool CChannel::enableWriting()
{
	m_events |= kWriteEvent;
	return update();
}

bool CChannel::disableWriting()
{
	m_events &= ~kWriteEvent;
	return update();
}

bool CChannel::hasWriteEvent()
{
	return (m_events & kWriteEvent) != 0;
}

void CChannel::removeAllEvent()
{
	m_loop->removeChannel(this);
}