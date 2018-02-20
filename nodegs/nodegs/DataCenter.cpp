#include "stdafx.h"
#include "DataCenter.h"
#include "CStr.h"

namespace OwLibSV
{
	ChatService *m_chatService = 0;
	map<String, HttpEasyService*> m_httpEasyServices;
	HttpHardService *m_httpHardService;
	IndicatorService *m_indicatorService = 0;
	IndicatorLayoutService *m_indicatorLayoutService = 0;
	bool m_isAppAlive = true;
	NodeService *m_nodeService = 0;
	SecurityService *m_securityService = 0;

	ChatService* DataCenter::GetChatService()
	{
		return m_chatService;
	}	
	
	map<String, HttpEasyService*> DataCenter::GetHttpEasyServices()
	{
		return m_httpEasyServices;
	}

	HttpHardService* DataCenter::GetHttpHardService()
	{
		return m_httpHardService;
	}

	IndicatorService* DataCenter::GetIndicatorService()
	{
		return m_indicatorService;
	}

	IndicatorLayoutService* DataCenter::GetIndicatorLayoutService()
	{
		return m_indicatorLayoutService;
	}

	NodeService* DataCenter::GetNodeService()
	{
		return m_nodeService;
	}

	SecurityService* DataCenter::GetSecurityService()
	{
		return m_securityService;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	string DataCenter::GetAppDir()
	{
		char exeFullPath[MAX_PATH]; 
		string strPath = "";
		GetModuleFileNameA(0, exeFullPath, MAX_PATH);
		strPath = (string)exeFullPath; 
		int pos = (int)strPath.rfind('\\', strPath.length());
		return strPath.substr(0, pos);
	}

	bool DataCenter::IsAppAlive()
	{
		return m_isAppAlive;
	}

	void DataCenter::StartService(string fileName)
	{
		m_httpEasyServices[L"easy"] = new HttpEasyService;
		m_httpHardService = new HttpHardService;
		m_chatService = new ChatService;
		m_indicatorService = new IndicatorService;
		m_indicatorLayoutService = new IndicatorLayoutService;
		m_nodeService = new NodeService(fileName);
		m_securityService = new SecurityService;
		BaseService::AddService(m_chatService);
		BaseService::AddService(m_indicatorLayoutService);
		BaseService::AddService(m_indicatorService);
		BaseService::AddService(m_securityService);	
		BaseService::StartServer(0, 9968);
		m_nodeService->Start(fileName);
	}
}