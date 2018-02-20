/**************************************************************************************\
 *                                                                                      *
 * FileTransferService.h -  File transfer service functions, types, and definitions.      *
 *                                                                                      *
 *               Version 1.00 ★                                                        *
 *                                                                                      *
 *               Copyright (c) 2016-2016, Client. All rights reserved.                  *
 *               Created by TangWen.                                 CT                   *
 *                                                                                      *
 ***************************************************************************************/

#ifndef __FILETRANSFERSERVICE__H__
#define __FILETRANSFERSERVICE__H__
#pragma once
#include "stdafx.h"

namespace OwLib
{
    class FileTransferListener
    {
    public:
        virtual int OnFileTransfer(int id, long totalLength, long currentLength, int errorID, String *errorMsg)
        {
            return 0;
        }
    };
    
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
