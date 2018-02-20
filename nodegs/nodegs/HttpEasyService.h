/****************************************************************************************\
*																						*
* HttpEasyService.h -  HttpEasyService functions, types, and definitions				*
*																						*
*               Version 1.00 ¡ï¡ï¡ï¡ï¡ï													*
*																						*
*               Copyright (c) 2016-2016, Server. All rights reserved.					*
*               Created by Lord.														*
*																						*
******************************************************************************************/

#ifndef __HTTPEASYSERVICE_H__
#define __HTTPEASYSERVICE_H__
#pragma once
#include "stdafx.h"
#include <string>
#include <map>
#include "NodeService.h"

using namespace std;

namespace OwLibSV
{
	class HttpData;

	class HttpEasyService
	{
	private:
	public:
		HttpEasyService();
		virtual ~HttpEasyService();
	public:
		virtual void OnReceive(HttpData* data);
	};
}

#endif