/**************************************************************************************\
*                                                                                      *
* FileTransferService.h -  File transfer service functions, types, and definitions.      *
*                                                                                      *
*               Version 1.00 бя                                                        *
*                                                                                      *
*               Copyright (c) 2016-2016, Client. All rights reserved.                  *
*               Created by TangWen.                                 CT                   *
*                                                                                      *
***************************************************************************************/

#ifndef __FILETRANSFERSERVICE__H__
#define __FILETRANSFERSERVICE__H__
#pragma once
#include "..\\stdafx.h"
#include "BaseService.h"
#include "io.h"
#include <direct.h>
#include <fstream>
#include <sys/stat.h>
#include "..\\CurlHttp.h"

namespace OwLib
{	
	class FileTransferService
	{
	private:
		int m_maxID;
	public:
		map<int, FileTransferListener*> m_listeners;
		map<int, String> m_runningWork;
	public:
		FileTransferService();
		virtual ~FileTransferService();
	public:
		void OnUploadWork(int id, const String& url);
		int Upload(const String& url, const String& filePath, FileTransferListener *listener);
	};
}
#endif