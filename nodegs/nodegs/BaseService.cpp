#include "stdafx.h"
#include "BaseService.h"
#include "Binary.h"
namespace OwLibSV
{
	_int64 m_downFlow = 0;

	_int64 m_upFlow = 0;

	MessageCallBack m_messageCallBack;

	map<int, BaseService*> m_services;

	WriteLogCallBack m_writeLogCallBack;

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

	///////////////////////////////////////////////////////////////////////////////////////////////////////

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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SocketArray::SocketArray()
	{
	}

	SocketArray::~SocketArray()
	{
	}

	void SocketArray::AddSocket(int socketID)
	{
		vector<int>::iterator sIter = m_sockets.begin();
		for(; sIter != m_sockets.end(); ++sIter)
		{
			if(socketID == *sIter)
			{
				return;
			}
		}
		m_sockets.push_back(socketID);
	}

	void SocketArray::GetSocketList(vector<int> *socketList)
	{
		vector<int>::iterator sIter = m_sockets.begin();
		for(; sIter != m_sockets.end(); ++sIter)
		{
			socketList->push_back(*sIter);
		}
	}

	void SocketArray::RemoveSocket(int socketID)
	{
		vector<int>::iterator sIter = m_sockets.begin();
		for(; sIter != m_sockets.end(); ++sIter)
		{
			if(socketID == *sIter)
			{
				m_sockets.erase(sIter);
				return;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	funcCloseServer p_funcCloseServer;
	funcRegisterCallBack p_funcRegisterCallBack;
	funcRegisterLog p_funcRegisterLog;
	funcSendByServer p_funcSendByServer;
	funcStartServer p_funcStartServer;

	int BaseService::CloseServer(int socketID)
	{
		return p_funcCloseServer(socketID);
	}

	void BaseService::RegisterCallBack(MessageCallBack callBack)
	{
		p_funcRegisterCallBack(callBack);
	}

	void BaseService::RegisterLog(WriteLogCallBack callBack)
	{
		p_funcRegisterLog(callBack);
	}

	int BaseService::SendByServer(int socketID, const char *str, int len)
	{
		return p_funcSendByServer(socketID, str, len);
	}

	int BaseService::StartServer(int type, int port)
	{
		return p_funcStartServer(type, port);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		return 1;
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	void BaseService::AddService(BaseService *service)
	{
		if (!m_messageCallBack)
		{
			string path = GetProgramDir() + "\\owsock_server.dll";
			HINSTANCE hdll = LoadLibraryA(path.c_str());
			p_funcCloseServer = (funcCloseServer)GetProcAddress(hdll, "CloseServer");
			p_funcRegisterCallBack = (funcRegisterCallBack)GetProcAddress(hdll, "RegisterServerCallBack");
			p_funcRegisterLog = (funcRegisterLog)GetProcAddress(hdll, "RegisterServerLog");
			p_funcSendByServer = (funcSendByServer)GetProcAddress(hdll, "SendByServer");
			p_funcStartServer = (funcStartServer)GetProcAddress(hdll, "StartServer");
			MessageCallBack callBack = &CallBack;
			m_messageCallBack = callBack;
			RegisterCallBack(callBack);
			WriteLogCallBack writeLogCallBack = &WriteServerLog;
			m_writeLogCallBack = writeLogCallBack;
			RegisterLog(m_writeLogCallBack);
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

	string BaseService::GetProgramDir()
	{
		char exeFullPath[MAX_PATH]; 
		string strPath = "";
		GetModuleFileNameA(0, exeFullPath, MAX_PATH);
		strPath = (string)exeFullPath; 
		int pos = (int)strPath.rfind('\\', strPath.length());
		return strPath.substr(0, pos);
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
		return SendByServer(socketID, str, 4);
	}

	void BaseService::OnCallBack(Binary *binary, int socketID, int localSID, int len)
	{
		int headSize = sizeof(int) * 4 + sizeof(short) * 3 + sizeof(byte) * 2;
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
			uLong cLen = 128;
			if(uncBodyLength > cLen)
			{
				cLen = uncBodyLength;
			}
			char *buffer = new char[cLen];
			memset(buffer, '\0', cLen);
			CStrA::GZDeCompress((Byte*)body, bodyLength, (Byte*)buffer, &cLen);
			delete[] body;
			body = buffer;
			uncBodyLength = cLen;
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

	void BaseService::OnClientClose(int socketID, int localSID)
	{
	}

	void BaseService::OnReceive(CMessage *message)
	{
		m_lock.Lock();
		m_compressTypes[message->m_socketID] = message->m_compressType;
		m_lock.UnLock();
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

	void BaseService::RegisterWait(int requestID, CMessage *message)
	{
		m_lock.Lock();
		m_waitMessages[requestID] = message;
		m_lock.UnLock();
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

	int BaseService::Send(CMessage *message)
	{
		Binary binary;
		char *buffer = 0;
		const char *body = message->m_body;
		int bodyLength = message->m_bodyLength;
		int uncBodyLength = bodyLength;
		m_lock.Lock();
		map<int, int>::iterator sIter = m_compressTypes.find(message->m_socketID);
		if(sIter != m_compressTypes.end())
		{
			message->m_compressType = sIter->second;
		}
		m_lock.UnLock();
		if (message->m_compressType == COMPRESSTYPE_GZIP)
        {
			uLong cLen = bodyLength;
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
		int ret = SendByServer(message->m_socketID, str, len);
		m_upFlow += ret;
		return ret;
	}

	void BaseService::SendToListener(CMessage *message)
	{
		MessageListener *listener = 0;
		void *pInvoke = 0;
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
			Sleep(10);
		}
		UnRegisterWait(requestID);
		return state;
	}

	void BaseService::WriteServerLog(int socketID, int localSID, int state, const char *log)
	{
		if(state == 2)
		{
			map<int, BaseService*>::iterator sIter = m_services.begin();
			for(; sIter != m_services.end(); ++sIter)
			{
				BaseService *service = sIter->second;
				service->OnClientClose(socketID, localSID);
			}
		}
	}
}

