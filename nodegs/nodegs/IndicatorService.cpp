#include "stdafx.h"
#include "IndicatorService.h"
#include "CStr.h"
#include "Binary.h"
#include "CFile.h"
#include "sqlite3x.hpp"
using namespace sqlite3x;

namespace OwLibSV
{
	Indicator::Indicator()
	{
		m_category = L"";
		m_coordinate = L"";
		m_description = L"";
		m_digit = 0;
		m_indicatorID = L"";
		m_name = L"";
		m_orderNum = 0;
		m_paintType = 0;
		m_parameters = L"";
		m_password = L"";
		m_specialCoordinate = L"";
		m_text = L"";
		m_type = 0;
		m_usePassword = 0;
		m_userID = 0;
		m_version = 0;
	}

	IndicatorService::IndicatorService()
	{
		m_createTableSQL = "CREATE TABLE INDICATOR(INDICATORID PRIMARY KEY, USERID INTEGER, CATEGORY, COORDINATE, DESCRIPTION, DIGIT INTEGER, NAME, ORDERNUM INTEGER, PAINTTYPE INTEGER, PARAMETERS, PASSWORD, SPECIALCOORDINATE, TEXT, TYPE INTEGER, USEPASSWORD INTEGER, VERSION INTEGER, CREATETIME DATE, MODIFYTIME DATE)";
		string dataBasePath = DataCenter::GetAppDir() + "\\indicators.db";
		CStrA::stringTowstring(m_dataBasePath, dataBasePath);
		if(!CFileA::IsFileExist(dataBasePath.c_str()))
		{
			CreateTable();
		}
		SetServiceID(SERVICEID_INDICATOR);
	}

	IndicatorService::~IndicatorService()
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

	int IndicatorService::AddIndicators(CMessage *message)
	{
		vector<Indicator> indicators;
        GetIndicators(&indicators, message->m_body, message->m_bodyLength);
		vector<Indicator>::iterator sIter = indicators.begin();
		vector<Indicator> addIndicators;
		for(; sIter != indicators.end(); ++sIter)
		{
			Indicator receive = *sIter;
			CStrA::stringTowstring(receive.m_indicatorID, CStrA::GetGuid());
			int strLen = sizeof(int) * 7 + CStrA::GetStringCapacity(receive.m_indicatorID) + CStrA::GetStringCapacity(receive.m_category) + CStrA::GetStringCapacity(receive.m_coordinate)
				+ CStrA::GetStringCapacity(receive.m_description) + CStrA::GetStringCapacity(receive.m_name) 
				+ CStrA::GetStringCapacity(receive.m_parameters) + CStrA::GetStringCapacity(receive.m_password) + CStrA::GetStringCapacity(receive.m_specialCoordinate)
				+ CStrA::GetStringCapacity(receive.m_text) + 300;
			wchar_t *sql = new wchar_t[strLen];
			memset(sql, 0, strLen * sizeof(wchar_t));
			_stprintf_s(sql, strLen - 1, L"INSERT INTO INDICATOR(INDICATORID, USERID, CATEGORY, COORDINATE, DESCRIPTION, DIGIT, NAME, ORDERNUM, PAINTTYPE, PARAMETERS, PASSWORD, SPECIALCOORDINATE, TEXT, TYPE, USEPASSWORD, VERSION, CREATETIME, MODIFYTIME) values ('%s', %d, '%s', '%s', '%s', %d, '%s', %d, %d, '%s', '%s', '%s', '%s', %d, %d, %d, '1970-1-1', '1970-1-1')",
				CStrA::GetDBString(receive.m_indicatorID).c_str(), receive.m_userID, CStrA::GetDBString(receive.m_category).c_str(), CStrA::GetDBString(receive.m_coordinate).c_str(), CStrA::GetDBString(receive.m_description).c_str(), receive.m_digit, CStrA::GetDBString(receive.m_name).c_str(), receive.m_orderNum, receive.m_paintType, CStrA::GetDBString(receive.m_parameters).c_str(), CStrA::GetDBString(receive.m_password).c_str(), CStrA::GetDBString(receive.m_specialCoordinate).c_str(), CStrA::GetDBString(receive.m_text).c_str(), receive.m_type, receive.m_usePassword, receive.m_version);
			sqlite3_connection conn(m_dataBasePath.c_str());
			conn.executenonquery(sql);
			conn.close();
			delete[] sql;
			sql = 0;
			addIndicators.push_back(receive);
		}
		int ret = Send(message, &addIndicators, true);
		indicators.clear();
		addIndicators.clear();
		return ret;
	}

	void IndicatorService::CreateTable()
	{
		const wchar_t *strPath = m_dataBasePath.c_str();
		sqlite3_connection conn(strPath);
		conn.executenonquery(m_createTableSQL.c_str());
		conn.close();
	}

	int IndicatorService::DeleteIndicators(CMessage *message)
	{
		vector<Indicator> indicators;
        GetIndicators(&indicators, message->m_body, message->m_bodyLength);
		vector<Indicator>::iterator sIter = indicators.begin();
		vector<Indicator> deleteIndicators;
		for(; sIter != indicators.end(); ++sIter)
		{
			Indicator receive = *sIter;
			int strLen = sizeof(int)  + CStrA::GetStringCapacity(receive.m_indicatorID) + 100;
			wchar_t *sql = new wchar_t[strLen];
			memset(sql, 0, strLen * sizeof(wchar_t));
			_stprintf_s(sql, strLen - 1, L"DELETE FROM INDICATOR WHERE USERID = %d AND INDICATORID = '%s'",
				receive.m_userID, CStrA::GetDBString(receive.m_indicatorID).c_str());
			sqlite3_connection conn(m_dataBasePath.c_str());
			conn.executenonquery(sql);
			conn.close();
			delete[] sql;
			sql = 0;
			deleteIndicators.push_back(receive);
		}
		int ret = Send(message, &deleteIndicators, true);
		indicators.clear();
		deleteIndicators.clear();
		return ret;
	}

	int IndicatorService::GetIndicators(vector<Indicator> *indicators, const char *body, int bodyLength)
	{
		Binary binary;
		binary.Write(body, bodyLength);
		int indicatorsSize = binary.ReadInt();
		for(int i = 0; i < indicatorsSize; i++)
		{
			Indicator indicator;
			binary.ReadString(indicator.m_indicatorID);
			indicator.m_userID = binary.ReadInt();
			binary.ReadString(indicator.m_category);
			binary.ReadString(indicator.m_coordinate);
			binary.ReadString(indicator.m_description);
			indicator.m_digit = binary.ReadInt();
			binary.ReadString(indicator.m_name);
			indicator.m_orderNum = binary.ReadInt();
			indicator.m_paintType = binary.ReadInt();
			binary.ReadString(indicator.m_parameters);
			binary.ReadString(indicator.m_password);
			binary.ReadString(indicator.m_specialCoordinate);
			binary.ReadString(indicator.m_text);
			indicator.m_type = binary.ReadInt();
			indicator.m_usePassword = binary.ReadInt();
			indicator.m_version = binary.ReadInt();
			indicators->push_back(indicator);
		}
		return 1;
	}

	int IndicatorService::GetIndicators(vector<Indicator> *indicators, int userID, String indicatorID)
	{
		int strLen = 0;
		if(indicatorID.length() > 0)
		{
			strLen = sizeof(int)  + CStrA::GetStringCapacity(indicatorID) + 100;
		}
		else
		{
			strLen = sizeof(int) + 100;
		}
		wchar_t *sql = new wchar_t[strLen];
		memset(sql, 0, strLen * sizeof(wchar_t));
		if(indicatorID.length() > 0)
		{
			_stprintf_s(sql, strLen - 1, L"SELECT * FROM INDICATOR WHERE TYPE = 0 OR (USERID = %d AND INDICATORID = '%s')",
				userID, CStrA::GetDBString(indicatorID).c_str());
		}
		else
		{
			if(userID > 0)
			{
				_stprintf_s(sql, strLen - 1, L"SELECT * FROM INDICATOR WHERE TYPE = 0 OR USERID = %d ORDER BY ORDERNUM", userID);
			}
			else
			{
				_stprintf_s(sql, strLen - 1, L"%s", L"SELECT * FROM INDICATOR ORDER BY ORDERNUM");
			}
		}
		sqlite3_connection conn(m_dataBasePath.c_str());
		sqlite3_command cmd(conn, sql);
		sqlite3_reader reader = cmd.executereader();
		while(reader.read())
		{
			Indicator indicator;
            indicator.m_indicatorID = reader.getstring16(0);
            indicator.m_userID = reader.getint(1);
			indicator.m_category = reader.getstring16(2);
            indicator.m_coordinate = reader.getstring16(3);
            indicator.m_description = reader.getstring16(4);
            indicator.m_digit = reader.getint(5);
            indicator.m_name = reader.getstring16(6);
            indicator.m_orderNum = reader.getint(7);
            indicator.m_paintType = reader.getint(8);
            indicator.m_parameters = reader.getstring16(9);
            indicator.m_password = reader.getstring16(10);
            indicator.m_specialCoordinate = reader.getstring16(11);
            indicator.m_text = reader.getstring16(12);
            indicator.m_type = reader.getint(13);
            indicator.m_usePassword = reader.getint(14);
            indicator.m_version = reader.getint(15);
			indicators->push_back(indicator);
		}
		reader.close();
		delete[] sql;
		sql = 0;
		return 1;
	}

	int IndicatorService::GetIndicators(CMessage *message)
	{
	    Indicator receive;
		vector<Indicator> indicators;
        GetIndicators(&indicators, message->m_body, message->m_bodyLength);
		receive = indicators[0];
		indicators.clear();
		GetIndicators(&indicators, receive.m_userID, receive.m_indicatorID.c_str());
		int ret = Send(message, &indicators, false);
		indicators.clear();
		return ret;
	}

	void IndicatorService::OnClientClose(int socketID, int localSID)
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

	void IndicatorService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
	    switch (message->m_functionID)
        {
            case FUNCTIONID_INDICATOR_ADDINDICATORS:
                AddIndicators(message);
                break;
            case FUNCTIONID_INDICATOR_DELETEINDICATORS:
                DeleteIndicators(message);
                break;
            case FUNCTIONID_INDICATOR_GETINDICATORS:
                GetIndicators(message);
                break;
			case FUNCTIONID_INDICATOR_UPDATEINDICATORS:
				UpdateIndicators(message);
				break;
			case FUNCTIONID_INDICATOR_UPDATEORDERNUM:
				UpdateOrderNum(message);
				break;
        }
	}

	int IndicatorService::Send(CMessage *message, vector<Indicator> *indicators, bool broadCast)
	{
		int size = (int)indicators->size();
		int strSize = sizeof(int);
		for(int i = 0; i < size; i++)
		{
			Indicator indicator = indicators->at(i);
			strSize += sizeof(int) * 7 + CStrA::GetStringCapacity(indicator.m_indicatorID) + CStrA::GetStringCapacity(indicator.m_category) + CStrA::GetStringCapacity(indicator.m_coordinate)
			+ CStrA::GetStringCapacity(indicator.m_description) + CStrA::GetStringCapacity(indicator.m_name) 
			+ CStrA::GetStringCapacity(indicator.m_parameters) + CStrA::GetStringCapacity(indicator.m_password) + CStrA::GetStringCapacity(indicator.m_specialCoordinate)
			+ CStrA::GetStringCapacity(indicator.m_text);
		}
		strSize += 100;
		Binary binary;
		binary.Initialize(strSize, false);
		binary.WriteInt(size);
		for(int i = 0; i < size; i++)
		{
			Indicator indicator = indicators->at(i);
            binary.WriteString(indicator.m_indicatorID);
            binary.WriteInt(indicator.m_userID);
			binary.WriteString(indicator.m_category);
            binary.WriteString(indicator.m_coordinate);
            binary.WriteString(indicator.m_description);
            binary.WriteInt(indicator.m_digit);
            binary.WriteString(indicator.m_name);
            binary.WriteInt(indicator.m_orderNum);
            binary.WriteInt(indicator.m_paintType);
            binary.WriteString(indicator.m_parameters);
            binary.WriteString(indicator.m_password);
            binary.WriteString(indicator.m_specialCoordinate);
            binary.WriteString(indicator.m_text);
            binary.WriteInt(indicator.m_type);
            binary.WriteInt(indicator.m_usePassword);
            binary.WriteInt(indicator.m_version);
		}
		char *ps = (char*)binary.GetBytes();
		message->m_bodyLength = strSize;
		message->m_body = ps;
		int userID = 0; 
		if(size > 0)
		{
			userID = indicators->at(0).m_userID;
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

	int IndicatorService::UpdateIndicators(CMessage *message)
	{
		vector<Indicator> indicators;
        GetIndicators(&indicators, message->m_body, message->m_bodyLength);
		vector<Indicator>::iterator sIter = indicators.begin();
		vector<Indicator> updateIndicators;
		for(; sIter != indicators.end(); ++sIter)
		{
			Indicator receive = *sIter;
			UpdateIndicator(&receive);
			updateIndicators.push_back(receive);
		}
		int ret = Send(message, &updateIndicators, true);
		indicators.clear();
		updateIndicators.clear();
		return ret;
	}

	void IndicatorService::UpdateIndicator(Indicator *indicator)
	{
		int strLen = sizeof(int) * 7 + CStrA::GetStringCapacity(indicator->m_indicatorID) + CStrA::GetStringCapacity(indicator->m_category) + CStrA::GetStringCapacity(indicator->m_coordinate)
			+ CStrA::GetStringCapacity(indicator->m_description) + CStrA::GetStringCapacity(indicator->m_name) 
			+ CStrA::GetStringCapacity(indicator->m_parameters) + CStrA::GetStringCapacity(indicator->m_password) + CStrA::GetStringCapacity(indicator->m_specialCoordinate)
			+ CStrA::GetStringCapacity(indicator->m_text) + 300;
		wchar_t *sql = new wchar_t[strLen];
		memset(sql, 0, strLen * sizeof(wchar_t));
		_stprintf_s(sql, strLen - 1, L"UPDATE INDICATOR SET CATEGORY = '%s', COORDINATE = '%s', DESCRIPTION = '%s', DIGIT = %d, NAME = '%s', ORDERNUM = %d, PAINTTYPE = %d, PARAMETERS = '%s', PASSWORD = '%s', SPECIALCOORDINATE = '%s', TEXT = '%s', TYPE = %d, USEPASSWORD = %d, VERSION = %d WHERE USERID = %d AND INDICATORID = '%s'",
			CStrA::GetDBString(indicator->m_category).c_str(), CStrA::GetDBString(indicator->m_coordinate).c_str(), 
			CStrA::GetDBString(indicator->m_description).c_str(), indicator->m_digit, CStrA::GetDBString(indicator->m_name).c_str(), 
			indicator->m_orderNum, indicator->m_paintType, CStrA::GetDBString(indicator->m_parameters).c_str(), 
			CStrA::GetDBString(indicator->m_password).c_str(), CStrA::GetDBString(indicator->m_specialCoordinate).c_str(),
			CStrA::GetDBString(indicator->m_text).c_str(), indicator->m_type, indicator->m_usePassword, indicator->m_version, 
			indicator->m_userID, CStrA::GetDBString(indicator->m_indicatorID).c_str());
		sqlite3_connection conn(m_dataBasePath.c_str());
		conn.executenonquery(sql);
		conn.close();
		delete[] sql;
		sql = 0;
	}

	int IndicatorService::UpdateOrderNum(CMessage *message)
	{
		vector<Indicator> indicators;
        GetIndicators(&indicators, message->m_body, message->m_bodyLength);
		vector<Indicator>::iterator sIter = indicators.begin();
		vector<Indicator> updateIndicators;
		for(; sIter != indicators.end(); ++sIter)
		{
			Indicator receive = *sIter;
			vector<String> ids = CStrA::Split(receive.m_indicatorID, L",");
			int idsSize = (int)ids.size();
            for (int j = 0; j < idsSize; j++)
            {
				int strLen = sizeof(int) * 2 + CStrA::GetStringCapacity(ids[j]) + 200;
				wchar_t *sql = new wchar_t[strLen];
				memset(sql, 0, strLen * sizeof(wchar_t));
				_stprintf_s(sql, strLen - 1, L"UPDATE INDICATOR SET ORDERNUM = %d WHERE USERID = %d AND INDICATORID = '%s'",
					j, receive.m_userID, CStrA::GetDBString(ids[j]).c_str());
				sqlite3_connection conn(m_dataBasePath.c_str());
				conn.executenonquery(sql);
				conn.close();
				delete[] sql;
				sql = 0;
			}
			ids.clear();
			updateIndicators.push_back(receive);
		}
		int ret = Send(message, &updateIndicators, true);
		indicators.clear();
		updateIndicators.clear();
		return ret;
	}
}