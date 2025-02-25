/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegsclient.base;

import gaia.owchart.owchart_android.Base.INativeBase;
import gaia.owchart.owchart_android.Grid.GridA;
import gaia.owchart.owchart_android.Grid.GridBoolCell;
import gaia.owchart.owchart_android.Grid.GridCell;
import gaia.owchart.owchart_android.Grid.GridColumn;
import gaia.owchart.owchart_android.Grid.GridDoubleCell;
import gaia.owchart.owchart_android.Grid.GridFloatCell;
import gaia.owchart.owchart_android.Grid.GridIntCell;
import gaia.owchart.owchart_android.Grid.GridLongCell;
import gaia.owchart.owchart_android.Grid.GridRow;
import gaia.owchart.owchart_android.Grid.GridStringCell;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.List;
import nodegsclient.base.BaseService;
import static nodegsclient.base.BaseService.COMPRESSTYPE_GZIP;
import static nodegsclient.base.BaseService.COMPRESSTYPE_NONE;
import nodegsclient.base.Binary;
import nodegsclient.base.CMessage;

/**
 *
 * @author GAIA
 */
public class EventService extends BaseService {
	/*
	 * 创建服务
	 */
	public EventService() {
		SetCompressType(COMPRESSTYPE_NONE);
		SetServiceID(SERVICEID_EVENT);
	}

	public static final int SERVICEID_EVENT = 30;

	public static final int FUNCTIONID_EXCUTEID = 0;

	private int m_socketID = 0;

	/**
	 * 执行
	 * 
	 * @param requestID
	 * @param grid
	 * @return
	 */
	public int Excute(int requestID, GridA grid) {
		int ret = Send(FUNCTIONID_EXCUTEID, requestID, grid);
		return ret > 0 ? 1 : 0;
	}

	/**
	 * 获取或设置套接字ID
	 * 
	 */
	public final int GetSocketID() {
		return m_socketID;
	}

	public final void SetSocketID(int value) {
		m_socketID = value;
	}

	@Override
	public void OnReceive(CMessage message) {
		super.OnReceive(message);
		SendToListener(message);
	}

	/*
	 * 获取表格的流
	 */
	public static byte[] GetBytes(GridA grid) {
		byte[] bytes = null;
		try {
			Binary br = new Binary();
			br.WriteString(grid.GetName());
			ArrayList<GridColumn> columns = grid.GetColumns();
			int columnsSize = columns.size();
			br.WriteInt(columnsSize);
			for (int i = 0; i < columnsSize; i++) {
				GridColumn column = columns.get(i);
				br.WriteString(column.GetName());
				br.WriteString(column.GetColumnType());
			}
			List<GridRow> rows = grid.GetRows();
			int rowsCount = rows.size();
			br.WriteInt(rowsCount);
			for (int i = 0; i < rowsCount; i++) {
				GridRow row = rows.get(i);
				for (int j = 0; j < columnsSize; j++) {
					GridColumn column = columns.get(j);
					String columnType = column.GetColumnType().toLowerCase();
					GridCell cell = null;
					if (columnType.equals("bool")) {
						cell = new GridBoolCell();
						row.AddCell(j, cell);
						cell.SetBool(br.ReadBool());
					} else if (columnType.equals("double")) {
						cell = new GridDoubleCell();
						row.AddCell(j, cell);
						cell.SetDouble(br.ReadDouble());
					} else if (columnType.equals("float")) {
						cell = new GridFloatCell();
						row.AddCell(j, cell);
						cell.SetFloat(br.ReadFloat());
					} else if (columnType.equals("int")) {
						cell = new GridIntCell();
						row.AddCell(j, cell);
						cell.SetInt(br.ReadInt());
					} else if (columnType.equals("long")) {
						cell = new GridLongCell();
						row.AddCell(j, cell);
						cell.SetLong((long) br.ReadDouble());
					} else if (columnType.equals("string")) {
						cell = new GridStringCell();
						row.AddCell(j, cell);
						cell.SetString(br.ReadString());
					} else {
						cell = new GridStringCell();
						row.AddCell(j, cell);
						cell.SetString(br.ReadString());
					}
				}
			}
			bytes = br.GetBytes();
			br.Close();
		} catch (Exception ex) {

		}
		return bytes;
	}

	/*
	 * 根据流获取表格
	 */
	public static GridA GetGrid(byte[] bytes, INativeBase inative) {
		GridA grid = new GridA();
		try {
			Binary br = new Binary();
			br.Write(bytes, bytes.length);
			grid.SetNative(inative);
			grid.SetName(br.ReadString());
			int columnsSize = br.ReadInt();
			for (int i = 0; i < columnsSize; i++) {
				GridColumn column = new GridColumn();
				column.SetName(br.ReadString());
				column.SetColumnType(br.ReadString());
				grid.AddColumn(column);
			}
			grid.Update();
			List<GridColumn> columns = grid.GetColumns();
			int rowsCount = br.ReadInt();
			for (int i = 0; i < rowsCount; i++) {
				GridRow row = new GridRow();
				grid.AddRow(row);
				for (int j = 0; j < columnsSize; j++) {
					GridColumn column = columns.get(j);
					String columnType = column.GetColumnType().toLowerCase();
					GridCell cell = new GridCell();
					row.AddCell(j, cell);
					if (columnType == "bool") {
						cell.SetBool(br.ReadBool());
					} else if (columnType == "double") {
						cell.SetDouble(br.ReadDouble());
					} else if (columnType == "float") {
						cell.SetFloat(br.ReadFloat());
					} else if (columnType == "int") {
						cell.SetInt(br.ReadInt());
					} else if (columnType == "long") {
						cell.SetLong((long) br.ReadDouble());
					} else if (columnType == "string") {
						cell.SetString(br.ReadString());
					}
				}
			}
			br.Close();
		} catch (Exception ex) {

		}
		return grid;
	}

	/**
	 * 发送消息
	 * 
	 * @param functionID
	 * @param requestID
	 * @param grid
	 * @return
	 */
	public int Send(int functionID, int requestID, GridA grid) {
		byte[] bytes = GetBytes(grid);
		int ret = Send(new CMessage(GetGroupID(), GetServiceID(), functionID, GetSessionID(), requestID, GetSocketID(),
				0, GetCompressType(), bytes.length, bytes));
		return ret;
	}
}
