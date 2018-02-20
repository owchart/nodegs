/**************************************************************************************\
*                                                                                      *
* UserSessionService.h -  User session service functions, types, and definitions.      *
*                                                                                      *
*               Version 1.00 ��                                                        *
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
	//�û��Ự
	class UserSession
	{
	public:
		UserSession();
		String m_key; //��
		int m_userID; //�û�ID
		String m_value; //ֵ
	};

	//�û��Ự����
	class UserSessionService : public BaseService
	{
	private:
		int m_socketID;
	public:
		UserSessionService();
		virtual ~UserSessionService();
		//�׽���ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		//��ӻỰ
		int AddSession(int requestID, UserSession *session);
		//ɾ���Ự
		int DeleteSession(int requestID, UserSession *session);
		//��ȡ�Ự
		int GetSessions(int requestID, UserSession *session);
		//�����Ự
		static int GetSessions(vector<UserSession> *sessions, const char *body, int bodyLength);
		//������Ϣ
		virtual void OnReceive(CMessage *message);
		//���ͻỰ
		int Send(int functionID, int requestID, vector<UserSession> *sessions);
	};
}

#endif