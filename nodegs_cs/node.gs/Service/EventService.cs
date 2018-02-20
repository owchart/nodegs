using System;
using System.Collections.Generic;
using System.Text;
using OwLibSV;
using OwLib;

namespace node.gs
{
    /// <summary>
    /// 事件服务
    /// </summary>
    public class EventService:BaseService
    {
        /// <summary>
        /// 创建事件服务
        /// </summary>
        public EventService()
        {
            ServiceID = SERVICEID_EVENT;
        }

        private INativeBase m_native = NativeHandler.CreateNative();

        /// <summary>
        /// 事件服务ID
        /// </summary>
        public const int SERVICEID_EVENT = 30;

        /// <summary>
        /// 方法ID
        /// </summary>
        public const int FUNCTIONID_EXCUTEID = 0;

        private int m_socketID = 0;

        /// <summary>
        /// 获取或设置套接字ID
        /// </summary>
        public int SocketID
        {
            get { return m_socketID; }
            set { m_socketID = value; }
        }

        /// <summary>
        /// 获取表格数据
        /// </summary>
        /// <param name="grid">表格</param>
        /// <returns>流</returns>
        public byte[] GetBytes(GridA grid)
        {
            Binary br = new Binary();
            br.WriteString(grid.Name);
            List<GridColumn> columns = grid.GetColumns();
            int columnsSize = columns.Count;
            br.WriteInt(columnsSize);
            for (int i = 0; i < columnsSize; i++)
            {
                GridColumn column = columns[i];
                br.WriteString(column.Name);
                br.WriteString(column.ColumnType);
            }
            List<GridRow> rows = grid.GetRows();
            int rowsCount = rows.Count;
            br.WriteInt(rowsCount);
            for (int i = 0; i < rowsCount; i++)
            {
                GridRow row = rows[i];
                for (int j = 0; j < columnsSize; j++)
                {
                    GridColumn column = columns[j];
                    String columnType = column.ColumnType.ToLower();
                    GridCell cell = row.GetCell(j);
                    if (columnType == "bool")
                    {
                        br.WriteBool(cell.GetBool());
                    }
                    else if (columnType == "double")
                    {
                        br.WriteDouble(cell.GetDouble());
                    }
                    else if (columnType == "float")
                    {
                        br.WriteFloat(cell.GetFloat());
                    }
                    else if (columnType == "int")
                    {
                        br.WriteInt(cell.GetInt());
                    }
                    else if (columnType == "long")
                    {
                        br.WriteDouble(cell.GetLong());
                    }
                    else if (columnType == "string")
                    {
                        br.WriteString(cell.GetString());
                    }
                }
            }
            byte[] bytes = br.GetBytes();
            br.Close();
            return bytes;
        }

        /// <summary>
        /// 获取表格
        /// </summary>
        /// <param name="message"></param>
        /// <returns></returns>
        public int Excute(CMessage message)
        {
            GridA grid = GetGrid(message.m_body, m_native);
            Send(message, grid);
            return 1;
        }

        /// <summary>
        /// 获取表格
        /// </summary>
        /// <param name="bytes">流</param>
        /// <param name="native">方法库</param>
        /// <returns>表格</returns>
        public GridA GetGrid(byte[] bytes, INativeBase native)
        {
            Binary br = new Binary();
            br.Write(bytes, bytes.Length);
            GridA grid = new GridA();
            grid.Native = native;
            grid.Name = br.ReadString();
            int columnsSize = br.ReadInt();
            for (int i = 0; i < columnsSize; i++)
            {
                GridColumn column = new GridColumn();
                column.Name = br.ReadString();
                column.ColumnType = br.ReadString();
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
                    GridColumn column = columns[j];
                    string columnType = column.ColumnType.ToLower();
                    GridCell cell = null;
                    if (columnType == "bool")
                    {
                        cell = new GridBoolCell();
                        row.AddCell(j, cell);
                        cell.SetBool(br.ReadBool());
                    }
                    else if (columnType == "double")
                    {
                        cell = new GridDoubleCell();
                        row.AddCell(j, cell);
                        cell.SetDouble(br.ReadDouble());
                    }
                    else if (columnType == "float")
                    {
                        cell = new GridFloatCell();
                        row.AddCell(j, cell);
                        cell.SetFloat(br.ReadFloat());
                    }
                    else if (columnType == "int")
                    {
                        cell = new GridIntCell();
                        row.AddCell(j, cell);
                        cell.SetInt(br.ReadInt());
                    }
                    else if (columnType == "long")
                    {
                        cell = new GridLongCell();
                        row.AddCell(j, cell);
                        cell.SetLong((long)br.ReadDouble());
                    }
                    else if (columnType == "string")
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
            br.Close();
            return grid;
        }

        /// <summary>
        /// 接收数据
        /// </summary>
        /// <param name="message">消息</param>
        public override void OnReceive(CMessage message)
        {
            base.OnReceive(message);
            switch (message.m_functionID)
            {
                case FUNCTIONID_EXCUTEID:
                    Excute(message);
                    break;
            }
        }

        /// <summary>
        /// 发送消息
        /// </summary>
        /// <param name="message">消息</param>
        /// <param name="grid">集合</param>
        public int Send(CMessage message, GridA grid)
        {
            byte[] bytes = GetBytes(grid);
            message.m_bodyLength = bytes.Length;
            message.m_body = bytes;
            int ret = Send(message);
            return ret;
        }
    }
}
