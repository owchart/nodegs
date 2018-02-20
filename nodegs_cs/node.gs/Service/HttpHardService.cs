/*******************************************************************************************\
*                                                                                           *
* HttpService.cs -  Http service functions, types, and definitions.                     *
*                                                                                           *
*               Version 1.00  ★★★                                                        *
*                                                                                           *
*               Copyright (c) 2016-2016, Piratecat. All rights reserved.                    *
*               Created by Lord 2016/10/17.                                                  *
*                                                                                           *
********************************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using OwLibSV;
using System.IO.Compression;
using OwLib;

namespace node.gs
{
    /// <summary>
    /// HTTP服务
    /// </summary>
    public class HttpHardService : BaseService
    {
        /// <summary>
        /// 创建HTTP服务
        /// </summary>
        public HttpHardService()
        {
            ServiceID = SERVICEID_HTTPHARD;
        }

        /// <summary>
        /// 用户会话服务ID
        /// </summary>
        public const int SERVICEID_HTTPHARD = 20;

        /// <summary>
        /// POST方法ID
        /// </summary>
        public const int FUNCTIONID_HTTPHARD_TEST = 0;

        /// <summary>
        /// 获取表格数据
        /// </summary>
        /// <param name="grid">表格</param>
        /// <returns>流</returns>
        public static byte[] GetBytes(GridA grid)
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
        /// <param name="bytes">流</param>
        /// <param name="native">方法库</param>
        /// <returns>表格</returns>
        public static GridA GetGrid(byte[] bytes, INativeBase native)
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
            SendToListener(message);
        }

        /// <summary>
        /// 发送消息
        /// </summary>
        /// <param name="message">消息</param>
        public override int Send(CMessage message)
        {
            Binary bw = new Binary();
            byte[] body = message.m_body;
            int bodyLength = message.m_bodyLength;
            int uncBodyLength = bodyLength;
            lock (m_compressTypes)
            {
                if (m_compressTypes.ContainsKey(message.m_socketID))
                {
                    message.m_compressType = m_compressTypes[message.m_socketID];
                }
            }
            if (message.m_compressType == COMPRESSTYPE_GZIP)
            {
                using (MemoryStream cms = new MemoryStream())
                {
                    using (GZipStream gzip = new GZipStream(cms, CompressionMode.Compress))
                    {
                        gzip.Write(body, 0, body.Length);
                    }
                    body = cms.ToArray();
                    bodyLength = body.Length;
                }
            }
            int len = sizeof(int) * 4 + bodyLength + sizeof(short) * 3 + sizeof(byte) * 2;
            bw.WriteInt(len);
            bw.WriteShort((short)message.m_groupID);
            bw.WriteShort((short)message.m_serviceID);
            bw.WriteShort((short)message.m_functionID);
            bw.WriteInt(message.m_sessionID);
            bw.WriteInt(message.m_requestID);
            bw.WriteByte((byte)message.m_state);
            bw.WriteByte((byte)message.m_compressType);
            bw.WriteInt(uncBodyLength);
            bw.WriteBytes(body);
            byte[] bytes = bw.GetBytes();
            lock (DataCenter.NodeService.m_httpDatas)
            {
                DataCenter.NodeService.m_httpDatas[message.m_socketID].m_resBytes = bytes;
            }
            int ret = bytes.Length;
            UpFlow += ret;
            bw.Close();
            OnClientClose(message.m_socketID, 0);
            return ret;
        }
    }
}
