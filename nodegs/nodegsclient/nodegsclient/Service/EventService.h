/*****************************************************************************\
*                                                                             *
* EventService.h -  Event service functions, types, and definitions           *
*                                                                             *
*               Version 1.00 ★                                               *
*                                                                             *
*               Copyright (c) 2016-2016, Client. All rights reserved.         *
*               Created by Lord.                                              *
*                                                                             *
*******************************************************************************/

#ifndef __EVENTSERVICECT_H__
#define __EVENTSERVICECT_H__
#pragma once
#include "..\\stdafx.h"
#include "BaseService.h"

#define SERVICEID_EVENT 30
#define FUNCTIONID_EXCUTEID 0

namespace OwLibCT
{
	//事件服务
	class EventService : public BaseService
	{
	private:
		NativeBase *m_native;
		int m_socketID;
	public:
		EventService();
		virtual ~EventService();
		NativeBase* GetNative();
		//套接字ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		// 执行
		int SendEvent(int requestID, GridA *grid);
		// 获取表格数据
		GridA* GetGrid(CMessage *message);
		// 接收数据
		virtual void OnReceive(CMessage *message);
		// 发送数据
		int Send(int functionID, int requestID, GridA *grid);
	};
}

#endif