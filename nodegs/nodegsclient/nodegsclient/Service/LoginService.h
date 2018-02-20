/*****************************************************************************\
*                                                                             *
* LoginService.h -  Login service functions, types, and definitions           *
*                                                                             *
*               Version 1.00 ��                                               *
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
	//��½��Ϣ
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

	//��¼����
	class LoginService : public BaseService
	{
	private:
		int m_socketID;
	public:
		LoginService();
		virtual ~LoginService();
		//�׽���ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		//������¼��Ϣ
		static int GetLoginInfos(vector<LoginInfo> *loginInfos, const char *body, int bodyLength);
		//��¼
		int Login(const String& userName, const String& passWord, int requestID);
		//�ǳ�
		int Logout(int requestID);
		//������Ϣ
		virtual void OnReceive(CMessage *message);
		//���͵�¼��Ϣ
		int Send(int functionID, int requestID, vector<LoginInfo> *loginInfos);
	};
}

#endif