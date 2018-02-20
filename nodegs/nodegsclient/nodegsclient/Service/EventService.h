/*****************************************************************************\
*                                                                             *
* EventService.h -  Event service functions, types, and definitions           *
*                                                                             *
*               Version 1.00 ��                                               *
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
	//�¼�����
	class EventService : public BaseService
	{
	private:
		NativeBase *m_native;
		int m_socketID;
	public:
		EventService();
		virtual ~EventService();
		NativeBase* GetNative();
		//�׽���ID
		int GetSocketID();
		void SetSocketID(int socketID);
	public:
		// ִ��
		int SendEvent(int requestID, GridA *grid);
		// ��ȡ�������
		GridA* GetGrid(CMessage *message);
		// ��������
		virtual void OnReceive(CMessage *message);
		// ��������
		int Send(int functionID, int requestID, GridA *grid);
	};
}

#endif