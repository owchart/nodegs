/*****************************************************************************\
 *                                                                             *
 * Client.h -    Client functions, types, and definitions.                     *
 *                                                                             *
 *               Version 1.00 ★★★★★                                       *
 *                                                                             *
 *               Copyright (c) 2010-2014, Lord's client. All rights reserved.  *
 *                                                                             *
 ******************************************************************************/

#ifndef __CLIENT_H__
#define __CLIENT_H__
#pragma once
#include "stdafx.h"
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<fcntl.h>
#include "CBase64.h"
#include "Clients.h"
using namespace std;

namespace OwLibSock
{
	enum ConnectStatus
	{
		SUCCESS,
		CONNECT_PROXY_FAIL,
		NOT_CONNECT_PROXY,
		CONNECT_SERVER_FAIL
	};
    
    struct TSock4req1
	{
		char VN;
		char CD;
		unsigned short Port;
		unsigned long IPAddr;
		char other;
	};
    
	struct TSock4ans1
	{
		char VN;
		char CD;
	};
    
	struct TSock5req1
	{
		char Ver;
		char nMethods;
		char Methods;
	};
    
	struct TSock5ans1
	{
		char Ver;
		char Method;
	};
    
	struct TSock5req2
	{
		char Ver;
		char Cmd;
		char Rsv;
		char Atyp;
		char other;
	};
    
	struct TSock5ans2
	{
		char Ver;
		char Rep;
		char Rsv;
		char Atyp;
		char other;
	};
    
	struct TAuthreq
	{
		char Ver;
		char Ulen;
		char Name;
		char PLen;
		char Pass;
	};
    
	struct TAuthans
	{
		char Ver;
		char Status;
	};
    
	class Client
	{
	private:
		bool m_blnProxyServerOk;
        struct addrinfo hints;
        struct addrinfo *res;
        struct addrinfo *res0;
		string m_ip;
		u_short m_port;
		string m_proxyDomain;
		long m_proxyType;
		string m_proxyIp;
		u_short m_proxyPort;
		string m_proxyUserName;
		string m_proxyUserPwd;
        int m_timeout;
    private:
		ConnectStatus ConnectStandard();
        ConnectStatus ConnectByHttp();
		ConnectStatus ConnectBySock4();
		ConnectStatus ConnectBySock5();
		void CreateSocket();
	public:
		int m_hSocket;
		RecvMsg m_recvEvent;
		WriteClientLog m_writeLogEvent;
	public:
		Client(long proxyType, string ip, u_short port, string proxyIp, u_short proxyPort, string proxyUserName, string proxyUserPwd, string proxyDomain, int timeout);
		virtual ~Client();
    public:
		int Close(int socketID);
		ConnectStatus Connect();
		ConnectStatus ConnectProxyServer();
        string GetHostIP(const char* ip);
		static int Send(int socketID, const char *str, int len);
		void WriteLog(int socketID, int localSID, int state, const char *log);
	};
}

#endif
