#include "..\\stdafx.h"
#include "..\\CStr.h"
#include "UserSecurityService.h"
#include "Binary.h"

namespace OwLibCT
{
	UserSecurityCategory::UserSecurityCategory()
	{
		m_categoryID = L"";
		m_userID = 0;
		m_name = L"";
		m_codes = L"";
		m_type = 0;
		m_orderNum = 0;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UserSecurityService::UserSecurityService()
	{
		m_socketID = 0;
		SetServiceID(SERVICEID_USERSECURITY);
	}

	UserSecurityService::~UserSecurityService()
	{
	}

	int UserSecurityService::GetSocketID()
	{
		return m_socketID;
	}

	void UserSecurityService::SetSocketID(int socketID)
	{
		m_socketID = socketID;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int UserSecurityService::AddCategory(int requestID, UserSecurityCategory *category)
	{
		vector<UserSecurityCategory> categories;
		categories.push_back(*category);
		int ret = Send(FUNCTIONID_USERSECURITY_ADDCATEGORIES, requestID, &categories);
		categories.clear();
		return ret > 0 ? 1 : 0;
	}

	int UserSecurityService::AddSecurities(int requestID, UserSecurityCategory *category)
	{
		vector<UserSecurityCategory> categories;
		categories.push_back(*category);
		int ret = Send(FUNCTIONID_USERSECURITY_ADDSECURITIES, requestID, &categories);
		categories.clear();
		return ret > 0 ? 1 : 0;
	}

	int UserSecurityService::DeleteCategory(int requestID, UserSecurityCategory *category)
	{
		vector<UserSecurityCategory> categories;
		categories.push_back(*category);
		int ret = Send(FUNCTIONID_USERSECURITY_DELETECATEGORIES, requestID, &categories);
		categories.clear();
		return ret > 0 ? 1 : 0;
	}

	int UserSecurityService::DeleteSecurities(int requestID, UserSecurityCategory *category)
	{
		vector<UserSecurityCategory> categories;
		categories.push_back(*category);
		int ret = Send(FUNCTIONID_USERSECURITY_DELETESECURITIES, requestID, &categories);
		categories.clear();
		return ret > 0 ? 1 : 0;
	}

	int UserSecurityService::GetCategories(int requestID, UserSecurityCategory *category)
	{
		vector<UserSecurityCategory> categories;
		categories.push_back(*category);
		int ret = Send(FUNCTIONID_USERSECURITY_GETCATEGORIES, requestID, &categories);
		categories.clear();
		return ret > 0 ? 1 : 0;
	}

	int UserSecurityService::GetCategories(vector<UserSecurityCategory> *categories, const char *body, int bodyLength)
	{
		Binary binary;
		binary.Write(body, bodyLength);
		int size = binary.ReadInt();
		for(int i = 0; i < size; i++)
		{
			UserSecurityCategory category;
			binary.ReadString(category.m_categoryID);
			category.m_userID = binary.ReadInt();
			binary.ReadString(category.m_name);
			binary.ReadString(category.m_codes);
			category.m_type = binary.ReadInt();
			category.m_orderNum = binary.ReadInt();
			categories->push_back(category);
		}
		return 1;
	}

	void UserSecurityService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
		SendToListener(message);
	}

	int UserSecurityService::Send(int function, int requestID, vector<UserSecurityCategory> *categories)
	{
		int categoriesSize = (int)categories->size();
		int strSize = 0;
		for(int i = 0; i < categoriesSize; i++)
		{
			UserSecurityCategory category = (*categories)[i];
			strSize += sizeof(int) * 3 + CStrA::GetWStringCapacity(category.m_categoryID) + CStrA::GetWStringCapacity(category.m_codes) + CStrA::GetWStringCapacity(category.m_name) + 20;
		}
		Binary binary;
		binary.Initialize(strSize, false);
		binary.WriteInt(categoriesSize);
		for(int i = 0; i < categoriesSize; i++)
		{
			UserSecurityCategory category = (*categories)[i];
			binary.WriteString(category.m_categoryID);
			binary.WriteInt(category.m_userID);
			binary.WriteString(category.m_name);
			binary.WriteString(category.m_codes);
			binary.WriteInt(category.m_type);
			binary.WriteInt(category.m_orderNum);
		}
		char *ps = (char*)binary.GetBytes();
		CMessage message(GetGroupID(), GetServiceID(), function, GetSessionID(), requestID, m_socketID, 0, GetCompressType(), strSize, ps);
		return BaseService::Send(&message);
	}

	int UserSecurityService::UpdateCategory(int requestID, UserSecurityCategory *category)
	{
		vector<UserSecurityCategory> categories;
		categories.push_back(*category);
		int ret = Send(FUNCTIONID_USERSECURITY_UPDATECATEGORIES, requestID, &categories);
		categories.clear();
		return ret;
	}
}