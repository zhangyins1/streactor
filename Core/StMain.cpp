#include <functional>
#include <string.h>

#include "Buffer.h"
#include "TcpClient.h"
#include "../Util/TypeDef.h"
#ifdef ST_WIN32
#include "../Event/IocpModule.h"
#else 
#include "../Event/EpollModule.h"
#endif
#include "../Util/NetUtil.h"
#include "../Util/Log.h"
#include "../Util/Misc.h"
#include "../Util/Config.h"
#include "../Event/EventLoop.h"
#include "../UserDef/EchoServer.h"

CConfig *g_config = nullptr;
CEventLoop g_mainLoop;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		_LOG_INFO("main", "too few args, argc: %d", argc);
		return 0;
	}

	const char *pszCfgPath = argv[1];
	if (!pszCfgPath)
	{
		_LOG_INFO("main", "cfg path is null.");
		return 0;
	}

	g_config = new CConfig;
	if (!g_config->Init(pszCfgPath))
	{
		_LOG_INFO("main", "Config init failed.");
		return 0;
	}
	
	CEventModule *emd = nullptr;
	emd = new CEpollModule(&g_mainLoop);
	emd->Init();
	g_mainLoop.setEventModule(emd);

	CEchoServer *server = new CEchoServer();
	server->Init(&g_mainLoop, "addr_any", g_config->m_listenPort);
	server->Start();
	g_mainLoop.loop();

	/*
	CServer *server = new CServer("addr_any", g_config->m_listenPort);
	g_server = server; 
	server->AddListening(string("addr_any"), g_config->m_listenPort, ls, aev, &g_emptyEvent);

	if (!server->Init(emd))
	{
		_LOG_ERROR("main()", "server::Init failed! Process exit.");
		return -1;
	}
	server->Start(); 
	*/
	return 0;
}

