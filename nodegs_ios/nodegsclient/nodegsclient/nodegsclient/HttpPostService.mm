#include "HttpPostService.h"

namespace OwLibCT
{
    HttpPostService::HttpPostService()
    {
        m_isSyncSend = false;
        m_timeout = 0;
    }
    
    HttpPostService::~HttpPostService()
    {
    }
    
    CLockEx HttpPostService::GetLock()
    {
        return m_lock;
    }
    
    bool HttpPostService::GetIsSyncSend()
    {
        return m_isSyncSend;
    }
    
    void HttpPostService::SetIsSyncSend(bool isSyncSend)
    {
        m_isSyncSend = isSyncSend;
    }
    
    int HttpPostService::GetTimeout()
    {
        return m_timeout;
    }
    
    void HttpPostService::SetTimeout(int timeout)
    {
        m_timeout = timeout;
    }
    
    string HttpPostService::GetUrl()
    {
        return m_url;
    }
    
    void HttpPostService::SetUrl(string url)
    {
        m_url = url;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    void* AsynSend(void *lpVoid)
    {
        HttpPostService *postService = (HttpPostService*)lpVoid;
        if(postService == NULL)
        {
            return NULL;
        }
        
        vector<CMessage*> messages = postService->m_messages;
        CLockEx lock = postService->GetLock();
        
        lock.Lock();
        CMessage *message = NULL;
        if (!messages.empty())
        {
            message = messages[0];
            messages.erase(messages.begin());
        }
        lock.UnLock();
        
        if(message == NULL)
        {
            return NULL;
        }
        postService->SendRequest(message);
        return NULL;
    }
    
    void HttpPostService::OnReceive(CMessage *message)
    {
        BaseService::OnReceive(message);
        SendToListener(message);     
    }
    
    String HttpPostService::Post(const String& url)
    {
        string result = Post(url, 0, 0);
        String text = L"";
        CStrA::stringTowstring(text, result);
        return text;
    }
    
    string HttpPostService::Post(const String& url, char *sendDatas, int sendSize)
    {
        string surl = "";
        CStrA::wstringTostring(surl, url);
        NSString *nUrl = [NSString stringWithUTF8String: surl.c_str()];
        NSURL *URL = [NSURL URLWithString:nUrl];
        NSMutableURLRequest *request=[NSMutableURLRequest requestWithURL:URL];
        request.HTTPMethod = @"POST";//设置请求方法
        if(sendDatas)
        {
            NSData *nsBody = [NSData dataWithBytes:sendDatas length:sendSize];
            request.HTTPBody = nsBody;
        }
        NSURLResponse * response = nil;
        NSError *error = nil;
        NSData *data = [NSURLConnection sendSynchronousRequest:request
                                             returningResponse:&response
                                                         error:&error];
        if(error)
        {
            return "";
        }
        else
        {
            char *resData = (char *) [data bytes];
            return resData;
        }
    }
    
    String HttpPostService::Post( const String& url, const String &data )
    {
        string sData;
        CStrA::wstringTostring(sData, data);
        string str = Post(url, (char*)sData.c_str(), sData.length() + 1);
        String wResult;
        CStrA::stringTowstring(wResult, str);
        return wResult;
    }
    
    int HttpPostService::Send(CMessage *message)
    {
        if(!m_isSyncSend)
        {
            m_lock.Lock();
            CMessage *copyMessage = new CMessage;
            copyMessage->Copy(message);
            copyMessage->m_body = new char[copyMessage->m_bodyLength];
            char *str = new char[copyMessage->m_bodyLength];
            for(int i = 0; i < copyMessage->m_bodyLength; i++)
            {
                str[i] = message->m_body[i];
            }
            copyMessage->m_body = str;
        
            m_messages.push_back(copyMessage);
            m_lock.UnLock();
        
            pthread_t hThread;
            pthread_create(&hThread, 0, AsynSend, this);
            return 1;
        }else
        {
            return SendRequest(message);
        }
    }
    
    int HttpPostService::SendRequest(CMessage *message)
    {
        _int64 upFlow = BaseService::GetUpFlow();
        Binary binary;
        char *buffer = 0;
        const char *body = message->m_body;
        int bodyLength = message->m_bodyLength;
        int uncBodyLength = bodyLength;
        if (message->m_compressType == COMPRESSTYPE_GZIP)
        {
            //TODO
        }
        
        int len = sizeof(int) * 4 + bodyLength + sizeof(short) * 3 + sizeof(char) * 2;
        binary.Initialize(len, false);
        binary.WriteInt(len);
        binary.WriteShort((short)message->m_groupID);
        binary.WriteShort((short)message->m_serviceID);
        binary.WriteShort((short)message->m_functionID);
        binary.WriteInt(message->m_sessionID);
        binary.WriteInt(message->m_requestID);
        binary.WriteChar((char)message->m_state);
        binary.WriteChar((char)message->m_compressType);
        binary.WriteInt(uncBodyLength);
        binary.Write(body, bodyLength);
        char *str = (char*)binary.GetBytes();
        if(buffer)
        {
            delete[] buffer;
            buffer = 0;
        }
        
        NSString *nUrl = [NSString stringWithUTF8String: GetUrl().c_str()];
        NSURL *URL = [NSURL URLWithString:nUrl];
        NSMutableURLRequest *request=[NSMutableURLRequest requestWithURL:URL];
        request.HTTPMethod = @"POST";//设置请求方法
        NSData *nsBody = [NSData dataWithBytes:str length:len];
        request.HTTPBody = nsBody;
        
        NSURLResponse * response = nil;
        NSError *error = nil;
        NSData *data = [NSURLConnection sendSynchronousRequest:request
                                             returningResponse:&response
                                                         error:&error];
        char *resData = (char *) [data bytes];
        int ret = [data length];
        upFlow += ret;
        BaseService::SetUpFlow(upFlow);
        
        BaseService::CallBack(message->m_socketID, 0, resData, ret);
        delete message;
        message = 0;
        return ret;
    }
}
