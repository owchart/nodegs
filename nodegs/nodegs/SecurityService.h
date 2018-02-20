/***************************************************************************\
*                                                                           *
* SecurityService.h -  Security service functions, types, and definitions   *
*                                                                           *
*               Version 1.00 бя                                             *
*                                                                           *
*               Copyright (c) 2016-2016, Server. All rights reserved.       *
*               Created by Lord.                                            *
*                                                                           *
****************************************************************************/

#ifndef __SECURITYSERVICE_H__
#define __SECURITYSERVICE_H__
#pragma once
#include "stdafx.h"
#include "BaseService.h"

#define SERVICEID_SECURITY 3
#define FUNCTIONID_SECURITY_GETSECURITIES 0
#define FUNCTIONID_SECURITY_GETSECURITIESBYID 1

namespace OwLibSV
{
	class Security
	{
	public:
		Security();
		String m_code;
		String m_name;
		String m_pingyin;
		int m_status;
		string m_sCode;
		int m_type;
	};

	class SecurityService : public BaseService
	{
	private:
		string m_createTableSQL;
		String m_dataBasePath;
	public:
		SecurityService();
		virtual ~SecurityService();
	public:
		void CreateTable();
		int GetSecurities(vector<Security> *securities, String filter);
		int GetSecurities(CMessage *message);
		void ImportSecurities(string path);
		virtual void OnReceive(CMessage *message);
		int Send(CMessage *message, vector<Security> *securities);
	};
}

#endif