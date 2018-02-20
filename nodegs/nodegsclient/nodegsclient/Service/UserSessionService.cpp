#include "..\\stdafx.h"
#include "UserSessionService.h"
#include "Binary.h"
#include "..\\CStr.h"

namespace OwLibCT
{
	UserSession::UserSession()
	{
		m_key = L"";
		m_userID = 0;
		m_value = L"";
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UserSessionService::UserSessionService()
	{
		m_socketID = 0;
		SetServiceID(SERVICEID_USERSESSION);
	}

	UserSessionService::~UserSessionService()
	{

	}

	int UserSessionService::GetSocketID()
	{
		return m_socketID;
	}

	void UserSessionService::SetSocketID(int socketID)
	{
		m_socketID = socketID;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int UserSessionService::AddSession(int requestID, UserSession *session)
	{
		vector<UserSession> sessions;
        sessions.push_back(*session);
        int ret = Send(FUNCTIONID_USERSESSION_ADDSESSIONS, requestID, &sessions);
        sessions.clear();
		return ret > 0 ? 1 : 0;
	}

	int UserSessionService::DeleteSession(int requestID, UserSession *session)
	{
		vector<UserSession> sessions;
        sessions.push_back(*session);
        int ret = Send(FUNCTIONID_USERSESSION_DELETESESSIONS, requestID, &sessions);
        sessions.clear();
        return ret > 0 ? 1 : 0;
	}

	int UserSessionService::GetSessions(int requestID, UserSession *session)
	{
	    vector<UserSession> sessions;
        sessions.push_back(*session);
        int ret = Send(FUNCTIONID_USERSESSION_GETSESSIONS, requestID, &sessions);
        sessions.clear();
        return ret > 0 ? 1 : 0;
	}

	int UserSessionService::GetSessions(vector<UserSession> *sessions, const char *body, int bodyLength)
	{
		Binary binary;
		binary.Write(body, bodyLength);
		int size = binary.ReadInt();
		for(int i = 0; i < size; i++)
		{
			UserSession session;
			session.m_userID = binary.ReadInt();
			binary.ReadString(session.m_key);
			binary.ReadString(session.m_value);
			sessions->push_back(session);
		}
		return 1;
	}

	void UserSessionService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
		SendToListener(message);
	}

	int UserSessionService::Send(int functionID, int requestID, vector<UserSession> *sessions)
	{
		int sessionSize = (int)sessions->size();
		int strSize = 0;
		for(int i = 0; i < sessionSize; i++)
		{
			UserSession session = (*sessions)[i];
			strSize += sizeof(int) + CStrA::GetWStringCapacity(session.m_key) + CStrA::GetWStringCapacity(session.m_value);
		}
		Binary binary;
		binary.Initialize(strSize, false);
		binary.WriteInt(sessionSize);
		for(int i = 0; i < sessionSize; i++)
		{
			UserSession session = (*sessions)[i];
			binary.WriteInt(session.m_userID);
			binary.WriteString(session.m_key);
			binary.WriteString(session.m_value);
		}
		char *ps = (char*)binary.GetBytes();
		CMessage message(GetGroupID(), GetServiceID(), functionID, GetSessionID(), requestID, m_socketID, 0, GetCompressType(), strSize, ps);
		return BaseService::Send(&message);
	}
}