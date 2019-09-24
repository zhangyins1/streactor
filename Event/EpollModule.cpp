#include <unistd.h>
#include "EpollModule.h"
#include "EventLoop.h"
#include "Channel.h"

#include "../Util/Log.h"
#include "../Util/TypeDef.h"
#include "../Util/Misc.h"
#include "../Common/Platform.h"
namespace
{
	const int kNew = -1;
	const int kAdded = 1;
	const int kDeleted = 2;
}

CEpollModule::CEpollModule(CEventLoop *loop) : m_ownerLoop(loop), m_events(kInitEventListSize)
{
}

CEpollModule::~CEpollModule()
{
}

int CEpollModule::Add(CChannel *channel)
{
	int fd = channel->fd();
	auto it = m_channels.find(fd);
	if (it != m_channels.end())
	{
		return ST_ERR;
	}
	update(XEPOLL_CTL_ADD, channel);
	m_channels[fd] = channel;
	channel->setState(kAdded);

	_LOG_INFO("CEpollModule::Add", "fd: %d, m_fds.size: %d", fd, m_channels.size());
	return ST_OK;
}

int CEpollModule::Del(CChannel *channel)
{
	int fd = channel->fd();
	auto it = m_channels.find(fd);
	if (it != m_channels.end())
	{
		update(XEPOLL_CTL_DEL, channel);
		m_channels.erase(it);
		channel->setState(kNew);
	}
	return ST_OK;
}

int CEpollModule::Modify(CChannel *channel)
{
	int fd = channel->fd();
	auto it = m_channels.find(fd);
	if (it != m_channels.end())
	{
		update(XEPOLL_CTL_MOD, channel);
		channel->setState(kAdded);
	}
	else {
		return ST_ERR;
	}
	return ST_OK;
}

int CEpollModule::UpdateChannel(CChannel *channel)
{
	int state = channel->state();
	if (state == kNew) {
		return Add(channel);
	}
	else if (state == kAdded) {
		return Modify(channel);
	}
	else {
		return Del(channel);
	}
}

void CEpollModule::RemoveChannel(CChannel *channel)
{
	Del(channel);
}

int CEpollModule::Init()
{
	m_efd = epoll_create(EPOLL_CLOEXEC);
	if (m_efd < 0)
	{
		LOG_ERROR("error on epoll_create().", errno);
		return ST_ERR; 
	}
	CEventModule::Init();
	return ST_OK;
}

int CEpollModule::update(int option, CChannel *channel)
{
	int fd = channel->fd();
	struct epoll_event ev;
	st_memzero(&ev, sizeof(ev));
	ev.events = channel->events();
	ev.data.ptr = channel;
	if (::epoll_ctl(m_efd, option, fd, &ev) < 0) {
		return ST_ERR;
	}
	return ST_OK;
}

int CEpollModule::GetActiveChannels(int timeoutMs, ChannelVec& retActiveChannels)
{
	int eventNum = epoll_wait(m_efd, &(*m_events.begin()), m_events.size(), timeoutMs);
	int savedErrno = errno;
	if (eventNum > 0) {
		fillActiveChannels(eventNum, retActiveChannels);
		if (static_cast<size_t>(eventNum) == m_events.size()) {
			m_events.resize(m_events.size() * 2);
		}
	}
	else if (eventNum == 0) {

	}
	else {
		if (savedErrno != EINTR)
		{
			errno = savedErrno;
			return ST_ERR;
		}
	}
	return ST_OK;
}

bool CEpollModule::HasChannel(CChannel *channel)
{
	int fd = channel->fd();
	auto it = m_channels.find(fd);
	if (it != m_channels.end() && it->second == channel)
		return true;
	return false; 
}

void CEpollModule::fillActiveChannels(int eventNum, ChannelVec& retActiveChannels)
{
	for (int i = 0; i < eventNum; ++i)
	{
		CChannel* channel = static_cast<CChannel*>(m_events[i].data.ptr);
		int fd = channel->fd();
		ChannelMap::const_iterator it = m_channels.find(fd);
		if (it == m_channels.end() || it->second != channel)
			return;
		channel->setRevents(m_events[i].events);
		retActiveChannels.push_back(channel);
	}
}