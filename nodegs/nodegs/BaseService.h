/*****************************************************************************\
*                                                                             *
* BaseService.h -  Base service functions, types, and definitions             *
*                                                                             *
*               Version 1.00 ¡ï¡ï¡ï¡ï¡ï                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Server. All rights reserved.         *
*               Created by Lord.                                              *
*                                                                             *
*******************************************************************************/

#ifndef __BASESERVICE_H__
#define __BASESERVICE_H__
#pragma once
#include "stdafx.h"
#include "Binary.h"
#include <string>
#include <map>
using namespace std;

#define  COMPRESSTYPE_NONE			  0
#define  COMPRESSTYPE_GZIP			  1

static int m_requestID = 10000;

namespace OwLibSV
{	
	struct CMessage
	{
	public:
		CMessage();
		CMessage(int groupID, int serviceID, int functionID, int sessionID, int requestID, int socketID, int state, int compressType, int bodyLength, char *body);
		int m_groupID;   
		int m_serviceID;   
		int m_functionID;  
		int m_sessionID;   
		int m_requestID;   
		int m_socketID; 
		int m_state;
		int m_compressType;
		int m_bodyLength;  
		char *m_body; 
		void Copy(CMessage *message)
		{
			m_groupID = message->m_groupID;
			m_serviceID = message->m_serviceID;
			m_functionID = message->m_functionID;
			m_sessionID = message->m_sessionID;
			m_requestID= message->m_requestID;
			m_socketID = message->m_socketID;
			m_state = message->m_state;
			m_compressType = message->m_compressType;
			m_bodyLength = message->m_bodyLength;
			m_body = message->m_body;
		}
	};

	typedef void (__stdcall *MessageCallBack)(int socketID, int localSID, const char *str, int len);

	typedef void (__stdcall *WriteLogCallBack)(int socketID, int localSID, int state, const char *log);

	typedef void (__stdcall *ListenerMessageCallBack)(CMessage *message, void *pInvoke);

	typedef int (*funcCloseServer)(int);

	typedef void (*funcRegisterCallBack)(MessageCallBack);

	typedef void (*funcRegisterLog)(WriteLogCallBack);

	typedef int (*funcSendByServer)(int, const char*, int);

	typedef int (*funcStartServer)(int, int);

	class MessageListener
	{
	private:
		vector<ListenerMessageCallBack> m_callBacks;
		vector<void*> m_callBackInvokes;
	public:
		MessageListener();
		virtual ~MessageListener();
	public:
		void Add(ListenerMessageCallBack callBack, void *pInvoke);
		void CallBack(CMessage *message);
		void Clear();
		void Remove(ListenerMessageCallBack callBack);
	};

	class SocketArray
	{
	private:
		vector<int> m_sockets;
	public:
		SocketArray();
		virtual ~SocketArray();
	public:
		void AddSocket(int socketID);
		void GetSocketList(vector<int> *socketList);
		void RemoveSocket(int socketID);
	};

	class CLockEx
	{
	public:
		CLockEx()
		{ 
			InitializeCriticalSection(&m_cs); 
		}
		virtual ~CLockEx()
		{
			DeleteCriticalSection(&m_cs); 
		}
		void Lock()
		{ 
			EnterCriticalSection(&m_cs);
		}
		void UnLock()
		{ 
			LeaveCriticalSection(&m_cs); 
		}
	private:
		CRITICAL_SECTION m_cs;
	};

	class BaseService
	{
	protected:
		int m_compressType;
		map<int, int> m_compressTypes;
		int m_groupID;
		map<int, MessageListener*> m_listeners;
		int m_serviceID;
		int m_sessionID;
		map<int, CMessage*> m_waitMessages;
	protected:
		CLockEx m_lock;
	public:
		static int CloseServer(int socketID);
		static void RegisterCallBack(MessageCallBack callBack);
		static void RegisterLog(WriteLogCallBack callBack);
		static int SendByServer(int socketID, const char *str, int len);
		static int StartServer(int type, int port);
	public:
		BaseService();
		virtual ~BaseService();
		int GetCompressType();
		void SetCompressType(int compressType);
		static _int64 GetDownFlow();
		static void SetDownFlow(_int64 downFlow);
		int GetGroupID();
		void SetGroupID(int groupID);
		int GetServiceID();
		void SetServiceID(int serviceID);
		int GetSessionID();
		void SetSessionID(int sessionID);
		static _int64 GetUpFlow();
		static void SetUpFlow(_int64 upFlow);
	public:
		static void AddService(BaseService *service);
		static void __stdcall CallBack(int socketID, int localSID, const char *str, int len);
		static string GetProgramDir();
		static int GetRequestID();
		static void GetServices(vector<BaseService*> *services);
		virtual int KeepAlive(int socketID);
		virtual void OnCallBack(Binary *binary, int socketID, int localSID, int len);
		virtual void OnClientClose(int socketID, int localSID);
		virtual void OnReceive(CMessage *message);
		virtual void OnWaitMessageHandle(CMessage *message);
		virtual void RegisterListener(int requestID, ListenerMessageCallBack callBack, void *pInvoke);
		virtual void RegisterWait(int requestID, CMessage *message);
		virtual int Send(CMessage *message);
		virtual void SendToListener(CMessage *message);
		virtual void UnRegisterListener(int requestID);
		virtual void UnRegisterListener(int requestID, ListenerMessageCallBack callBack);
		virtual void UnRegisterWait(int requestID);
		virtual int WaitMessage(int requestID, int timeout);
		static void __stdcall WriteServerLog(int socketID, int localSID, int state, const char *log);
	};
}

#endif