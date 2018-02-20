#include "stdafx.h"
#include "LoginService.h"
#include "CStrA.h"
#include "Binary.h"

namespace OwLibCT
{
	LoginService::LoginService()
	{
		m_socketID = 0;
		SetServiceID(SERVICEID_LOGIN);
	}
    
	LoginService::~LoginService()
	{
	}
    
	int LoginService::GetSocketID()
	{
		return m_socketID;
	}
    
	void LoginService::SetSocketID(int socketID)
	{
		m_socketID = socketID;
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
	int LoginService::GetLoginInfos(int requestID)
	{
		LoginInfo loginInfo;
        loginInfo.m_sessionID = GetSessionID();
		vector<LoginInfo> loginInfos;
		loginInfos.push_back(loginInfo);
        int ret = Send(FUNCTIONID_LOGIN_GETLOGININFOS, requestID, &loginInfos);
		loginInfos.clear();
		return ret > 0 ? 1 : 0;
	}
    
	int LoginService::GetLoginInfos(vector<LoginInfo> *loginInfos, const char *body, int bodyLength)
	{
		Binary binary;
		binary.Write(body, bodyLength);
		int size = binary.ReadInt();
		for(int i = 0; i < size; i++)
		{
			LoginInfo loginInfo;
			loginInfo.m_userID = binary.ReadInt();
			binary.ReadString(loginInfo.m_userName);
			binary.ReadString(loginInfo.m_passWord);
			binary.ReadString(loginInfo.m_nickName);
			loginInfo.m_type = binary.ReadInt();
			loginInfo.m_state = binary.ReadInt();
			loginInfo.m_sessionID = binary.ReadInt();
			loginInfos->push_back(loginInfo);
		}
		return 1;
	}
    
	int LoginService::Login(string userName, string passWord, int requestID)
	{
	    LoginInfo loginInfo;
        loginInfo.m_userName = userName;
        loginInfo.m_passWord = passWord;
		vector<LoginInfo> loginInfos;
		loginInfos.push_back(loginInfo);
        int ret = Send(FUNCTIONID_LOGIN_LOGIN, requestID, &loginInfos);
		loginInfos.clear();
		return ret > 0 ? 1 : 0;
	}
    
	int LoginService::Logout(int requestID)
	{
	    LoginInfo loginInfo;
        loginInfo.m_sessionID = GetSessionID();
		vector<LoginInfo> loginInfos;
		loginInfos.push_back(loginInfo);
        int ret = Send(FUNCTIONID_LOGIN_LOGOUT, requestID, &loginInfos);
		loginInfos.clear();
		return ret > 0 ? 1 : 0;
	}
    
	void LoginService::OnReceive(CMessage *message)
	{
        BaseService::OnReceive(message);
		SendToListener(message);
	}
    
	int LoginService::Send(int functionID, int requestID, vector<LoginInfo> *loginInfos)
	{
		int loginInfosSize = (int)loginInfos->size();
		int strSize = 0;
		for(int i = 0; i < loginInfosSize; i++)
		{
			LoginInfo loginInfo = (*loginInfos)[i];
			strSize += sizeof(int) * 4 + CStrA::GetStringCapacity(loginInfo.m_userName) + CStrA::GetStringCapacity(loginInfo.m_passWord) + CStrA::GetStringCapacity(loginInfo.m_nickName) + 20;
		}
		Binary binary;
		binary.Initialize(strSize, false);
		binary.WriteInt(loginInfosSize);
		for(int i = 0; i < loginInfosSize; i++)
		{
			LoginInfo loginInfo = (*loginInfos)[i];
			binary.WriteInt(loginInfo.m_userID);
			binary.WriteString(loginInfo.m_userName);
			binary.WriteString(loginInfo.m_passWord);
			binary.WriteString(loginInfo.m_nickName);
			binary.WriteInt(loginInfo.m_type);
			binary.WriteInt(loginInfo.m_state);
			binary.WriteInt(loginInfo.m_sessionID);
		}
		char *ps = (char*)binary.GetBytes();
		CMessage message(GetGroupID(), GetServiceID(), functionID, GetSessionID(), requestID, m_socketID, 0, GetCompressType(), strSize, ps);
		return BaseService::Send(&message);
	}
}
