/*****************************************************************************\
 *                                                                             *
 *  HttpPostService.cs -  HttpPostService service functions, types, and definitions.   *
 *                                                                             *
 *               Version 1.00 ★                                               *
 *                                                                             *
 *               Copyright (c) 2016-2016, Client. All rights reserved.         *
 *               Created by Lord.                                              *
 *                                                                             *
 *******************************************************************************/

#ifndef __HTTPPOSTSERVICE_H__
#define __HTTPPOSTSERVICE_H__
#pragma once
#include "stdafx.h"
#include "BaseService.h"
#include "CFile.h"

#define SERVICEID_HTTPPOST 20
#define FUNCTIONID_HTTPPOST_TEST 0

namespace OwLibCT
{
    class HttpPostService : public BaseService
    {
    private:
        bool m_isSyncSend;
        CLockEx m_lock;
        string m_url;
        int m_timeout;
    public:
        vector<CMessage*> m_messages;
        HttpPostService();
        virtual ~HttpPostService();
        bool GetIsSyncSend();
        void SetIsSyncSend(bool isSyncSend);
        int GetTimeout();
        void SetTimeout(int timeout);
        string GetUrl();
        void SetUrl(string url);
    public:
        CLockEx GetLock();
        virtual void OnReceive(CMessage *message);
        String Post(const String& url);
        string Post(const String& url, char *sendDatas, int sendSize);
        String Post(const String& url, const String &data);
        virtual int Send(CMessage *message);
        int SendRequest(CMessage *message);
    };
}

#endif
