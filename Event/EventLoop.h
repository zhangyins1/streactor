#ifndef __EVENT_LOOP_H_H__
#define __EVENT_LOOP_H_H__

#include <vector>
#include <functional>
#include <memory>

class CEventLoop;
class CChannel;
class CEventModule;
class CEventLoop
{
public:
	using PendingFunctor = std::function<void()>;
	using ChannelVec = std::vector<CChannel *>;
	using EventLoopRuntimeCallBack = std::function<void(CEventLoop *)>;
public:
	CEventLoop();
	~CEventLoop();
public:
	bool					updateChannel(CChannel * channel);
	void					removeChannel(CChannel * channel);
	bool					hasChannel(CChannel * channel);
	void					loop();
	void					stop();
	void					doPendingFunctors();
public:
	void					setRuntimeCallBack(const EventLoopRuntimeCallBack& cb) { runtimeCallBack_ = cb; }
	void					addPendingFunctor(const PendingFunctor& functor) {
		m_pendingFunctors.push_back(functor);
	}
private:
	EventLoopRuntimeCallBack runtimeCallBack_;
private:
	std::shared_ptr<CEventModule> m_eventModule;
	ChannelVec				m_activeChannels;
	CChannel				*m_curActiveChannel;
	bool					m_stoped;
	std::vector<PendingFunctor> m_pendingFunctors;
};

#endif  // !__EVENT_LOOP_H_H__
