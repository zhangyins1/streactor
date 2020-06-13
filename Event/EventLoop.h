#ifndef __EVENT_LOOP_H_H__
#define __EVENT_LOOP_H_H__

#include <vector>
#include <functional>

class CEventLoop;
class CChannel;
class CEventModule;
class CEventLoop
{
public:
	using PendingFunctor = std::function<void()>;
	using ChannelVec = std::vector<CChannel *>;
public:
	CEventLoop();
	~CEventLoop();
public:
	typedef std::function<void(CEventLoop *)> EventLoopRuntimeCallBack;
	bool					updateChannel(CChannel * channel);
	void					removeChannel(CChannel * channel);
	bool					hasChannel(CChannel * channel);
	void					loop();
	void					stop();
	void					doPendingFunctors();
public:
	void					setEventModule(CEventModule *eventModule) { m_eventModule = eventModule; }
	void					setRuntimeCallBack(const EventLoopRuntimeCallBack& cb) { runtimeCallBack_ = cb; }
	void					addPendingFunctor(const PendingFunctor& functor) {
		m_pendingFunctors.push_back(functor);
	}
private:
	EventLoopRuntimeCallBack runtimeCallBack_;
private:
	CEventModule			*m_eventModule;
	ChannelVec				m_activeChannels;
	CChannel				*m_curActiveChannel;
	bool					m_stoped;
	std::vector<PendingFunctor> m_pendingFunctors;
};

#endif  // !__EVENT_LOOP_H_H__
