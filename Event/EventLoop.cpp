#include "EventLoop.h"
#include "EventModule.h"
#include "Channel.h"
#include "../Util/TypeDef.h"
CEventLoop::CEventLoop() : m_stoped(true)
{

}

CEventLoop::~CEventLoop()
{

}

void CEventLoop::loop()
{
	m_stoped = false;
	while (!m_stoped)
	{
		m_activeChannels.clear();
		if (m_eventModule->GetActiveChannels(50, m_activeChannels) != ST_OK) {

		}

		for (CChannel *channel : m_activeChannels)
		{
			m_curActiveChannel = channel;
			m_curActiveChannel->handleEvents();
		}
		m_curActiveChannel = nullptr;

		if (runtimeCallBack_)
			runtimeCallBack_(this);
	}
}

void CEventLoop::stop()
{
	m_stoped = true; 
}

bool CEventLoop::updateChannel(CChannel * channel)
{
	return m_eventModule->UpdateChannel(channel) == ST_OK;
}

void CEventLoop::removeChannel(CChannel * channel)
{
	m_eventModule->RemoveChannel(channel);
}