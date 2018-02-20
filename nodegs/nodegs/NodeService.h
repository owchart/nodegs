/*****************************************************************************\
*                                                                             *
* NodeService.h -  Node service functions, types, and definitions             *
*                                                                             *
*               Version 1.00 ¡ï¡ï¡ï¡ï¡ï                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Server. All rights reserved.         *
*               Created by Lord.                                              *
*                                                                             *
*******************************************************************************/

#ifndef __NODESERVICE_H__
#define __NODESERVICE_H__
#pragma once
#include "stdafx.h"
#include "Binary.h"
#include <string>
#include <map>
#pragma comment(lib, "ws2_32.lib")
#include "winsock2.h"
#include "HttpEasyService.h"
using namespace std;

namespace OwLibSV
{
	class HttpData
	{
	public:
		HttpData();
		~HttpData();
		SOCKADDR_IN m_addr;
		char *m_body;
        bool m_close;
		int m_contentLength;
        String m_contentType;
        String m_method;
		map<String, String> m_parameters;
		String m_remoteIP;
        int m_remotePort;
		char* m_resBytes;
		int m_resBytesLength;
		String m_resStr;
		SOCKET m_socket;
		int m_statusCode;	
        String m_url;
	};

	class NodeService
	{
	public:
		SOCKADDR_IN m_addr;
		string m_fileName;
		SOCKET m_hSocket;
		map<int, HttpData*> m_httpDatas;
		stack<CIndicator*> m_indicators;
		CIndicator *m_indicator;
		CLock m_lock;
		CLock m_lockHttpData;
		NativeBase *m_native;
		int m_port;
		String m_script;
		bool m_useScript;
	public:
		NodeService(string fileName);
		virtual ~NodeService();
		CIndicator* GetIndicator();
		NativeBase* GetNative();
		int GetPort();
		void SetPort(int port);
		String GetScript();
		void SetScript(String script);
		SOCKET GetSocket();
		bool GetUseScript();
		void SetHttpData(int socketID, HttpData*);
		CIndicator* PopCIndicator();
	public:
		void CheckScript();
		int Close(int socketID);		
		int Start(string fileName);
	};
}

#endif