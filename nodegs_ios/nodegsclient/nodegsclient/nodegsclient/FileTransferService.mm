#include "stdafx.h"
#include "CStrA.h"
#include "FileTransferService.h"
#include "HttpPostService.h"
#include "CFile.h"

namespace OwLib
{
    
    FileTransferService::FileTransferService()
    {
        m_maxID = 0;
    }
    
    FileTransferService::~FileTransferService()
    {
        m_listeners.clear();
        m_runningWork.clear();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void* MultithreadUploadWork(void *lpVoid)
    {
        void **parameters = (void**)lpVoid;
        FileTransferService *service = (FileTransferService*)parameters[0];
        service->OnUploadWork(*(int*)parameters[1], *(String*)parameters[2]);
        delete[] parameters;
        parameters = 0;
        return 0;
    }
    
    void FileTransferService::OnUploadWork(int id, const String& url)
    {
        String filePath = m_runningWork[id];
        FileTransferListener *listener = m_listeners[id];
        string surl = "", sFilePath = "";
        CStrA::wstringTostring(surl, url);
        CStrA::wstringTostring(sFilePath, filePath);
        NSString *nUrl = [NSString stringWithUTF8String: surl.c_str()];
        NSURL *URL = [NSURL URLWithString:nUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL];
        [request setValue:@"application/octet-stream" forHTTPHeaderField:@"content-type"];
        request.HTTPMethod = @"POST";//设置请求方法
        long currentLength = 0;
        long totalLength = CFileA::GetFileLength(sFilePath.c_str());
        char *str = new char[totalLength];
        memset(str, '\0', totalLength);
        ifstream fs(sFilePath, ios::in);
        while (true)
        {
            while(!fs.eof())
            {
                fs.read(str, totalLength);
            }
            fs.close();
        }
        NSData *nsBody = [NSData dataWithBytes:str length:totalLength];
        request.HTTPBody = nsBody;
        NSURLResponse * response = nil;
        NSError *error = nil;
        NSData *data = [NSURLConnection sendSynchronousRequest:request
                                             returningResponse:&response
                                                         error:&error];
        delete[] str;
        str = 0;
    }
    
    int FileTransferService::Upload(const String& url, const String& filePath, FileTransferListener *listener)
    {
        int id = m_maxID++;
        m_runningWork[id] = filePath;
        m_listeners[id] = listener;
        void **parameters = new void*[3];
        parameters[0] = (void*)this;
        parameters[1] = (void*)id;
        parameters[2] = (void*)&url;
        pthread_t hThread;
        pthread_create(&hThread, 0, MultithreadUploadWork, parameters);
        return 0;
    }
    ;}
