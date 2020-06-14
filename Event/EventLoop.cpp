#include "EventLoop.h"
#include "EventModule.h"
#include "EpollModule.h"
#include "Channel.h"
#include "../Util/TypeDef.h"

CEventLoop::CEventLoop() : m_stoped(true)
{
	m_eventModule.reset(new CEpollModule(this));
	m_eventModule->Init();
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

		doPendingFunctors();
	}
}

void CEventLoop::stop()
{
	m_stoped = true; 
}

void CEventLoop::doPendingFunctors()
{
	if (m_pendingFunctors.size() > 0) {
		std::vector<PendingFunctor> functors;
		functors.swap(m_pendingFunctors);

		for (size_t i = 0; i < functors.size(); ++i)
		{
			functors[i]();
		}
	}
}

bool CEventLoop::updateChannel(CChannel * channel)
{
	return m_eventModule->UpdateChannel(channel) == ST_OK;
}

void CEventLoop::removeChannel(CChannel * channel)
{
	m_eventModule->RemoveChannel(channel);
}

bool CEventLoop::hasChannel(CChannel * channel)
{
	return m_eventModule->HasChannel(channel);
}