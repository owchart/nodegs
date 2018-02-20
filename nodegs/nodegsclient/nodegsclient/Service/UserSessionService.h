/**************************************************************************************\
*                                                                                      *
* UserSessionService.h -  User session service functions, types, and definitions.      *
*                                                                                      *
*               Version 1.00 ★                                                        *
*                                                                                      *
*               Copyright (c) 2016-2016, Client. All rights reserved.                  *
*               Created by TangWen.                                 CT                   *
*                                                                                      *
***************************************************************************************/

#ifndef __USERSESSIONSERVICECT_H__
#define __USERSESSIONSERVICECT_H__
#pragma once
#include "..\\stdafx.h"
#include "BaseService.h"

#define SERVICEID_USERSESSION 8
#define FUNCTIONID_USERSESSION_ADDSESSIONS 0
#define FUNCTIONID_USERSESSION_DELETESESSIONS 1
#define FUNCTIONID_USERSESSION_GETSESSIONS 2

namespace OwLibCT
{
	//用户会话
	class UserSession
	{
	public:
		UserSession();
		String m_key; //键
		int m_userID; //用户ID
		String m_value; //值
	};

	//用户会话服务
	class UserSessionService : public BaseService
	{
	private:
		int m_socketID;
	public:
		UserSessionService();
		virtual ~UserSessionService();
		//套接字ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		//添加会话
		int AddSession(int requestID, UserSession *session);
		//删除会话
		int DeleteSession(int requestID, UserSession *session);
		//获取会话
		int GetSessions(int requestID, UserSession *session);
		//解析会话
		static int GetSessions(vector<UserSession> *sessions, const char *body, int bodyLength);
		//接收消息
		virtual void OnReceive(CMessage *message);
		//发送会话
		int Send(int functionID, int requestID, vector<UserSession> *sessions);
	};
}

#endif