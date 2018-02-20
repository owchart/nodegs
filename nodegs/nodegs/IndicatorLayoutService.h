/************************************************************************************\
*                                                                                    *
* IndicatorLayoutService.h -  Kline layout service functions, types, and definitions *
*                                                                                    *
*               Version 1.00 бя                                                      *
*                                                                                    *
*               Copyright (c) 2016-2016, Server. All rights reserved.                *
*               Created by Lord.                                                     *
*                                                                                    *
*************************************************************************************/

#ifndef __INDICATORLAYOUTSERVICE_H__
#define __INDICATORLAYOUTSERVICE_H__
#pragma once
#include "stdafx.h"
#include "BaseService.h"
#include "DataCenter.h"

#define SERVICEID_INDICATORLAYOUT 6
#define FUNCTIONID_INDICATORLAYOUT_ADDLAYOUTS 0
#define FUNCTIONID_INDICATORLAYOUT_DELETELAYOUTS 1
#define FUNCTIONID_INDICATORLAYOUT_GETLAYOUTS 2
#define FUNCTIONID_INDICATORLAYOUT_UPDATELAYOUTS 3
#define FUNCTIONID_INDICATORLAYOUT_UPDATEORDERNUM 4

namespace OwLibSV
{
	class IndicatorLayout
	{
	public:
		IndicatorLayout();
		String m_layoutID;
		String m_name;
		int m_orderNum;
		String m_text;
		int m_type;
		int m_userID;
	};

	class IndicatorLayoutService : public BaseService
	{
	private:
		string m_createTableSQL;
		String m_dataBasePath;
		map<int, SocketArray*> m_sockets;
	private:
		CLockEx m_lock;
	public:
		IndicatorLayoutService();
		virtual ~IndicatorLayoutService();
	public:
		int AddLayouts(CMessage *message);
		void CreateTable();
		int DeleteLayouts(CMessage *message);
		int GetLayouts(vector<IndicatorLayout> *layouts, const char *body, int bodyLength);
		int GetLayouts(vector<IndicatorLayout> *layouts, int userID, String indicatorID);
		int GetLayouts(CMessage *message);
		virtual void OnClientClose(int socketID, int localSID);
		virtual void OnReceive(CMessage *message);
		int Send(CMessage *message, vector<IndicatorLayout> *layouts, bool broadCast);
		int UpdateLayouts(CMessage *message);
		void UpdateLayout(IndicatorLayout *layout);
		int UpdateOrderNum(CMessage *message);
	};
}

#endif