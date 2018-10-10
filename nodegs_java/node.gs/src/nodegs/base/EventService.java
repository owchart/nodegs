/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.base;

import owchart.owlib.Base.INativeBase;
import owchart.owlib.Base.NativeBase;
import owchart.owlib.Grid.GridA;
import owchart.owlib.Grid.GridBoolCell;
import owchart.owlib.Grid.GridCell;
import owchart.owlib.Grid.GridColumn;
import owchart.owlib.Grid.GridDoubleCell;
import owchart.owlib.Grid.GridFloatCell;
import owchart.owlib.Grid.GridIntCell;
import owchart.owlib.Grid.GridLongCell;
import owchart.owlib.Grid.GridRow;
import owchart.owlib.Grid.GridStringCell;
import java.util.ArrayList;
import java.util.List;
import nodegs.base.BaseService;
import nodegs.base.Binary;
import nodegs.base.CMessage;

/**
 *
 * @author GAIA
 */
public class EventService extends BaseService{
    /*
        创建服务
    */
    public EventService()
    {
        SetServiceID(SERVICEID_EVENT);
    }

    private INativeBase m_native = new NativeBase();
    
    public static final int SERVICEID_EVENT = 30;

    public static final int FUNCTIONID_EXCUTEID = 0;
    
    /**
     * 执行
     * @param message
     * @return 
     */
    public int Excute(CMessage message)
    {
        GridA grid = GetGrid(message.m_body, m_native);
        Send(message, grid);
        return 1;
    }

    /*
    获取表格的流
    */
    public static byte[] GetBytes(GridA grid)
    {
        byte[] bytes = null;
        try
        {
            Binary br = new Binary();
            br.WriteString(grid.GetName());
            ArrayList<GridColumn> columns = grid.GetColumns();
            int columnsSize = columns.size();
            br.WriteInt(columnsSize);
            for (int i = 0; i < columnsSize; i++)
            {
                GridColumn column = columns.get(i);
                br.WriteString(column.GetName());
                br.WriteString(column.GetColumnType());
            }
            List<GridRow> rows = grid.GetRows();
            int rowsCount = rows.size();
            br.WriteInt(rowsCount);
            for (int i = 0; i < rowsCount; i++)
            {
                GridRow row = rows.get(i);
                for (int j = 0; j < columnsSize; j++)
                {
                    GridColumn column = columns.get(j);
                    String columnType = column.GetColumnType().toLowerCase();
                    GridCell cell = null;
                    if (columnType.equals("bool"))
                    {
                        cell = new GridBoolCell();
                        row.AddCell(j, cell);
                        cell.SetBool(br.ReadBool());
                    }
                    else if (columnType.equals("double"))
                    {
                        cell = new GridDoubleCell();
                        row.AddCell(j, cell);
                        cell.SetDouble(br.ReadDouble());
                    }
                    else if (columnType.equals("float"))
                    {
                        cell = new GridFloatCell();
                        row.AddCell(j, cell);
                        cell.SetFloat(br.ReadFloat());
                    }
                    else if (columnType.equals("int"))
                    {
                        cell = new GridIntCell();
                        row.AddCell(j, cell);
                        cell.SetInt(br.ReadInt());
                    }
                    else if (columnType.equals("long"))
                    {
                        cell = new GridLongCell();
                        row.AddCell(j, cell);
                        cell.SetLong((long)br.ReadDouble());
                    }
                    else if (columnType.equals("string"))
                    {
                        cell = new GridStringCell();
                        row.AddCell(j, cell);
                        cell.SetString(br.ReadString());
                    }
                    else
                    {
                        cell = new GridStringCell();
                        row.AddCell(j, cell);
                        cell.SetString(br.ReadString());
                    }
                }
            }
            bytes = br.GetBytes();
            br.Close();
        }
        catch(Exception ex)
        {

        }
        return bytes;
    }

    /*
    根据流获取表格
    */
    public static GridA GetGrid(byte[] bytes, INativeBase inative)
    {
        GridA grid = new GridA();
        try
        {
            Binary br = new Binary();
            br.Write(bytes, bytes.length);
            grid.SetNative(inative);
            grid.SetName(br.ReadString());
            int columnsSize = br.ReadInt();
            for (int i = 0; i < columnsSize; i++)
            {
                GridColumn column = new GridColumn();
                column.SetName(br.ReadString());
                column.SetColumnType(br.ReadString());
                grid.AddColumn(column);
            }
            grid.Update();
            List<GridColumn> columns = grid.GetColumns();
            int rowsCount = br.ReadInt();
            for (int i = 0; i < rowsCount; i++)
            {
                GridRow row = new GridRow();
                grid.AddRow(row);
                for (int j = 0; j < columnsSize; j++)
                {
                    GridColumn column = columns.get(j);
                    String columnType = column.GetColumnType().toLowerCase();
                    GridCell cell = new GridCell();
                    row.AddCell(j, cell);
                    if (columnType == "bool")
                    {
                        cell.SetBool(br.ReadBool());
                    }
                    else if (columnType == "double")
                    {
                        cell.SetDouble(br.ReadDouble());
                    }
                    else if (columnType == "float")
                    {
                        cell.SetFloat(br.ReadFloat());
                    }
                    else if (columnType == "int")
                    {
                        cell.SetInt(br.ReadInt());
                    }
                    else if (columnType == "long")
                    {
                        cell.SetLong((long)br.ReadDouble());
                    }
                    else if (columnType == "string")
                    {
                        cell.SetString(br.ReadString());
                    }
                }
            }
            br.Close();
        }
        catch(Exception ex)
        {

        }
        return grid;
    }

    @Override
    public void OnReceive(CMessage message)
    {
        super.OnReceive(message);
        switch (message.m_functionID)
        {
            case FUNCTIONID_EXCUTEID:
                Excute(message);
                break;
        }
    }

    /**
     * 发送消息
     * @param message
     * @param grid
     * @return 
     */
    public int Send(CMessage message, GridA grid)
    {       
         byte[] bytes = GetBytes(grid);
         message.m_bodyLength = bytes.length;
         message.m_body = bytes;
         int ret = Send(message);
         return ret;
    }
}
