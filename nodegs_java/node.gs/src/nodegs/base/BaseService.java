//GAIA_LORDSETTLEMENT_JAVA_S1:LORD;
//****************************************************************************\
//*                                                                             *
//* BaseService.java -  BaseService functions, types, and definitions *
//*                                                                             *
//*               Version 1.00 ★                                                              	                        *
//*                                                                             *
//*               Copyright (c) 2017-2017, Server. All rights reserved.         *
//*               Created by tanfeng 2017/06/05.                                *
//*                                                                             *
//******************************************************************************

package nodegs.base;

import owchart.owlib.Sock.*;
//****************************************************************************\
//*                                                                             *
//* BaseService.cs -  Base service functions, types, and definitions            *
//*                                                                             *
//*               Version 1.00 ★★★★★                                       *
//*                                                                             *
//*               Copyright (c) 2016-2016, Server. All rights reserved.         *
//*               Created by Lord.                                              *
//*                                                                             *
//******************************************************************************

import java.io.*;
import java.util.*;

/**
 * 基础服务类
 * 
 */
public class BaseService {
	/**
	 * 创建服务
	 * 
	 */
	public BaseService() {
	}

	/**
	 * 析构函数
	 * 
	 */
	protected void finalize() throws Throwable {
		synchronized (m_listeners) {
			m_listeners.clear();
		}
		synchronized (m_waitMessages) {
			m_waitMessages.clear();
		}
	}

	/**
	 * 无压缩
	 * 
	 */
	public static int COMPRESSTYPE_NONE = 0;

	/**
	 * GZIP压缩
	 * 
	 */
	public static int COMPRESSTYPE_GZIP = 1;
	
	/**
	 * 压缩类型
	 */
	protected HashMap<Integer, Integer> m_compressTypes = new HashMap<Integer, Integer>();

	/**
	 * 监听者集合
	 * 
	 */
	private java.util.HashMap<Integer, MessageListener> m_listeners = new java.util.HashMap<Integer, MessageListener>();

	/**
	 * 请求ID
	 * 
	 */
	private static int m_requestID = 10000;

	/**
	 * 所有的服务
	 * 
	 */
	private static java.util.HashMap<Integer, BaseService> m_services = new java.util.HashMap<Integer, BaseService>();

	/**
	 * 等待消息队列
	 * 
	 */
	private java.util.HashMap<Integer, CMessage> m_waitMessages = new java.util.HashMap<Integer, CMessage>();

	private int m_compressType = COMPRESSTYPE_GZIP;

	/**
	 * 获取或设置压缩类型
	 * 
	 */
	public final int GetCompressType() {
		return m_compressType;
	}

	public final void SetCompressType(int value) {
		m_compressType = value;
	}

	protected static long m_downFlow;

	/**
	 * 获取或设置下载流量
	 * 
	 */
	public static long GetDownFlow() {
		return BaseService.m_downFlow;
	}

	public static void SetDownFlow(long value) {
		BaseService.m_downFlow = value;
	}

	private int m_groupID = 0;

	/**
	 * 获取或设置组ID
	 * 
	 */
	public final int GetGroupID() {
		return m_groupID;
	}

	public final void SetGroupID(int value) {
		m_groupID = value;
	}

	private boolean m_isDisposed = false;

	/**
	 * 获取对象是否已被销毁
	 * 
	 */
	public final boolean IsDisposed() {
		return m_isDisposed;
	}

	private int m_serviceID = 0;

	/**
	 * 获取或设置服务的ID
	 * 
	 */
	public final int GetServiceID() {
		return m_serviceID;
	}

	public final void SetServiceID(int value) {
		m_serviceID = value;
	}

	private int m_sessionID = 0;

	/**
	 * 获取或设置登录ID
	 * 
	 */
	public final int GetSessionID() {
		return m_sessionID;
	}

	public final void SetSessionID(int value) {
		m_sessionID = value;
	}

	private int m_socketID = 0;

	public final int GetSocketID() {
		return m_socketID;
	}

	public final void SetSocketID(int socketID) {
		m_socketID = socketID;
	}

	protected static long m_upFlow;

	/**
	 * 获取或设置上传流量
	 * 
	 */
	public static long GetUpFlow() {
		return m_upFlow;
	}

	public static void GetUpFlow(long value) {
		m_upFlow = value;
	}

	/**
	 * 关闭
	 * 
	 * @param socketID
	 *            连接ID
	 * @return 状态
	 */
	public static int CloseServer(int socketID) {
		return Servers.Close(socketID);
	}

	/**
	 * 发送消息
	 * 
	 * @param socketID
	 *            连接ID
	 * @param str
	 *            数据
	 * @param len
	 *            长度
	 * @return 状态
	 */
	public static int SendByServer(int socketID, int localSID, byte[] str, int len) {
		return Servers.Send(socketID, localSID, str, len);
	}

	/**
	 * 启动服务
	 * 
	 * @param port
	 *            端口号
	 * @return 状态
	 */
	public static int StartServer(int port) {
		Servers.SetListener(new ServerSocketListener());
		return Servers.Start(0, port);
	}

	/**
	 * 添加服务
	 * 
	 * @param service
	 *            服务
	 */
	public static void AddService(BaseService service) {
		m_services.put(service.GetServiceID(), service);
	}

	/**
	 * 回调函数
	 * 
	 * @param socketID
	 *            连接ID
	 * @param localSID
	 *            本地连接ID
	 * @param str
	 *            数据
	 * @param len
	 *            长度
	 */
	public static void CallBack(int socketID, int localSID, byte[] str, int len) {
		m_downFlow += len;
		try {
			if (len > 4) {
				Binary br = new Binary();
				br.Write(str, len);
				int head = br.ReadInt();
				int groupID = br.ReadShort();
				int serviceID = br.ReadShort();
				BaseService service = null;
				if (m_services.containsKey(serviceID)) {
					m_services.get(serviceID).OnCallBack(br, socketID, localSID, len);
				}
				br.Close();
			}
		} catch (Exception ex) {
			System.out.println(ex.getMessage() + "\r\n" + ex.getStackTrace());
		}
	}

	/**
	 * 销毁对象
	 * 
	 */
	public void Dispose() {
		if (!m_isDisposed) {
			m_listeners.clear();
			m_isDisposed = true;
		}
	}

	/**
	 * 获取网页数据
	 * 
	 * @param url
	 *            地址
	 * @return 页面源码
	 */
	public static String GetHttpWebRequest(String url) {
		return "";
	}

	/**
	 * 获取请求ID
	 * 
	 * @return 请求ID
	 */
	public static int GetRequestID() {
		return m_requestID++;
	}

	/**
	 * 获取所有的服务
	 * 
	 * @param services
	 *            服务列表
	 */
	public static void GetServices(java.util.ArrayList<BaseService> services) {
		for (BaseService service : m_services.values()) {
			services.add(service);
		}
	}

	/**
	 * 保持活跃
	 * 
	 * @param socketID
	 *            套接字ID
	 * @return 状态
	 */
	public int KeepAlive(int socketID) {
		int ret = -1;
		try {
			Binary bw = new Binary();
			bw.WriteInt((int) 4);
			ret = SendByServer(socketID, m_socketID, bw.GetBytes(), 4);
			bw.Close();
		} catch (Exception ex) {

		}
		return ret;
	}

	/**
	 * 收到消息
	 * 
	 * @param br
	 *            流
	 * @param socketID
	 *            套接字ID
	 * @param localSID
	 *            本地套接字ID
	 * @param len
	 *            长度
	 */
	public void OnCallBack(Binary br, int socketID, int localSID, int len) {
		try {
			int headSize = 4 * 4 + 2 * 3 + 1 * 2;
			int functionID = br.ReadShort();
			int sessionID = br.ReadInt();
			int requestID = br.ReadInt();
			int state = (int) br.ReadChar();
			int compressType = (int) br.ReadChar();
			int bodyLength = br.ReadInt();
			byte[] body = new byte[len - headSize];
			br.ReadBytes(body);
			CMessage message = null;
			byte[] unzipBody = null;
			if (compressType == COMPRESSTYPE_GZIP) {
				unzipBody = CStrA.UnGZip(body);
				bodyLength = unzipBody.length;
				message = new CMessage(GetGroupID(), GetServiceID(), functionID, sessionID, requestID, socketID, state,
						compressType, bodyLength, unzipBody);
				// TODO...
			} else {
				message = new CMessage(GetGroupID(), GetServiceID(), functionID, sessionID, requestID, socketID, state,
						compressType, bodyLength, body);
			}

			OnReceive(message);
			OnWaitMessageHandle(message);
			body = null;
			unzipBody = null;
		} catch (Exception ex) {

		}
	}
	
	/**
	 * 客户端退出方法
	 * 
	 */
	public void OnClientClose(int socketID, int localSID)
	{
        if (m_compressTypes.containsKey(socketID))
        {
            m_compressTypes.remove(socketID);
        }
	}

	/**
	 * 接收数据
	 * 
	 * @param message
	 *            消息
	 */
	public void OnReceive(CMessage message) {
		synchronized (m_compressTypes)
        {
            m_compressTypes.put(message.m_socketID, message.m_compressType);
        }
	}

	/**
	 * 等待消息的处理
	 * 
	 * @param message
	 *            消息
	 */
	public void OnWaitMessageHandle(CMessage message) {
		if (m_waitMessages.size() > 0) {
			synchronized (m_waitMessages) {
				if (m_waitMessages.containsKey(message.m_requestID)) {
					CMessage waitMessage = m_waitMessages.get(message.m_requestID);
					waitMessage.Copy(message);
				}
			}
		}
	}

	/**
	 * 注册数据监听
	 * 
	 * @param requestID
	 *            请求ID
	 * @param callBack
	 *            回调函数
	 */
	public void RegisterListener(int requestID, ListenerMessageCallBack callBack) {
		synchronized (m_listeners) {
			MessageListener listener = null;
			if (!m_listeners.containsKey(requestID)) {
				listener = new MessageListener();
				m_listeners.put(requestID, listener);
			} else {
				listener = m_listeners.get(requestID);
			}
			listener.Add(callBack);
		}
	}

	/**
	 * 注册等待
	 * 
	 * @param requestID
	 *            请求ID
	 * @param message
	 *            消息
	 */
	public void RegisterWait(int requestID, CMessage message) {
		synchronized (m_waitMessages) {
			m_waitMessages.put(requestID, message);
		}
	}

	/**
	 * 发送消息
	 * 
	 * @param message
	 *            消息
	 */
	public int Send(CMessage message) {
		int ret = -1;
		try {
			Binary bw = new Binary();
			byte[] body = message.m_body;
			int bodyLength = message.m_bodyLength;
			int uncBodyLength = bodyLength;
			synchronized (m_compressTypes)
            {
                if (m_compressTypes.containsKey(message.m_socketID))
                {
                    message.m_compressType = m_compressTypes.get(message.m_socketID);
                }
            }
			byte[] zipBody = null;
			if (message.m_compressType == COMPRESSTYPE_GZIP) {
				zipBody = CStrA.GZip(body);
				bodyLength = zipBody.length;
			}
			int len = 4 * 4 + bodyLength + 2 * 3 + 1 * 2;
			bw.WriteInt(len);
			bw.WriteShort((short) message.m_groupID);
			bw.WriteShort((short) message.m_serviceID);
			bw.WriteShort((short) message.m_functionID);
			bw.WriteInt(message.m_sessionID);
			bw.WriteInt(message.m_requestID);
			bw.WriteChar((char) message.m_state);
			bw.WriteChar((char) message.m_compressType);
			bw.WriteInt(uncBodyLength);
			if (message.m_compressType == COMPRESSTYPE_GZIP) {
				bw.WriteBytes(zipBody);
			} else {
				bw.WriteBytes(body);
			}
			byte[] bytes = bw.GetBytes();
			ret = SendByServer(message.m_socketID, m_socketID, bytes, bytes.length);
			bw.Close();
		} catch (Exception ex) {

		}
		return ret;
	}

	/**
	 * 发送到监听者
	 * 
	 * @param message
	 *            消息
	 */
	public void SendToListener(CMessage message) {
		MessageListener listener = null;
		synchronized (m_listeners) {
			if (m_listeners.containsKey(message.m_requestID)) {
				listener = m_listeners.get(message.m_requestID);
			}
		}
		if (listener != null) {
			listener.CallBack(message);
		}
	}

	/**
	 * 取消注册数据监听
	 * 
	 * @param requestID
	 *            请求ID
	 */
	public void UnRegisterListener(int requestID) {
		synchronized (m_listeners) {
			m_listeners.remove(requestID);
		}
	}

	/**
	 * 取消注册监听
	 * 
	 * @param requestID
	 *            请求ID
	 * @param callBack
	 *            回调函数
	 */
	public void UnRegisterListener(int requestID, ListenerMessageCallBack callBack) {
		synchronized (m_listeners) {
			if (m_listeners.containsKey(requestID)) {
				m_listeners.get(requestID).Remove(callBack);
			}
		}
	}

	/**
	 * 取消注册等待
	 * 
	 * @param requestID
	 *            请求ID
	 */
	public void UnRegisterWait(int requestID) {
		synchronized (m_waitMessages) {
			if (m_waitMessages.containsKey(requestID)) {
				m_waitMessages.remove(requestID);
			}
		}
	}

	/**
	 * 等待消息
	 * 
	 * @param requestID
	 *            请求ID
	 * @param timeout
	 *            超时
	 * @return 状态
	 */
	public int WaitMessage(int requestID, int timeout) {
		int state = 0;
		try {
			while (timeout > 0) {
				synchronized (m_waitMessages) {
					if (m_waitMessages.containsKey(requestID)) {
						if (m_waitMessages.get(requestID).m_bodyLength > 0) {
							state = 1;
							break;
						}
					} else {
						break;
					}
				}
				timeout -= 10;
				Thread.sleep(10);
			}
			UnRegisterWait(requestID);
		} catch (Exception ex) {
		}
		return state;
	}

	/**
	 * 等待消息
	 * 
	 * @param socketID
	 *            目标SocketID
	 * @param localSID
	 *            本地SocketID
	 * @param state
	 *            状态
	 * @param log
	 *            日志
	 * @return 状态
	 */
	public static void WriteLog(int socketID, int localSID, int state, String log) {
		if(state == 2)
		{
			for (BaseService service : m_services.values()) {
				service.OnClientClose(socketID, localSID);
			}
		}
	}
}