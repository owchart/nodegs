/*****************************************************************************\
 *                                                                             *
 * Clients.h -    Clients functions, types, and definitions.                   *
 *                                                                             *
 *               Version 1.00 ★★★★★                                       *
 *                                                                             *
 *               Copyright (c) 2010-2014, Lord's client. All rights reserved.  *
 *                                                                             *
 ******************************************************************************/

#ifndef __CLIENTS_H__
#define __CLIENTS_H__
#pragma once
#include "stdafx.h"
using namespace std;

namespace OwLibSock
{
	typedef void (*RecvMsg)(int socketID, int localSID, const char *str, int len);
    
	typedef void (*WriteClientLog)(int socketID, int localSID, int state, const char *log);
    
	class Clients
	{
	public:
		static void RecvServerMsg(int socketID, int localSID, const char *str, int len);
		static void WriteLog(int socketID, int localSID, int state, const char *log);
	public:
		static int Close(int socketID);
		static int Connect(int proxyType, const char *ip, int port, const char *proxyIp, int proxyPort, const char *proxyUserName, const char *proxyUserPwd, const char *proxyDomain, int timeout);
		static void RegisterLog(WriteClientLog writeLogCallBack);
		static void RegisterRecv(RecvMsg recvMsgCallBack);
		static int Send(int socketID, const char *str, int len);
	};
}

#endif