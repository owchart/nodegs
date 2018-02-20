/****************************************************************************************\
*                                                                                        *
* SecurityFilterService.h -  Security filter service functions, types, and definitions   *
*                                                                                        *
*               Version 1.00 ★                                                          *
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
	//自选合约类别
	class UserSecurityCategory
	{
	public:
		UserSecurityCategory();
		String m_categoryID; //类别ID
		int m_userID; //用户ID
		String m_name; //类别名称
		String m_codes; //代码
		int m_type; //类别
		int m_orderNum; //排序
	};

	//自选合约服务
	class UserSecurityService : public BaseService
	{
	private:
		int m_socketID;
	public:
		UserSecurityService();
		virtual ~UserSecurityService();
		//套接字ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		//添加类别
		int AddCategory(int requestID, UserSecurityCategory *category);
		//添加自选合约
		int AddSecurities(int requestID, UserSecurityCategory *category);
		//删除类别
		int DeleteCategory(int requestID, UserSecurityCategory *category);
		//删除自选合约
		int DeleteSecurities(int requestID, UserSecurityCategory *category);
		//获取类别
		int GetCategories(int requestID, UserSecurityCategory *category);
		//解析类别
		static int GetCategories(vector<UserSecurityCategory> *categories, const char *body, int bodyLength);
		//接收消息
		virtual void OnReceive(CMessage *message);
		//发送类别信息
		int Send(int function, int requestID, vector<UserSecurityCategory> *categories);
		//更新类别
		int UpdateCategory(int requestID, UserSecurityCategory *category);
	};
}

#endif