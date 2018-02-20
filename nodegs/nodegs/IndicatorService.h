/************************************************************************************\
*                                                                                    *
* IndicatorService.h -  Indicator service functions, types, and definitions          *
*                                                                                    *
*               Version 1.00 бя                                                      *
*                                                                                    *
*               Copyright (c) 2016-2016, Server. All rights reserved.                *
*               Created by Lord.                                                     *
*                                                                                    *
*************************************************************************************/

#ifndef __INDICATORSERVICE_H__
#define __INDICATORSERVICE_H__
#pragma once
#include "stdafx.h"
#include "BaseService.h"
#include "DataCenter.h"

#define SERVICEID_INDICATOR 4
#define FUNCTIONID_INDICATOR_ADDINDICATORS 0
#define FUNCTIONID_INDICATOR_DELETEINDICATORS 1
#define FUNCTIONID_INDICATOR_GETINDICATORS 2
#define FUNCTIONID_INDICATOR_UPDATEINDICATORS 3
#define FUNCTIONID_INDICATOR_UPDATEORDERNUM 4

namespace OwLibSV
{
	class Indicator
	{
	public:
		Indicator();
		String m_category;
		String m_coordinate;
		String m_description;
		int m_digit;
		String m_indicatorID;
		String m_name;
		int m_orderNum;
		int m_paintType;
		String m_parameters;
		String m_password;
		String m_specialCoordinate;
		String m_text;
		int m_type;
		int m_usePassword;
		int m_userID;
		int m_version;
	};

	class IndicatorService : public BaseService
	{
	private:
		string m_createTableSQL;
		String m_dataBasePath;
		map<int, SocketArray*> m_sockets;
	private:
		CLockEx m_lock;
	public:
		IndicatorService();
		virtual ~IndicatorService();
	public:
		int AddIndicators(CMessage *message);
		void CreateTable();
		int DeleteIndicators(CMessage *message);
		int GetIndicators(vector<Indicator> *indicators, const char *body, int bodyLength);
		int GetIndicators(vector<Indicator> *indicators, int userID, String indicatorID);
		int GetIndicators(CMessage *message);
		virtual void OnClientClose(int socketID, int localSID);
		virtual void OnReceive(CMessage *message);
		int Send(CMessage *message, vector<Indicator> *indicators, bool broadCast);
		int UpdateIndicators(CMessage *message);
		void UpdateIndicator(Indicator *indicator);
		int UpdateOrderNum(CMessage *message);
	};
}

#endif