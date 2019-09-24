#ifndef __EVENT_MODULE_H_H__
#define __EVENT_MODULE_H_H__
#include <map>
#include <vector>

class CChannel;
class CEventModule
{
public:
	typedef std::vector<CChannel *> ChannelVec;
	CEventModule() : m_hasInit(false) {}
	~CEventModule() {}
public:
	/*event actions*/
	virtual int UpdateChannel(CChannel *channel) = 0;
	virtual void RemoveChannel(CChannel *channel) = 0;
	virtual int GetActiveChannels(int timeoutMs, ChannelVec& retActiveChannels) = 0;
	virtual bool HasChannel(CChannel *channel) = 0;

	virtual int Init() { m_hasInit = true; }
	virtual bool HasInit() { return m_hasInit;	}
protected:
	bool			m_hasInit;
};

#endif  // !__EVENT_MODULE_H_H__
