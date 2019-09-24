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
	CEventLoop();
	~CEventLoop();
public:
	typedef std::function<void(CEventLoop *)> EventLoopRuntimeCallBack;
	bool					updateChannel(CChannel * channel);
	void					removeChannel(CChannel * channel);
	void					loop();
	void					stop();
public:
	void					setEventModule(CEventModule *eventModule) { m_eventModule = eventModule; }
	void					setRuntimeCallBack(const EventLoopRuntimeCallBack& cb) { runtimeCallBack_ = cb; }
private:
	EventLoopRuntimeCallBack runtimeCallBack_;
private:
	typedef std::vector<CChannel *> ChannelVec;
	CEventModule			*m_eventModule;
	ChannelVec				m_activeChannels;
	CChannel				*m_curActiveChannel;
	bool					m_stoped;
};

#endif  // !__EVENT_LOOP_H_H__
