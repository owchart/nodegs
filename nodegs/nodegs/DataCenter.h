/*****************************************************************************\
*                                                                             *
* LoginService.h -  Login service functions, types, and definitions           *
*                                                                             *
*               Version 1.00 бя                                               *
*                                                                             *
*               Copyright (c) 2016-2016, Server. All rights reserved.         *
*               Created by Lord.                                              *
*                                                                             *
*******************************************************************************/

#ifndef __DATACENTER_H__
#define __DATACENTER_H__
#pragma once
#include "CFile.h"
#include "BaseService.h"
#include "ChatService.h"
#include "HttpEasyService.h"
#include "HttpHardService.h"
#include "IndicatorLayoutService.h"
#include "IndicatorService.h"
#include "NodeService.h"
#include "SecurityService.h"

namespace OwLibSV
{
	class ChatService;
	class HttpEasyService;
	class HttpHardService;
	class IndicatorLayoutService;
	class IndicatorService;
	class NodeService;
	class SecurityService;

	class DataCenter
	{
	public:
		static ChatService* GetChatService();
		static map<String, HttpEasyService*> GetHttpEasyServices();
		static HttpHardService* GetHttpHardService();
		static IndicatorService* GetIndicatorService();
		static IndicatorLayoutService* GetIndicatorLayoutService();
		static NodeService* GetNodeService();
		static SecurityService* GetSecurityService();
	public:
		static string GetAppDir();
		static bool IsAppAlive();
		static void StartService(string fileName);
	};
}

#endif