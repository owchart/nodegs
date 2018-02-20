/*****************************************************************************\
*                                                                             *
* ChatService.h -  Login service functions, types, and definitions            *
*                                                                             *
*               Version 1.00 бя                                               *
*                                                                             *
*               Copyright (c) 2016-2016, Server. All rights reserved.         *
*               Created by QiChunyou.                                         *
*                                                                             *
*******************************************************************************/

#ifndef __CHATSERVICE__H__
#define __CHATSERVICE__H__
#pragma once
#include "stdafx.h"
#include "BaseService.h"

#define SERVICEID_CHAT 7
#define FUNCTIONID_CHAT_ENTER 0
#define FUNCTIONID_CHAT_EXIT 1
#define FUNCTIONID_CHAT_SEND 2
#define FUNCTIONID_CHAT_RECV 3


namespace OwLibSV
{
	class ChatData
	{
	public:
		ChatData();
		String m_text;
		int m_type;
		int m_userID;
	};

	class ChatService : public BaseService
	{
	private:
		map<int, int> m_sessions;
	private:
		CLockEx m_lock;
	public:
		ChatService();
		~ChatService();
	public:
		int Enter(CMessage* message);
		int Exit(CMessage* message);
		int GetChatDatas(vector<ChatData> *datas, const char* body, int bodyLength);
		virtual void OnClientClose(int socketID, int localSID);
		virtual void OnReceive(CMessage *message);
		int Send(CMessage* message, vector<ChatData>* datas);
		int SendToAllClients(CMessage *message);
	};
}
#endif