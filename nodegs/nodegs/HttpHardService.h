/****************************************************************************************\
*																						*
* HttpHardService.h -  HttpHardService functions, types, and definitions				*
*																						*
*               Version 1.00 ¡ï¡ï¡ï¡ï¡ï													*
*																						*
*               Copyright (c) 2016-2016, Server. All rights reserved.					*
*               Created by Lord.														*
*																						*
******************************************************************************************/

#ifndef __HTTPHARDSERVICE_H__
#define __HTTPHARDSERVICE_H__
#pragma once
#include "stdafx.h"
#include <string>
#include <map>
#include "BaseService.h"

#define SERVICEID_HTTPHARD 20
#define FUNCTIONID_HTTPHARD_TEST 0

namespace OwLibSV
{
	class HttpHardService:BaseService
	{
	private:
	public:
		HttpHardService();
		virtual ~HttpHardService();
	public:
		virtual void OnReceive(CMessage *message);
		int Send(CMessage *message);
	};
}

#endif