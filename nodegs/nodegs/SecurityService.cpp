#include "stdafx.h"
#include "SecurityService.h"
#include "CStr.h"
#include "Binary.h"
#include "CFile.h"
#include "sqlite3x.hpp"
#include "DataCenter.h"
using namespace sqlite3x;

namespace OwLibSV
{
	Security::Security()
	{
		m_code = L"";
		m_name = L"";
		m_pingyin = L"";
		m_sCode = "";
		m_status = 0;
		m_type = 0;
	}

	SecurityService::SecurityService()
	{
		m_createTableSQL = "CREATE TABLE SECURITY(ID INTEGER PRIMARY KEY, CODE, NAME, PINGYIN, TYPE INTEGER, STATUS INTEGER, CREATETIME DATE, MODIFYTIME DATE)";
		string dataBasePath = DataCenter::GetAppDir() + "\\securities.db";
		CStrA::stringTowstring(m_dataBasePath, dataBasePath);
		if(!CFileA::IsFileExist(dataBasePath.c_str()))
		{
			CreateTable();
		}
		string path = DataCenter::GetAppDir() + "\\securities.db";
		SetServiceID(SERVICEID_SECURITY);
	}

	SecurityService::~SecurityService()
	{
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void SecurityService::CreateTable()
	{
		const wchar_t *strPath = m_dataBasePath.c_str();
		sqlite3_connection conn(strPath);
		conn.executenonquery(m_createTableSQL.c_str());
		conn.close();
	}

	int SecurityService::GetSecurities(vector<Security> *securities, String filter)
	{
		int strLen = 100;
		wchar_t *sql = new wchar_t[strLen];
		memset(sql, 0, strLen * sizeof(wchar_t));
		_stprintf_s(sql, strLen - 1, L"%s", L"SELECT * FROM SECURITY");
		String wSql = sql;
		if(filter.length() > 0)
		{
			wSql += L" WHERE " + filter;
		}
		sqlite3_connection conn(m_dataBasePath.c_str());
		sqlite3_command cmd(conn, wSql.c_str());
		sqlite3_reader reader = cmd.executereader();
		while(reader.read())
		{
			Security security;
			security.m_code = reader.getstring16(1);
			security.m_name = reader.getstring16(2);
			security.m_pingyin = reader.getstring16(3);
			security.m_type = reader.getint(4);
			security.m_status = reader.getint(5);
			securities->push_back(security);
		}
		reader.close();
		delete[] sql;
		sql = 0;
		return 1;
	}

	int SecurityService::GetSecurities(CMessage *message)
	{
		Binary binary;
		binary.Write(message->m_body, message->m_bodyLength);
		String blockID = L"";
		binary.ReadString(blockID);
        String filter = L"";
		if (blockID == L"lord")
        {
            filter = L"TYPE = 1 or TYPE = 2 or TYPE = 3 or TYPE = 4 or TYPE = 13 or TYPE = 17";
        }
		else if (blockID == L"101001")
        {
            filter = L"TYPE=1 OR TYPE=2";
        }
        else if (blockID == L"101002")
        {
            filter = L"TYPE=1";
        }
        else if (blockID == L"101003")
        {
            filter = L"TYPE=2";
        }
        else if (blockID == L"101004")
        {
            filter = L"TYPE=2 AND CODE LIKE '3%'";
        }
        else if (blockID == L"101005")
        {
            filter = L"TYPE=3 OR TYPE=4";
        }
        else if (blockID == L"101006")
        {
            filter = L"TYPE=3";
        }
        else if (blockID == L"101007")
        {
            filter = L"TYPE=4";
        }
        else if (blockID == L"101008")
        {
            filter = L"TYPE=1 OR TYPE=2 OR TYPE=3 OR TYPE=4";
        }
        else if (blockID == L"101009")
        {
            filter = L"TYPE=1 OR TYPE=3";
        }
        else if (blockID == L"101010")
        {
			filter = L"TYPE=2 OR TYPE=4";
        }
        else if (blockID == L"101011")
        {
            filter = L"NAME LIKE 'ST%'";
        }
        else if (blockID == L"101012")
        {
            filter = L"NAME LIKE '*ST%'";
        }
		else if (blockID == L"111001001")
        {
            filter = L"CODE LIKE '%.CS'";
        }
        else if (blockID == L"111001002")
        {
            filter = L"CODE LIKE '%.CZ'";
        }
        else if (blockID == L"111001003")
        {
            filter = L"CODE LIKE '%.CD'";
        }
        else if (blockID == L"111001004")
        {
            filter = L"CODE LIKE '%.FS'";
        }
        else if (blockID == L"501001")
        {
            filter = L"TYPE=13";
        }
		else if (blockID == L"905008004")
        {
            filter = L"CODE LIKE '%.SH' AND TYPE IN (17,28,30)";
        }
        else if (blockID == L"905008038")
        {
            filter = L"CODE LIKE '%.SZ' AND TYPE IN (17,28,30)";
        }
		vector<Security> securities;
		GetSecurities(&securities, filter);
		return Send(message, &securities);
	}

	void SecurityService::ImportSecurities(string path)
	{
		const char *strFilePath = path.c_str();
		if(CFileA::IsFileExist(strFilePath))
		{
			string result = "";
			CFileA::Read(strFilePath, &result);
			String str;
			CStrA::stringTowstring(str, result);
			vector<String> strs = CStrA::Split(str, L"\n");
			int size = (int)strs.size();
			if(size > 0)
			{
				sqlite3_connection conn(m_dataBasePath.c_str());
				for (int i = size - 1; i > 0; i--)
				{
					vector<String> strs2 = CStrA::Split(strs[i], L",");
					String code = strs[0];
					String name = strs[1];
					String pingyin = strs[2];
					int strLen = sizeof(int) * 2 + CStrA::GetStringCapacity(code) + CStrA::GetStringCapacity(name) + CStrA::GetStringCapacity(pingyin) + 100;
					wchar_t *sql = new wchar_t[strLen];
					memset(sql, 0, strLen * sizeof(wchar_t));
					_stprintf_s(sql, strLen - 1, L"INSERT INTO SECURITY(CODE, NAME, PINGYIN, TYPE, STATUS, CREATETIME, MODIFYTIME) VALUES ('%s', '%s', '%s', %s, %s, '1970-1-1', '1970-1-1')", 
						code.c_str(), name.c_str(), pingyin.c_str(), strs[3].c_str(), strs[4].c_str());
					conn.executenonquery(sql);
					delete[] sql;
					sql = 0;
				}
				conn.close();
			}
		}
	}



	void SecurityService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
	    switch (message->m_functionID)
        {
            case FUNCTIONID_SECURITY_GETSECURITIES:
			case FUNCTIONID_SECURITY_GETSECURITIESBYID:
                GetSecurities(message);
                break;
        }
	}

	int SecurityService::Send(CMessage *message, vector<Security> *securities)
	{
		int size = (int)securities->size();
		int strSize = sizeof(int);
		for(int i = 0; i < size; i++)
		{
			Security security = (*securities)[i];
			if(message->m_functionID == FUNCTIONID_SECURITY_GETSECURITIES)
			{
				strSize += sizeof(int) * 2 + CStrA::GetStringCapacity(security.m_code) + CStrA::GetStringCapacity(security.m_name)
					+ CStrA::GetStringCapacity(security.m_pingyin);
			}
			else
			{
				strSize += CStrA::GetStringCapacity(security.m_code);
			}
		}
		strSize += 100;
		Binary binary;
		binary.Initialize(strSize, false);
		binary.WriteInt(size);
		for(int i = 0; i < size; i++)
		{
			Security security = (*securities)[i];
			binary.WriteString(security.m_code);
			if(message->m_functionID == FUNCTIONID_SECURITY_GETSECURITIES)
			{
				binary.WriteString(security.m_name);
				binary.WriteString(security.m_pingyin);
				binary.WriteShort((short)security.m_type);
				binary.WriteChar((char)0);
			}
		}
		char *ps = (char*)binary.GetBytes();
		message->m_bodyLength = strSize;
		message->m_body = ps;
		return BaseService::Send(message);
	}
}