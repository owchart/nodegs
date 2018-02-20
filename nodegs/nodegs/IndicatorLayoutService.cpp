#include "stdafx.h"
#include "IndicatorLayoutService.h"
#include "CStr.h"
#include "Binary.h"
#include "CFile.h"
#include "sqlite3x.hpp"
using namespace sqlite3x;

namespace OwLibSV
{
	IndicatorLayout::IndicatorLayout()
	{
		m_layoutID = L"";
		m_name = L"";
		m_orderNum = 0;
		m_text = L"";
		m_type = 0;
		m_userID = 0;
	}

	IndicatorLayoutService::IndicatorLayoutService()
	{
		m_createTableSQL = "CREATE TABLE INDICATORLAYOUT(LAYOUTID PRIMARY KEY, USERID INTEGER, NAME, TEXT, TYPE INTEGER, ORDERNUM INTEGER, CREATETIME DATE, MODIFYTIME DATE)";
		string dataBasePath = DataCenter::GetAppDir() + "\\indicatorlayouts.db";
		CStrA::stringTowstring(m_dataBasePath, dataBasePath);
		if(!CFileA::IsFileExist(dataBasePath.c_str()))
		{
			CreateTable();
		}
		SetServiceID(SERVICEID_INDICATORLAYOUT);
	}

	IndicatorLayoutService::~IndicatorLayoutService()
	{
		m_lock.Lock();
		map<int, SocketArray*>::iterator sIter = m_sockets.begin();
		for(; sIter != m_sockets.end(); ++sIter)
		{
			delete sIter->second;
		}
		m_sockets.clear();
		m_lock.UnLock();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int IndicatorLayoutService::AddLayouts(CMessage *message)
	{
		vector<IndicatorLayout> layouts;
        GetLayouts(&layouts, message->m_body, message->m_bodyLength);
		vector<IndicatorLayout>::iterator sIter = layouts.begin();
		vector<IndicatorLayout> addLayouts;
		for(; sIter != layouts.end(); ++sIter)
		{
			IndicatorLayout receive = *sIter;
			CStrA::stringTowstring(receive.m_layoutID, CStrA::GetGuid());
			int strLen = sizeof(int) * 4 + CStrA::GetStringCapacity(receive.m_layoutID) + CStrA::GetStringCapacity(receive.m_name) + CStrA::GetStringCapacity(receive.m_text)  + 300;
			wchar_t *sql = new wchar_t[strLen];
			memset(sql, 0, strLen * sizeof(wchar_t));
			_stprintf_s(sql, strLen - 1, L"INSERT INTO INDICATORLAYOUT(LAYOUTID, USERID, NAME, TEXT, TYPE, ORDERNUM, CREATETIME, MODIFYTIME) values ('%s', %d, '%s', '%s', %d, %d, '1970-1-1', '1970-1-1')",
				CStrA::GetDBString(receive.m_layoutID).c_str(), receive.m_userID, CStrA::GetDBString(receive.m_name).c_str(), CStrA::GetDBString(receive.m_text).c_str(), receive.m_type, receive.m_orderNum);
			sqlite3_connection conn(m_dataBasePath.c_str());
			conn.executenonquery(sql);
			conn.close();
			delete[] sql;
			sql = 0;
			addLayouts.push_back(receive);
		}
		int ret = Send(message, &addLayouts, true);
		layouts.clear();
		addLayouts.clear();
		return ret;
	}

	void IndicatorLayoutService::CreateTable()
	{
		const wchar_t *strPath = m_dataBasePath.c_str();
		sqlite3_connection conn(strPath);
		conn.executenonquery(m_createTableSQL.c_str());
		conn.close();
	}

	int IndicatorLayoutService::DeleteLayouts(CMessage *message)
	{
		vector<IndicatorLayout> layouts;
        GetLayouts(&layouts, message->m_body, message->m_bodyLength);
		vector<IndicatorLayout>::iterator sIter = layouts.begin();
		vector<IndicatorLayout> deleteLayouts;
		for(; sIter != layouts.end(); ++sIter)
		{
			IndicatorLayout receive = *sIter;
			int strLen = sizeof(int)  + CStrA::GetStringCapacity(receive.m_layoutID) + 100;
			wchar_t *sql = new wchar_t[strLen];
			memset(sql, 0, strLen * sizeof(wchar_t));
			_stprintf_s(sql, strLen - 1, L"DELETE FROM INDICATORLAYOUT WHERE USERID = %d AND LAYOUTID = '%s'",
				receive.m_userID, CStrA::GetDBString(receive.m_layoutID).c_str());
			sqlite3_connection conn(m_dataBasePath.c_str());
			conn.executenonquery(sql);
			conn.close();
			delete[] sql;
			sql = 0;
			deleteLayouts.push_back(receive);
		}
		int ret = Send(message, &deleteLayouts, true);
		layouts.clear();
		deleteLayouts.clear();
		return ret;
	}

	int IndicatorLayoutService::GetLayouts(vector<IndicatorLayout> *layouts, const char *body, int bodyLength)
	{
		Binary binary;
		binary.Write(body, bodyLength);
		int layoutsSize = binary.ReadInt();
		for(int i = 0; i < layoutsSize; i++)
		{
			IndicatorLayout layout;
			binary.ReadString(layout.m_layoutID);
			layout.m_userID = binary.ReadInt();
			binary.ReadString(layout.m_name);
			binary.ReadString(layout.m_text);
			layout.m_type = binary.ReadInt();
			layout.m_orderNum = binary.ReadInt();
			layouts->push_back(layout);
		}
		return 1;
	}

	int IndicatorLayoutService::GetLayouts(vector<IndicatorLayout> *layouts, int userID, String layoutID)
	{
		int strLen = 0;
		if(layoutID.length() > 0)
		{
			strLen = sizeof(int)  + CStrA::GetStringCapacity(layoutID) + 100;
		}
		else
		{
			strLen = sizeof(int) + 100;
		}
		wchar_t *sql = new wchar_t[strLen];
		memset(sql, 0, strLen * sizeof(wchar_t));
		if(layoutID.length() > 0)
		{
			_stprintf_s(sql, strLen - 1, L"SELECT * FROM INDICATORLAYOUT WHERE USERID = %d AND LAYOUTID = '%s'",
				userID, CStrA::GetDBString(layoutID).c_str());
		}
		else
		{
			if(userID > 0)
			{
				_stprintf_s(sql, strLen - 1, L"SELECT * FROM INDICATORLAYOUT WHERE USERID = %d ORDER BY ORDERNUM", userID);
			}
			else
			{
				_stprintf_s(sql, strLen - 1, L"%s", L"SELECT * FROM INDICATORLAYOUT ORDER BY ORDERNUM");
			}
		}
		sqlite3_connection conn(m_dataBasePath.c_str());
		sqlite3_command cmd(conn, sql);
		sqlite3_reader reader = cmd.executereader();
		while(reader.read())
		{
			IndicatorLayout layout;
            layout.m_layoutID = reader.getstring16(0);
            layout.m_userID = reader.getint(1);
			layout.m_name = reader.getstring16(2);
            layout.m_text = reader.getstring16(3);
            layout.m_type = reader.getint(4);
            layout.m_type = reader.getint(5);
			layouts->push_back(layout);
		}
		reader.close();
		delete[] sql;
		sql = 0;
		return 1;
	}

	int IndicatorLayoutService::GetLayouts(CMessage *message)
	{
	    IndicatorLayout receive;
		vector<IndicatorLayout> layouts;
        GetLayouts(&layouts, message->m_body, message->m_bodyLength);
		receive = layouts[0];
		layouts.clear();
		GetLayouts(&layouts, receive.m_userID, receive.m_layoutID.c_str());
		int ret = Send(message, &layouts, false);
		layouts.clear();
		return ret;
	}

	void IndicatorLayoutService::OnClientClose(int socketID, int localSID)
	{
		BaseService::OnClientClose(socketID, localSID);
		m_lock.Lock();
		map<int, SocketArray*>::iterator sIter = m_sockets.begin();
		for(; sIter != m_sockets.end(); ++sIter)
		{
			SocketArray *socketArray = sIter->second;
		    vector<int> sockets;
            socketArray->GetSocketList(&sockets);
            int socketsSize = (int)sockets.size();
            for (int i = 0; i < socketsSize; i++)
            {
                if (sockets[i] == socketID)
                {
                    socketArray->RemoveSocket(socketID);
                }
            }
            sockets.clear();
		}
		m_lock.UnLock();
	}

	void IndicatorLayoutService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
	    switch (message->m_functionID)
        {
            case FUNCTIONID_INDICATORLAYOUT_ADDLAYOUTS:
                AddLayouts(message);
                break;
            case FUNCTIONID_INDICATORLAYOUT_DELETELAYOUTS:
                DeleteLayouts(message);
                break;
            case FUNCTIONID_INDICATORLAYOUT_GETLAYOUTS:
                GetLayouts(message);
                break;
			case FUNCTIONID_INDICATORLAYOUT_UPDATELAYOUTS:
				UpdateLayouts(message);
				break;
			case FUNCTIONID_INDICATORLAYOUT_UPDATEORDERNUM:
				UpdateOrderNum(message);
				break;
        }
	}

	int IndicatorLayoutService::Send(CMessage *message, vector<IndicatorLayout> *layouts, bool broadCast)
	{
		int size = (int)layouts->size();
		int strSize = sizeof(int);
		for(int i = 0; i < size; i++)
		{
			IndicatorLayout layout = (*layouts)[i];
			strSize += sizeof(int) * 3 + CStrA::GetStringCapacity(layout.m_layoutID) + CStrA::GetStringCapacity(layout.m_name) + CStrA::GetStringCapacity(layout.m_text);
		}
		strSize += 100;
		Binary binary;
		binary.Initialize(strSize, false);
		binary.WriteInt(size);
		for(int i = 0; i < size; i++)
		{
			IndicatorLayout layout = (*layouts)[i];
			binary.WriteString(layout.m_layoutID);
            binary.WriteInt(layout.m_userID);
			binary.WriteString(layout.m_name);
            binary.WriteString(layout.m_text);
            binary.WriteInt(layout.m_type);
			binary.WriteInt(layout.m_orderNum);
		}
		char *ps = (char*)binary.GetBytes();
		message->m_bodyLength = strSize;
		message->m_body = ps;
		int userID = 0;
		if(size > 0)
		{
			userID = (*layouts)[0].m_userID;
		}
		else
		{
			broadCast = false;
		}
		m_lock.Lock();
		SocketArray *socketArray = 0;
		map<int, SocketArray*>::iterator sIter = m_sockets.find(userID);
		if(sIter != m_sockets.end())
		{
			socketArray = sIter->second;
		}
		else
		{
			socketArray = new SocketArray;
			m_sockets[userID] = socketArray;
		}
		if(message->m_sessionID > 0)
		{
			socketArray->AddSocket(message->m_socketID);
		}
		if(broadCast)
		{
			vector<int> sockets;
			socketArray->GetSocketList(&sockets);
			int socketsSize = (int)sockets.size();
			for (int i = 0; i < socketsSize; i++)
			{
				message->m_socketID = sockets[i];
				int ret = BaseService::Send(message);
				if (ret == -1)
				{
					socketArray->RemoveSocket(sockets[i]);
				}
			}
		}
		else
		{
			BaseService::Send(message);
		}
		m_lock.UnLock();
		return 1;
	}

	int IndicatorLayoutService::UpdateLayouts(CMessage *message)
	{
		vector<IndicatorLayout> layouts;
        GetLayouts(&layouts, message->m_body, message->m_bodyLength);
		vector<IndicatorLayout>::iterator sIter = layouts.begin();
		vector<IndicatorLayout> updateLayouts;
		for(; sIter != layouts.end(); ++sIter)
		{
			IndicatorLayout receive = *sIter;
			UpdateLayout(&receive);
			updateLayouts.push_back(receive);
		}
		int ret = Send(message, &updateLayouts, true);
		layouts.clear();
		updateLayouts.clear();
		return ret;
	}

	void IndicatorLayoutService::UpdateLayout(IndicatorLayout *layout)
	{
		int strLen = sizeof(int) * 3 + CStrA::GetStringCapacity(layout->m_layoutID) + CStrA::GetStringCapacity(layout->m_name) + CStrA::GetStringCapacity(layout->m_text);
		wchar_t *sql = new wchar_t[strLen];
		memset(sql, 0, strLen * sizeof(wchar_t));
		_stprintf_s(sql, strLen - 1, L"UPDATE INDICATORLAYOUT SET NAME = '%s', TEXT = '%s', TYPE = %d, ORDERNUM = %d WHERE USERID = %d AND LAYOUTID = '%s'",
			CStrA::GetDBString(layout->m_name).c_str(), CStrA::GetDBString(layout->m_text).c_str(), layout->m_type, layout->m_orderNum, layout->m_userID, CStrA::GetDBString(layout->m_layoutID).c_str());
		sqlite3_connection conn(m_dataBasePath.c_str());
		conn.executenonquery(sql);
		conn.close();
		delete[] sql;
		sql = 0;
	}

	int IndicatorLayoutService::UpdateOrderNum(CMessage *message)
	{
		vector<IndicatorLayout> layouts;
        GetLayouts(&layouts, message->m_body, message->m_bodyLength);
		vector<IndicatorLayout>::iterator sIter = layouts.begin();
		vector<IndicatorLayout> updateLayouts;
		for(; sIter != layouts.end(); ++sIter)
		{
			IndicatorLayout receive = *sIter;
			vector<String> ids = CStrA::Split(receive.m_layoutID, L",");
			int idsSize = (int)ids.size();
            for (int j = 0; j < idsSize; j++)
            {
				int strLen = sizeof(int) * 3 + CStrA::GetStringCapacity(ids[j]) + 200;
				wchar_t *sql = new wchar_t[strLen];
				memset(sql, 0, strLen * sizeof(wchar_t));
				_stprintf_s(sql, strLen - 1, L"UPDATE INDICATORLAYOUT SET ORDERNUM = %d WHERE USERID = %d AND LAYOUTID = '%s'",
					j, receive.m_userID, CStrA::GetDBString(ids[j]).c_str());
				sqlite3_connection conn(m_dataBasePath.c_str());
				conn.executenonquery(sql);
				conn.close();
				delete[] sql;
				sql = 0;
			}
			ids.clear();
			updateLayouts.push_back(receive);
		}
		int ret = Send(message, &updateLayouts, true);
		layouts.clear();
		updateLayouts.clear();
		return ret;
	}
}