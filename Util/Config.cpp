#include "Config.h"
#include "../Util/Log.h"
#include "../Util/Misc.h"

CConfig::CConfig() : m_cfg(nullptr), m_gameSrvIp(""), m_gameSrvPort(0), m_listenPort(0)
{

}

CConfig::~CConfig()
{
	if (m_cfg)
	{
		delete m_cfg;
		m_cfg = nullptr;
	}
}

bool CConfig::Init(const std::string& filepath)
{
	CCfgReader *cfg = new CCfgReader(filepath);
	if (!cfg)
	{
		_LOG_INFO("main", "cfg obj is null. path: %s", filepath.c_str());
		return false;
	}

	if (m_cfg)
	{
		SAFE_DEL(m_cfg);
	}

	m_cfg = cfg;

	m_gameSrvIp = cfg->GetValue("game_server", "ip");
	m_gameSrvPort = std::stoi(cfg->GetValue("game_server", "port"));
	m_listenPort = std::stoi(cfg->GetValue("local_server", "port"));

	return true; 
}