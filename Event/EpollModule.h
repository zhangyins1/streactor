#ifndef __EPOLL_MODULE_H_H__
#define __EPOLL_MODULE_H_H__
#include <map>
#include <sys/epoll.h>

#include "EventModule.h"


#include "../Util/win32def.h"

class CEventLoop;
class CChannel;
class CEpollModule : public CEventModule
{
public:
	CEpollModule(CEventLoop *loop);
	~CEpollModule();
private:
	CEpollModule(const CEpollModule& other) = delete;
	CEpollModule(const CEpollModule&& other) = delete;
	CEpollModule& operator=(const CEpollModule& other) = delete;
	CEpollModule& operator=(const CEpollModule&& other) = delete;
public:
	virtual int Init() override;
	virtual int UpdateChannel(CChannel *channel) override;
	virtual void RemoveChannel(CChannel *channel) override;
	virtual int GetActiveChannels(int timeoutMs, ChannelVec& retActiveChannels) override;
	virtual bool HasChannel(CChannel *channel) override;
private:
	int			Add(CChannel *channel); 
	int			Del(CChannel *channel);
	int			Modify(CChannel *channel);
	int			update(int option, CChannel *channel);
	void		fillActiveChannels(int eventNum, ChannelVec& retActiveChannels);
private:
	typedef std::vector<struct epoll_event> EventList;

	int                 m_efd;
	EventList           m_events;

	typedef std::map<int, CChannel*> ChannelMap;

	ChannelMap          m_channels;
	CEventLoop*         m_ownerLoop;
	static const int kInitEventListSize = 16;
};

#endif  // !__EPOLL_MODULE_H_H__
