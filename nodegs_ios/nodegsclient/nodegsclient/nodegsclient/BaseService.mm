#include "stdafx.h"
#include "BaseService.h"
#include "Binary.h"
#include "Clients.h"
using namespace OwLibSock;

namespace OwLibCT
{
	_int64 m_downFlow = 0;

	_int64 m_upFlow = 0;

	MessageCallBack m_messageCallBack;
    
	map<int, BaseService*> m_services;
    
	CMessage::CMessage()
	{
        m_groupID = 0;
		m_serviceID = 0;
		m_functionID = 0;
		m_sessionID = 0;
		m_requestID = 0;
		m_socketID = 0;
		m_state = 0;
		m_compressType = 0;
		m_bodyLength = 0;
		m_body = 0;
	}
    
	CMessage::CMessage(int groupID, int serviceID, int functionID, int sessionID, int requestID, int socketID, int state, int compressType, int bodyLength, char *body)
	{
		m_groupID = groupID;
		m_serviceID = serviceID;
		m_functionID = functionID;
		m_sessionID = sessionID;
		m_requestID = requestID;
		m_socketID = socketID;
		m_state = state;
		m_compressType = compressType;
		m_bodyLength = bodyLength;
		m_body = body;
	}
    
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	MessageListener::MessageListener()
	{
	}
    
	MessageListener::~MessageListener()
	{
		Clear();
	}
    
	void MessageListener::Add(ListenerMessageCallBack callBack, void *pInvoke)
	{
		m_callBacks.push_back(callBack);
		m_callBackInvokes.push_back(pInvoke);
	}
    
	void MessageListener::CallBack(CMessage *message)
	{
		int callBacksSize = (int)m_callBacks.size();
		for(int i = 0; i < callBacksSize; i++)
		{
			m_callBacks[i](message, m_callBackInvokes[i]);
		}
	}
    
	void MessageListener::Clear()
	{
		m_callBacks.clear();
		m_callBackInvokes.clear();
	}
    
	void MessageListener::Remove(ListenerMessageCallBack callBack)
	{
		void *pInvoke = 0;
		int pos = 0;
		vector<ListenerMessageCallBack>::iterator sIter = m_callBacks.begin();
		for(; sIter != m_callBacks.end(); ++sIter)
		{
			if(callBack == *sIter)
			{
				m_callBacks.erase(sIter);
				pInvoke = m_callBackInvokes[pos];
				break;
			}
			pos++;
		}
		vector<void*>::iterator sIter2 = m_callBackInvokes.begin();
		for(; sIter2 != m_callBackInvokes.end(); ++sIter2)
		{
			if(pInvoke == *sIter2)
			{
				m_callBackInvokes.erase(sIter2);
				break;
			}
		}
	}
    
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	int BaseService::CloseClient(int socketID)
	{
		return Clients::Close(socketID);
	}
    
	int BaseService::ConnectToServer(int proxyType, const char *ip, int port, const char *proxyIp, int proxyPort, const char *proxyUserName, const char *proxyUserPwd, const char *proxyDomain, int timeout)
	{
		return Clients::Connect(proxyType, ip, port, proxyIp, proxyPort, proxyUserName, proxyUserPwd, proxyDomain, timeout);
	}
    
	void BaseService::RegisterCallBack(MessageCallBack callBack)
	{
        Clients::RegisterRecv(callBack);
	}
    
	void BaseService::RegisterLog(WriteLogCallBack callBack)
	{
        Clients::RegisterLog(callBack);
	}
    
	int BaseService::SendByClient(int socketID, const char *str, int len)
	{
		return Clients::Send(socketID, str, len);
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	BaseService::BaseService()
	{
		m_compressType = COMPRESSTYPE_GZIP;
		m_groupID = 0;
		m_serviceID = 0;
		m_sessionID = 0;
	}
    
	BaseService::~BaseService()
	{
		m_lock.Lock();
		map<int, MessageListener*>::iterator sIter = m_listeners.begin();
		for(; sIter != m_listeners.end(); ++sIter)
		{
			delete sIter->second;
		}
		m_listeners.clear();
		m_waitMessages.clear();
		m_lock.UnLock();
	}
    
	int BaseService::GetCompressType()
	{
		return m_compressType;
	}
    
	void BaseService::SetCompressType(int compressType)
	{
		m_compressType = compressType;
	}
	
	_int64 BaseService::GetDownFlow()
	{
		return m_downFlow;
	}

	void BaseService::SetDownFlow(_int64 downFlow)
	{
		m_downFlow = downFlow;
	}
    
	int BaseService::GetGroupID()
	{
		return m_groupID;
	}
    
	void BaseService::SetGroupID(int groupID)
	{
		m_groupID = groupID;
	}
    
	int BaseService::GetServiceID()
	{
		return m_serviceID;
	}
    
	void BaseService::SetServiceID(int serviceID)
	{
		m_serviceID = serviceID;
	}
    
	int BaseService::GetSessionID()
	{
		return m_sessionID;
	}
    
	void BaseService::SetSessionID(int sessionID)
	{
		m_sessionID = sessionID;
	}
    
    _int64 BaseService::GetUpFlow()
	{
		return m_upFlow;
	}

	void BaseService::SetUpFlow(_int64 upFlow)
	{
		m_upFlow = upFlow;
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	void BaseService::AddService(BaseService *service)
	{
        if (!m_messageCallBack)
		{
			MessageCallBack callBack = &CallBack;
			m_messageCallBack = callBack;
			RegisterCallBack(callBack);
		}
		m_services[service->GetServiceID()] = service;	
	}
    
	void BaseService::CallBack(int socketID, int localSID, const char *str, int len)
	{
		m_downFlow += len;
		if (len > 4)
        {
			Binary binary;
			binary.Write(str, len);
			int head = binary.ReadInt();
			int groupID = (int)binary.ReadShort();
			int serviceID = (int)binary.ReadShort();
			BaseService *service = 0;
			if(m_services.find(serviceID) != m_services.end())
			{
				m_services[serviceID]->OnCallBack(&binary, socketID, localSID, len);
			}
		}
	}
    
	int BaseService::GetRequestID()
	{
		return m_requestID ++;
	}
    
	void BaseService::GetServices(vector<BaseService*> *services)
	{
		map<int, BaseService*>::iterator sIter = m_services.begin();
		for(; sIter != m_services.end(); ++sIter)
		{
			services->push_back(sIter->second);
		}
	}
    
    int BaseService::KeepAlive(int socketID)
    {
        Binary binary;
        binary.Initialize(4, false);
		binary.WriteInt(4);
		char *str = (char*)binary.GetBytes();
		return SendByClient(socketID, str, 4);
    }
    
    void BaseService::OnCallBack(Binary *binary, int socketID, int localSID, int len)
	{
		int headSize = sizeof(int) * 4 + sizeof(short) * 3 + sizeof(char) * 2;
		int functionID = (int)binary->ReadShort();
		int sessionID = binary->ReadInt();
		int requestID = binary->ReadInt();
		int state = (int)binary->ReadChar();
		int compressType = (int)binary->ReadChar();
		int uncBodyLength = binary->ReadInt();
		int bodyLength = len - headSize;
		char *body = new char[bodyLength];
		memset(body, '\0', bodyLength);
		binary->Read(body, bodyLength);
		if (compressType == COMPRESSTYPE_GZIP)
		{
            uLong deLen = uncBodyLength;
            char *buffer = new char[deLen];
            memset(buffer, '\0', deLen);
            CStrA::GZDeCompress((Byte*)body, bodyLength, (Byte*)buffer, &deLen);
            delete[] body;
            body = buffer;
		}
		CMessage message(GetGroupID(), GetServiceID(), functionID, sessionID, requestID, socketID, state, compressType, uncBodyLength, body);
		OnReceive(&message);
		OnWaitMessageHandle(&message);
		if(message.m_body)
		{
			delete[] body;
		}
		body = 0;
	}
    
	void BaseService::OnReceive(CMessage *message)
	{
		
	}
    
	void BaseService::OnWaitMessageHandle(CMessage *message)
	{
		if((int)m_waitMessages.size() > 0)
		{
            m_lock.Lock();
            map<int, CMessage*>::iterator sIter = m_waitMessages.find(message->m_requestID);
            if(sIter != m_waitMessages.end())
            {
                CMessage *waitMessage = sIter->second;
                waitMessage->Copy(message);
                char *newBody = new char[message->m_bodyLength];
                for (int i = 0; i < message->m_bodyLength; i++)
                {
                    newBody[i] = message->m_body[i];
                }
                waitMessage->m_body = newBody;
            }
            m_lock.UnLock();
		}
	}
    
	void BaseService::RegisterListener(int requestID, ListenerMessageCallBack callBack, void *pInvoke)
	{
		m_lock.Lock();
		MessageListener *listener = 0;
		map<int, MessageListener*>::iterator sIter = m_listeners.find(requestID);
		if(sIter != m_listeners.end())
		{
			listener = sIter->second;
		}
		else
		{
			listener = new MessageListener;
			m_listeners[requestID] = listener;
		}
		listener->Add(callBack, pInvoke);
		m_lock.UnLock();
	}
    
	void BaseService::RegisterWait(int requestID, CMessage *message)
	{
		m_lock.Lock();
		m_waitMessages[requestID] = message;
		m_lock.UnLock();
	}
    
	int BaseService::Send(CMessage *message)
	{
		Binary binary;
		char *buffer = 0;
		const char *body = message->m_body;
		int bodyLength = message->m_bodyLength;
		int uncBodyLength = bodyLength;
		if (message->m_compressType == COMPRESSTYPE_GZIP)
        {
            //uLong cLen = uncBodyLength;
            uLong cLen = 128;
            if(uncBodyLength > cLen)
            {
                cLen = uncBodyLength;
            }
            buffer = new char[cLen];
            memset(buffer, '\0', cLen);
            CStrA::GZCompress((Byte*)body, bodyLength, (Byte*)buffer, &cLen);
            body = buffer;
            bodyLength = (int)cLen;
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
		int ret = SendByClient(message->m_socketID, str, len);
		m_upFlow += ret;
		return ret;
	}
    
	void BaseService::SendToListener(CMessage *message)
	{
		MessageListener *listener = 0;
		m_lock.Lock();
		map<int, MessageListener*>::iterator sIter = m_listeners.find(message->m_requestID);
		if(sIter != m_listeners.end())
		{
			listener = sIter->second;
		}
		m_lock.UnLock();
		if(listener)
		{
			listener->CallBack(message);
		}
	}
    
	void BaseService::UnRegisterListener(int requestID)
	{
		m_lock.Lock();
		map<int, MessageListener*>::iterator sIter = m_listeners.find(requestID);
		if(sIter != m_listeners.end())
		{
			delete sIter->second;
			m_listeners.erase(sIter);
		}
		m_lock.UnLock();
	}
    
	void BaseService::UnRegisterListener(int requestID, ListenerMessageCallBack callBack)
	{
		m_lock.Lock();
		map<int, MessageListener*>::iterator sIter = m_listeners.find(requestID);
		if(sIter != m_listeners.end())
		{
			sIter->second->Remove(callBack);
		}
		m_lock.UnLock();
	}
    
	void BaseService::UnRegisterWait(int requestID)
	{
		m_lock.Lock();
		map<int, CMessage*>::iterator sIter = m_waitMessages.find(requestID);
		if(sIter != m_waitMessages.end())
		{
			m_waitMessages.erase(sIter);
		}
		m_lock.UnLock();
	}
    
	int BaseService::WaitMessage(int requestID, int timeout)
	{
		int state = 0;
		while(timeout > 0)
		{
			m_lock.Lock();
			map<int, CMessage*>::iterator sIter = m_waitMessages.find(requestID);
			if(sIter != m_waitMessages.end())
			{
				CMessage *message = sIter->second;
				if(message->m_bodyLength > 0)
				{
					state = 1;
					m_lock.UnLock();
					break;
				}
			}
			else
			{
				m_lock.UnLock();
				break;
			}
			m_lock.UnLock();
			timeout -= 10;
			usleep(10000);
		}
		UnRegisterWait(requestID);
		return state;
	}
}
