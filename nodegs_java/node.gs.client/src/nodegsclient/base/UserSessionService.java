package nodegsclient.base;
import nodegsclient.base.*;
import java.io.*;

//*************************************************************************************\
//*                                                                                      *
//* UserSessionService.cs -  User session service functions, types, and definitions.     *
//*                                                                                      *
//*               Version 1.00 ★                                                        *
//*                                                                                      *
//*               Copyright (c) 2016-2016, Client. All rights reserved.                  *
//*               Created by TangWen.                                                    *
//*                                                                                      *
//**************************************************************************************


/** 
 用户会话服务
 
*/
public class UserSessionService extends BaseService
{
	/** 
	 创建用户会话服务
	 
	*/
	public UserSessionService()
	{
		SetServiceID(SERVICEID_USERSESSION);
	}

	/** 
	 用户会话服务ID
	 
	*/
	public static final int SERVICEID_USERSESSION = 8;

	/** 
	 添加用户会话方法ID
	 
	*/
	public static final int FUNCTIONID_USERSESSION_ADDSESSION = 0;

	/** 
	 删除用户会话方法ID
	 
	*/
	public static final int FUNCTIONID_USERSESSION_DELETESESSION = 1;

	/** 
	 获取所有用户会话方法ID
	 
	*/
	public static final int FUNCTIONID_USERSESSION_GETSESSIONS = 2;

	/** 
	 更新用户会话方法ID
	 
	*/
	public static final int FUNCTIONID_USERSESSION_UPDATESESSION = 3;

	private int m_socketID = 0;

	/** 
	 获取或设置套接字ID
	 
	*/
	public final int GetSocketID()
	{
		return m_socketID;
	}
	public final void SetSocketID(int value)
	{
		m_socketID = value;
	}

	/** 
	 添加用户会话
	 
	 @param requestID 请求ID
	 @param session 用户会话
	 @return 状态
	*/
	public final int AddSession(int requestID, UserSession session)
	{
		java.util.ArrayList<UserSession> sessions = new java.util.ArrayList<UserSession>();
		sessions.add(session);
		int ret = Send(FUNCTIONID_USERSESSION_ADDSESSION, requestID, sessions);
		sessions.clear();
		return ret > 0 ? 1 : 0;
	}

	/** 
	 删除用户会话
	 
	 @param requestID 请求ID
	 @param session 用户会话
	 @return 状态
	*/
	public final int DeleteSession(int requestID, UserSession session)
	{
		java.util.ArrayList<UserSession> sessions = new java.util.ArrayList<UserSession>();
		sessions.add(session);
		int ret = Send(FUNCTIONID_USERSESSION_DELETESESSION, requestID, sessions);
		sessions.clear();
		return ret > 0 ? 1 : 0;
	}

	/** 
	 获取所有用户会话
	 
	 @param requestID 请求ID
	 @param session 用户会话
	 @return 状态
	*/
	public final int GetSessions(int requestID, UserSession session)
	{
		java.util.ArrayList<UserSession> sessions = new java.util.ArrayList<UserSession>();
		sessions.add(session);
		int ret = Send(FUNCTIONID_USERSESSION_GETSESSIONS, requestID, sessions);
		sessions.clear();
		return ret > 0 ? 1 : 0;
	}

	/** 
	 通过流获取用户会话
	 
	 @param sessions 用户会话
	 @param body 包体
	 @param bodyLength 包体长度
	*/
	public static int GetSessions(java.util.ArrayList<UserSession> sessions, byte[] body, int bodyLength)
	{
		try
		{
			Binary br = new Binary();
			br.Write(body, bodyLength);
			int size = br.ReadInt();
			if (size > 0)
			{
				for (int i = 0; i < size; i++)
				{
					UserSession session = new UserSession();
					session.m_userID = br.ReadInt();
					session.m_key = br.ReadString();
					session.m_value = br.ReadString();
					sessions.add(session);
				}
			}
			br.Close();
		}
		catch(Exception ex)
		{

		}
		return 1;
	}

	/** 
	 接收数据
	 
	 @param message 消息
	*/
	@Override
	public void OnReceive(CMessage message)
	{
		super.OnReceive(message);
		SendToListener(message);
	}

	/** 
	 发送消息
	 
	 @param functionID 方法ID
	 @param requestID 请求ID
	 @param sessions 用户会话集合
	 @return 状态
	*/
	public final int Send(int functionID, int requestID, java.util.ArrayList<UserSession> sessions)
	{
		int ret = -1;
		try
		{
			Binary bw = new Binary();
			int size = sessions.size();
			bw.WriteInt(size);
			for (int i = 0; i < size; i++)
			{
				UserSession session = sessions.get(i);
				bw.WriteInt(session.m_userID);
				bw.WriteString(session.m_key);
				bw.WriteString(session.m_value);
			}
			byte[] bytes = bw.GetBytes();
			ret = Send(new CMessage(GetGroupID(), GetServiceID(), functionID, GetSessionID(), requestID, m_socketID, 0, GetCompressType(), bytes.length, bytes));
			bw.Close();
		}
		catch (Exception ex)
		{

		}
		return ret;
	}

	/** 
	 更新会话
	 
	 @param requestID 请求ID
	 @param session 会话
	 @return 状态
	*/
	public final int UpdateSession(int requestID, UserSession session)
	{
		java.util.ArrayList<UserSession> sessions = new java.util.ArrayList<UserSession>();
		sessions.add(session);
		int ret = Send(FUNCTIONID_USERSESSION_UPDATESESSION, requestID, sessions);
		sessions.clear();
		return ret > 0 ? 1 : 0;
	}
}