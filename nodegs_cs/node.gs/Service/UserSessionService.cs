/**************************************************************************************\
*                                                                                      *
* UserSessionService.cs -  User session service functions, types, and definitions.     *
*                                                                                      *
*               Version 1.00 ★                                                        *
*                                                                                      *
*               Copyright (c) 2016-2016, Server. All rights reserved.                  *
*               Created by TangWen.                                                    *
*                                                                                      *
***************************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using OwLib;

namespace OwLibSV
{
    /// <summary>
    /// 用户会话信息
    /// </summary>
    public class UserSession
    {
        #region 汤文 2016/6/9
        /// <summary>
        /// 会话键
        /// </summary>
        public String m_key = "";

        /// <summary>
        /// 用户ID
        /// </summary>
        public int m_userID;

        /// <summary>
        /// 会话值
        /// </summary>
        public String m_value = "";
        #endregion
    }

    /// <summary>
    /// 用户状态服务
    /// </summary>
    public class UserSessionService : BaseService
    {
        #region 汤文 2016/5/28
        /// <summary>
        /// 创建用户状态服务
        /// </summary>
        public UserSessionService()
        {
            ServiceID = SERVICEID_USERSESSION;
        }

        /// <summary>
        /// 会话数据
        /// </summary>
        private Dictionary<String, UserSession> m_sessions = new Dictionary<String, UserSession>();

        /// <summary>
        /// 用户会话服务ID
        /// </summary>
        public const int SERVICEID_USERSESSION = 8;

        /// <summary>
        /// 添加用户会话方法ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_ADDSESSIONS = 0;

        /// <summary>
        /// 删除用户会话方法ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_DELETESESSIONS = 1;

        /// <summary>
        /// 获取所有用户会话方法ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_GETSESSIONS = 2;

        /// <summary>
        /// 更新用户会话方法ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_UPDATESESSIONS = 3;

        /// <summary>
        /// 添加用户会话
        /// </summary>
        /// <param name="message">消息</param>
        /// <returns>状态</returns>
        public int AddSessions(CMessage message)
        {
            List<UserSession> sessions = new List<UserSession>();
            GetSessions(sessions, message.m_body, message.m_bodyLength);
            int sessionsSize = sessions.Count;
            List<UserSession> addSessions = new List<UserSession>();
            for (int i = 0; i < sessionsSize; i++)
            {
                UserSession receive = sessions[i];
                List<UserSession> oldSessions = new List<UserSession>();
                GetSessions(oldSessions, receive.m_userID, receive.m_key);
                if (oldSessions.Count > 0)
                {
                    UpdateSession(receive);
                    oldSessions.Clear();
                }
                else
                {
                    String key = CStr.ConvertIntToStr(receive.m_userID) + ":" + receive.m_key;
                    m_sessions[key] = receive;
                }
                addSessions.Add(receive);
            }
            int ret = Send(message, addSessions);
            sessions.Clear();
            addSessions.Clear();
            return ret;
        }

        /// <summary>
        /// 删除用户会话
        /// </summary>
        /// <param name="message">消息</param>
        /// <returns>状态</returns>
        public int DeleteSessions(CMessage message)
        {
            List<UserSession> sessions = new List<UserSession>();
            GetSessions(sessions, message.m_body, message.m_bodyLength);
            int sessionsSize = sessions.Count;
            List<UserSession> deleteSessions = new List<UserSession>();
            for (int i = 0; i < sessionsSize; i++)
            {
                UserSession receive = sessions[i];
                String key = CStr.ConvertIntToStr(receive.m_userID) + ":" + receive.m_key;
                m_sessions.Remove(key);
            }
            int ret = Send(message, deleteSessions);
            sessions.Clear();
            deleteSessions.Clear();
            return ret;
        }

        /// <summary>
        /// 获取用户会话信息
        /// </summary>
        /// <param name="session">指标信息</param>
        /// <param name="body">包体</param>
        /// <param name="bodyLength">包体长度</param>
        /// <returns>状态</returns>
        public int GetSessions(List<UserSession> sessions, byte[] body, int bodyLength)
        {
            Binary br = new Binary();
            br.Write(body, bodyLength);
            int userSessionSize = br.ReadInt();
            for (int i = 0; i < userSessionSize; i++)
            {
                UserSession session = new UserSession();
                session.m_userID = br.ReadInt();
                session.m_key = br.ReadString();
                session.m_value = br.ReadString();
                sessions.Add(session);
            }
            br.Close();
            return 1;
        }

        /// <summary>
        /// 获取用户会话信息
        /// </summary>
        /// <param name="message">消息</param>
        /// <returns>状态</returns>
        public int GetSessions(CMessage message)
        {
            UserSession receive = new UserSession();
            List<UserSession> sessions = new List<UserSession>();
            GetSessions(sessions, message.m_body, message.m_bodyLength);
            receive = sessions[0];
            sessions.Clear();
            GetSessions(sessions, receive.m_userID, receive.m_key);
            int ret = Send(message, sessions);
            sessions.Clear();
            return ret;
        }

        /// <summary>
        /// 获取用户会话信息
        /// </summary>
        /// <param name="session">用户会话信息</param>
        /// <param name="userID">用户ID</param>
        /// <param name="key">键</param>
        public int GetSessions(List<UserSession> sessions, int userID, String key)
        {
            if (key != null && key.Length > 0)
            {
                String findKey = CStr.ConvertIntToStr(userID) + ":" + key;
                if (m_sessions.ContainsKey(findKey))
                {
                    sessions.Add(m_sessions[findKey]);
                }
            }
            else
            {
                foreach (UserSession session in m_sessions.Values)
                {
                    if (session.m_userID == userID)
                    {
                        sessions.Add(session);
                    }
                }
            }
            return 1;
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
                case FUNCTIONID_USERSESSION_ADDSESSIONS:
                    AddSessions(message);
                    break;
                case FUNCTIONID_USERSESSION_DELETESESSIONS:
                    DeleteSessions(message);
                    break;
                case FUNCTIONID_USERSESSION_GETSESSIONS:
                    GetSessions(message);
                    break;
                case FUNCTIONID_USERSESSION_UPDATESESSIONS:
                    UpdateSessions(message);
                    break;
            }
        }

        /// <summary>
        /// 发送会话
        /// </summary>
        /// <param name="message">消息</param>
        /// <param name="sessions">会话列表</param>
        /// <returns>状态</returns>
        public int Send(CMessage message, List<UserSession> sessions)
        {
            Binary bw = new Binary();
            int size = sessions.Count;
            bw.WriteInt(size);
            for (int i = 0; i < size; i++)
            {
                UserSession session = sessions[i];
                bw.WriteInt(session.m_userID);
                bw.WriteString(session.m_key);
                bw.WriteString(session.m_value);
            }
            byte[] bytes = bw.GetBytes();
            message.m_bodyLength = bytes.Length;
            message.m_body = bytes;
            int ret = Send(message);
            bw.Close();
            return ret;
        }

        /// <summary>
        /// 更新会话
        /// </summary>
        /// <param name="message">消息</param>
        /// <returns>状态</returns>
        public int UpdateSessions(CMessage message)
        {
            List<UserSession> sessions = new List<UserSession>();
            GetSessions(sessions, message.m_body, message.m_bodyLength);
            int sessionsSize = sessions.Count;
            List<UserSession> updateSessions = new List<UserSession>();
            for (int i = 0; i < sessionsSize; i++)
            {
                UserSession receive = sessions[i];
                UpdateSession(receive);
                updateSessions.Add(receive);
            }
            int ret = Send(message, updateSessions);
            sessions.Clear();
            updateSessions.Clear();
            return ret;
        }

        /// <summary>
        /// 更新会话
        /// </summary>
        /// <param name="session">会话</param>
        public void UpdateSession(UserSession session)
        {
            String key = CStr.ConvertIntToStr(session.m_userID) + ":" + session.m_key;
            m_sessions[key] = session;
        }
        #endregion
    }
}
