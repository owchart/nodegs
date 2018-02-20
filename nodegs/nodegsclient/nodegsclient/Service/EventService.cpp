#include "..\\stdafx.h"
#include "..\\CStr.h"
#include "EventService.h"
#include "Binary.h"

namespace OwLibCT
{
	EventService::EventService()
	{
		m_native = new NativeBase();
		m_socketID = 0;
		SetServiceID(SERVICEID_EVENT);
	}

	EventService::~EventService()
	{
		if(m_native)
		{
			delete m_native;
		}
	}

	NativeBase* EventService::GetNative()
	{
		return m_native;
	}

	int EventService::GetSocketID()
	{
		return m_socketID;
	}

	void EventService::SetSocketID(int socketID)
	{
		m_socketID = socketID;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int EventService::SendEvent(int requestID, GridA *grid)
	{
        int ret = Send(FUNCTIONID_EXCUTEID, requestID, grid);
        return ret > 0 ? 1 : 0;
	}

	GridA* EventService::GetGrid(CMessage *message)
	{
		Binary br;
		int size = message->m_bodyLength;
		br.Write(message->m_body, size);
		GridA *grid = new GridA();
		grid->SetNative(m_native);
		String name = L"";
		br.ReadString(name);
		grid->SetName(name);

		int columnsSize = br.ReadInt();
		for (int i = 0; i < columnsSize; i++)
		{
			GridColumn *column = new GridColumn();
			String name = L"";
			String columnType = L"";
			br.ReadString(name);
            br.ReadString(columnType);
			column->SetName(name);
			column->SetColumnType(columnType);
			grid->AddColumn(column);
		}
		grid->Update();
		vector<GridColumn*> columns = grid->GetColumns();
		int rowsCount = br.ReadInt();
		for (int i = 0; i < rowsCount; i++)
		{
			GridRow *row = new GridRow();
			grid->AddRow(row);
			for (int j = 0; j < columnsSize; j++)
			{
				GridColumn *column = columns[j];
				String columnType = column->GetColumnType();
				GridCell *cell;
				String value = L"";
				if (columnType == L"bool")
				{
					cell = new GridBoolCell();
					row->AddCell(j, cell);
					char charVal = br.ReadChar();
					if(charVal == '0')
					{
						cell->SetBool(false);
					}
					else
					{
						cell->SetBool(true);
					}
				}
				else if (columnType == L"double")
				{
					cell = new GridDoubleCell();
					row->AddCell(j, cell);
					cell->SetDouble(br.ReadDouble());
				}
				else if (columnType == L"float")
				{
					cell = new GridFloatCell();
					row->AddCell(j, cell);
					cell->SetFloat(br.ReadFloat());
				}
				else if (columnType == L"int")
				{
					cell = new GridIntCell();
					row->AddCell(j, cell);
					cell->SetInt(br.ReadInt());
				}
				else if (columnType == L"long")
				{
					cell = new GridLongCell();
					row->AddCell(j, cell);
					cell->SetLong((long)br.ReadDouble());
				}
				else if (columnType == L"string")
				{
					cell = new GridStringCell();
					row->AddCell(j, cell);
					br.ReadString(value);
					cell->SetString(value);
				}
				else
				{
					cell = new GridStringCell();
					row->AddCell(j, cell);
					br.ReadString(value);
					cell->SetString(value);
				}
			}
		}
		return grid;
	}

	
	void EventService::OnReceive(CMessage *message)
	{
		BaseService::OnReceive(message);
        SendToListener(message);
	}

	int EventService:: Send(int functionID, int requestID, GridA *grid)
	{
		int len = 0;
		Binary br;
		br.WriteString(grid->GetName());
		len += CStrA::GetWStringCapacity(grid->GetName());
        vector<GridColumn*> columns = grid->GetColumns();
		int columnsSize = (int)columns.size();
        br.WriteInt(columnsSize);
		len += 4;
        for (int i = 0; i < columnsSize; i++)
        {
            GridColumn *column = columns[i];
            br.WriteString(column->GetName());
			len += CStrA::GetWStringCapacity(column->GetName());
            br.WriteString(column->GetColumnType());
			len += CStrA::GetWStringCapacity(column->GetColumnType());
        }
        vector<GridRow*> rows = grid->GetRows();
		int rowsCount = (int)rows.size();
        br.WriteInt(rowsCount);
		len += 4;
        for (int i = 0; i < rowsCount; i++)
        {
            GridRow *row = rows[i];
            for (int j = 0; j < columnsSize; j++)
            {
                GridColumn *column = columns[j];
				String columnType = column->GetColumnType();
                GridCell *cell = row->GetCell(j);
                if (columnType == L"bool")
                {
					bool bolValue = cell->GetBool();
					if(!bolValue)
					{
						br.WriteChar(char(0));
					}else
					{
						br.WriteChar(char(1));
					}
					len +=1;
                }
                else if (columnType == L"double")
                {
                    br.WriteDouble(cell->GetDouble());
					len += 8;
                }
                else if (columnType == L"float")
                {
                    br.WriteFloat(cell->GetFloat());
					len += 4;
                }
                else if (columnType == L"int")
                {
                    br.WriteInt(cell->GetInt());
					len += 4;
                }
                else if (columnType == L"long")
                {
                    br.WriteDouble((double)cell->GetLong());
					len += 8;
                }
                else if (columnType == L"string")
                {
                    br.WriteString(cell->GetString());
					len += CStrA::GetWStringCapacity(cell->GetString());
                }
            }
        }
		char *bytes = (char*)br.GetBytes();

		CMessage message(GetGroupID(), GetServiceID(), functionID, GetSessionID(), requestID, m_socketID, 0, GetCompressType(), len, bytes);
		return BaseService::Send(&message);
	}
}