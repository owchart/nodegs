#include "stdafx.h"
#include "HttpHardService.h"
#include "NodeService.h"
#include "DataCenter.h"

namespace OwLibSV
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HttpHardService::HttpHardService()
	{
		SetServiceID(SERVICEID_HTTPHARD);
	}

	HttpHardService::~HttpHardService()
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	void HttpHardService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
		BaseService::SendToListener(message); 	
	}

	int HttpHardService::Send(CMessage *message)
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
		int ret = -1;
		NodeService *nodeService = DataCenter::GetNodeService();
		nodeService->m_lockHttpData.Lock();
		if(nodeService->m_httpDatas.find(message->m_socketID) != nodeService->m_httpDatas.end())
		{
			nodeService->m_httpDatas[message->m_socketID]->m_resBytes = str;
			nodeService->m_httpDatas[message->m_socketID]->m_resBytesLength = len;
			ret = 1;
		}
		nodeService->m_lockHttpData.UnLock();
		return ret;
	}
}