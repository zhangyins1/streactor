#ifndef __CHANNEL_H_H__
#define __CHANNEL_H_H__
#include <functional>

class CEventLoop;
class CChannel
{
public:
	typedef std::function<void()> EventCallback;
	
	CChannel(CEventLoop *loop, int fd);
	~CChannel();
public:
	void				handleEvents();
	bool				update();
public:
	void				setReadCallBack(const EventCallback& cb) { readCallBack_ = cb; }
	void				setWriteCallBack(const EventCallback& cb) { writeCallBack_ = cb; }
	void				setcloseCallBack(const EventCallback& cb) { closeCallBack_ = cb; }
	void				setErrorCallBack(const EventCallback& cb) { errorCallBack_ = cb; }
	void				setEvents(int events) { m_events = events; }
	void				setRevents(int revents) { m_revents = revents; }
	void				setState(int state) { m_state = state; }

	int					events(void) { return m_events; }
	int					revents(void) { return m_revents; }
	int					fd(void) { return m_fd; }
	int					state(void) { return m_state; }
public:
	bool				enableReading();
	bool				disableReading();
	bool 				enableWriting();
	bool				disableWriting();
	bool				hasWriteEvent();
	void				removeAllEvent();
private:
	EventCallback		readCallBack_;
	EventCallback		writeCallBack_;
	EventCallback		closeCallBack_;
	EventCallback		errorCallBack_;
private:
	static const int            kNoneEvent;
	static const int            kReadEvent;
	static const int            kWriteEvent;

	const int			m_fd;
	int					m_events;			// ��channel��ע���¼�
	int					m_revents;			// ��ǰ�� EventModule �յ��ĵȴ�������¼�  recved events
	CEventLoop			*m_loop;			
	int					m_state;			// -1 û�б����뵽 CEventModule ��, 1 �Ѿ�������  2 �Ѿ�ɾ����
};

#endif  // !__CHANNEL_H_H__