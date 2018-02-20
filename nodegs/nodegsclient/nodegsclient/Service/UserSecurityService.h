/****************************************************************************************\
*                                                                                        *
* SecurityFilterService.h -  Security filter service functions, types, and definitions   *
*                                                                                        *
*               Version 1.00 ��                                                          *
*                                                                                        *
*               Copyright (c) 2016-2016, Client. All rights reserved.                    *
*               Created by Lord.                                                         *
*                                                                                        *
*****************************************************************************************/

#ifndef __USERSECURITYSERVICE_H__
#define __USERSECURITYSERVICE_H__
#pragma once
#include "..\\stdafx.h"
#include "BaseService.h"

#define SERVICEID_USERSECURITY 1
#define FUNCTIONID_USERSECURITY_ADDCATEGORIES 0
#define FUNCTIONID_USERSECURITY_DELETECATEGORIES 1
#define FUNCTIONID_USERSECURITY_GETCATEGORIES 2
#define FUNCTIONID_USERSECURITY_UPDATECATEGORIES 3
#define FUNCTIONID_USERSECURITY_ADDSECURITIES 4
#define FUNCTIONID_USERSECURITY_DELETESECURITIES 5

namespace OwLibCT
{
	//��ѡ��Լ���
	class UserSecurityCategory
	{
	public:
		UserSecurityCategory();
		String m_categoryID; //���ID
		int m_userID; //�û�ID
		String m_name; //�������
		String m_codes; //����
		int m_type; //���
		int m_orderNum; //����
	};

	//��ѡ��Լ����
	class UserSecurityService : public BaseService
	{
	private:
		int m_socketID;
	public:
		UserSecurityService();
		virtual ~UserSecurityService();
		//�׽���ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		//������
		int AddCategory(int requestID, UserSecurityCategory *category);
		//�����ѡ��Լ
		int AddSecurities(int requestID, UserSecurityCategory *category);
		//ɾ�����
		int DeleteCategory(int requestID, UserSecurityCategory *category);
		//ɾ����ѡ��Լ
		int DeleteSecurities(int requestID, UserSecurityCategory *category);
		//��ȡ���
		int GetCategories(int requestID, UserSecurityCategory *category);
		//�������
		static int GetCategories(vector<UserSecurityCategory> *categories, const char *body, int bodyLength);
		//������Ϣ
		virtual void OnReceive(CMessage *message);
		//���������Ϣ
		int Send(int function, int requestID, vector<UserSecurityCategory> *categories);
		//�������
		int UpdateCategory(int requestID, UserSecurityCategory *category);
	};
}

#endif