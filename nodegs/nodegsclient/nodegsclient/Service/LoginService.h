/*****************************************************************************\
*                                                                             *
* LoginService.h -  Login service functions, types, and definitions           *
*                                                                             *
*               Version 1.00 ★                                               *
*                                                                             *
*               Copyright (c) 2016-2016, Client. All rights reserved.         *
*               Created by Lord.                                              *
*                                                                             *
*******************************************************************************/

#ifndef __LOGINSERVICECT_H__
#define __LOGINSERVICECT_H__
#pragma once
#include "..\\stdafx.h"
#include "BaseService.h"

#define SERVICEID_LOGIN 5
#define FUNCTIONID_LOGIN_LOGIN 0
#define FUNCTIONID_LOGIN_LOGOUT 1

namespace OwLibCT
{
	//登陆信息
	class LoginInfo
	{
	public:
		LoginInfo();
		int m_maxUsers;
		String m_nickName;
		String m_passWord;
		int m_sessionID;
		int m_state;
		int m_type;
		int m_userID;
		String m_userName;
	};

	//登录服务
	class LoginService : public BaseService
	{
	private:
		int m_socketID;
	public:
		LoginService();
		virtual ~LoginService();
		//套接字ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		//解析登录信息
		static int GetLoginInfos(vector<LoginInfo> *loginInfos, const char *body, int bodyLength);
		//登录
		int Login(const String& userName, const String& passWord, int requestID);
		//登出
		int Logout(int requestID);
		//接收消息
		virtual void OnReceive(CMessage *message);
		//发送登录信息
		int Send(int functionID, int requestID, vector<LoginInfo> *loginInfos);
	};
}

#endif