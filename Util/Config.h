#ifndef __CONFIG_H_H__
#define __CONFIG_H_H__

#include <string.h>
#include "../Util/cfg_reader.h"

class CCfgReader;
class CConfig
{
public:
	CConfig();
	~CConfig();
	
public:
	bool	 Init(const std::string& filepath);
public: 
	std::string					m_gameSrvIp;
	unsigned short				m_gameSrvPort;
	unsigned short				m_listenPort;
private:
	CCfgReader					*m_cfg;
};

#endif  // !__CONFIG_H_H__
