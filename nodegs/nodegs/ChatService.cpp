#include "stdafx.h"
#include "ChatService.h"
#include "CStr.h"
#include "Binary.h"

namespace OwLibSV
{
	ChatData::ChatData()
	{
		m_text = L"";
		m_type = 0;
		m_userID = 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ChatService::ChatService()
	{
		SetServiceID(SERVICEID_CHAT);
	}

	ChatService::~ChatService()
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	int ChatService::Enter(CMessage* message)
	{
		vector<ChatData> datas;
        GetChatDatas(&datas, message->m_body, message->m_bodyLength);
		int ret = Send(message, &datas);
		int sessionID = message->m_sessionID;
		if(ret != -1)
		{
			m_lock.Lock();
			m_sessions[sessionID] = message->m_socketID;
			m_lock.UnLock();
		}
		else 
		{
			m_lock.Lock();
			map<int, int>::iterator sIter = m_sessions.find(sessionID);
			if(sIter != m_sessions.end())
			{
				m_sessions.erase(sIter);
			}			
			m_lock.UnLock();
		}
		datas.clear();		
		return ret;
	}
	
	int ChatService::Exit(CMessage* message)
	{
		vector<ChatData> datas;
		GetChatDatas(&datas, message->m_body, message->m_bodyLength);
		int ret = Send(message, &datas);
		m_lock.Lock();
		map<int, int>::iterator sIter = m_sessions.find(message->m_sessionID);
		if(sIter != m_sessions.end())
		{
			m_sessions.erase(sIter);
		}	
		m_lock.UnLock();
        return ret;
	}
	
	int ChatService::GetChatDatas(vector<ChatData> *datas, const char* body, int bodyLength)
	{
		Binary binary;
		binary.Write(body, bodyLength);
		int chatSize = binary.ReadInt();
		for(int i = 0; i < chatSize; i++)
		{
			ChatData chat;
			chat.m_userID = binary.ReadInt();
			chat.m_type = (int)binary.ReadChar();
			binary.ReadString(chat.m_text);
			datas->push_back(chat);
		}
		return 1;
	}

	void ChatService::OnClientClose(int socketID, int localSID)
	{
		BaseService::OnClientClose(socketID, localSID);
		m_lock.Lock();
        vector<int> removeSessions;
		map<int, int>::iterator sIter = m_sessions.begin();
		for(; sIter != m_sessions.end(); ++sIter)
		{
			int sid = sIter->second;
            if (sid == socketID)
            {
				removeSessions.push_back(sIter->first);
            }
		}
        int removeSessionsSize = (int)removeSessions.size();
        for (int i = 0; i < removeSessionsSize; i++)
        {
			sIter = m_sessions.find(removeSessions[i]);
			if(sIter != m_sessions.end())
			{
				m_sessions.erase(sIter);
			}
        }
		removeSessions.clear();
		m_lock.UnLock();
	}
	
	void ChatService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
		switch (message->m_functionID)
        {
            case FUNCTIONID_CHAT_ENTER:
                Enter(message);
                break;
            case FUNCTIONID_CHAT_EXIT:
                Exit(message);
                break;
			case FUNCTIONID_CHAT_SEND:
				SendToAllClients(message);
				break;
			default:
				break;						
        }
	}
	
	int ChatService::Send(CMessage* message, vector<ChatData>* datas)
	{	
		int chatsize = (int)datas->size();
		int strSize = sizeof(int);
		for(int i = 0; i < chatsize; i++)
		{
			ChatData chat = datas->at(i);
			strSize += sizeof(int) * 2 + CStrA::GetStringCapacity(chat.m_text);
		}
		Binary binary;
		binary.Initialize(strSize, false);
		binary.WriteInt(chatsize);
		for(int i = 0; i < chatsize; i++)
		{
			ChatData chat = datas->at(i);
            binary.WriteInt(chat.m_userID);
			binary.WriteChar((char)chat.m_type);
			binary.WriteString(chat.m_text);
		}
		char *ps = (char*)binary.GetBytes();
		message->m_bodyLength = strSize;
		message->m_body = ps;
		int ret = BaseService::Send(message);
		return ret;
	}
	
	int ChatService::SendToAllClients(CMessage *message)
	{
		vector<ChatData> datas;
		GetChatDatas(&datas, message->m_body, message->m_bodyLength);
		message->m_functionID = FUNCTIONID_CHAT_RECV;
		m_lock.Lock();
		vector<int> removelist;
		map<int, int>::iterator sIter = m_sessions.begin();
		for(; sIter != m_sessions.end(); sIter++)
		{
			message->m_socketID = sIter->second;
			int ret = BaseService::Send(message);
			if(ret == -1)
			{
				removelist.push_back(sIter->first);
			}
		}
		int listsize = (int)removelist.size();
		for(int i = 0; i < listsize; i++)
		{
			m_sessions.erase(removelist[i]); 
		}		
		m_lock.UnLock();
        datas.clear();
        return 1;
	}
}