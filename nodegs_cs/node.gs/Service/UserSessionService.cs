/**************************************************************************************\
*                                                                                      *
* UserSessionService.cs -  User session service functions, types, and definitions.     *
*                                                                                      *
*               Version 1.00 ��                                                        *
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
    /// �û��Ự��Ϣ
    /// </summary>
    public class UserSession
    {
        #region ���� 2016/6/9
        /// <summary>
        /// �Ự��
        /// </summary>
        public String m_key = "";

        /// <summary>
        /// �û�ID
        /// </summary>
        public int m_userID;

        /// <summary>
        /// �Ựֵ
        /// </summary>
        public String m_value = "";
        #endregion
    }

    /// <summary>
    /// �û�״̬����
    /// </summary>
    public class UserSessionService : BaseService
    {
        #region ���� 2016/5/28
        /// <summary>
        /// �����û�״̬����
        /// </summary>
        public UserSessionService()
        {
            ServiceID = SERVICEID_USERSESSION;
        }

        /// <summary>
        /// �Ự����
        /// </summary>
        private Dictionary<String, UserSession> m_sessions = new Dictionary<String, UserSession>();

        /// <summary>
        /// �û��Ự����ID
        /// </summary>
        public const int SERVICEID_USERSESSION = 8;

        /// <summary>
        /// ����û��Ự����ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_ADDSESSIONS = 0;

        /// <summary>
        /// ɾ���û��Ự����ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_DELETESESSIONS = 1;

        /// <summary>
        /// ��ȡ�����û��Ự����ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_GETSESSIONS = 2;

        /// <summary>
        /// �����û��Ự����ID
        /// </summary>
        public const int FUNCTIONID_USERSESSION_UPDATESESSIONS = 3;

        /// <summary>
        /// ����û��Ự
        /// </summary>
        /// <param name="message">��Ϣ</param>
        /// <returns>״̬</returns>
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
        /// ɾ���û��Ự
        /// </summary>
        /// <param name="message">��Ϣ</param>
        /// <returns>״̬</returns>
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
        /// ��ȡ�û��Ự��Ϣ
        /// </summary>
        /// <param name="session">ָ����Ϣ</param>
        /// <param name="body">����</param>
        /// <param name="bodyLength">���峤��</param>
        /// <returns>״̬</returns>
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
        /// ��ȡ�û��Ự��Ϣ
        /// </summary>
        /// <param name="message">��Ϣ</param>
        /// <returns>״̬</returns>
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
        /// ��ȡ�û��Ự��Ϣ
        /// </summary>
        /// <param name="session">�û��Ự��Ϣ</param>
        /// <param name="userID">�û�ID</param>
        /// <param name="key">��</param>
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
        /// ��������
        /// </summary>
        /// <param name="message">��Ϣ</param>
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
        /// ���ͻỰ
        /// </summary>
        /// <param name="message">��Ϣ</param>
        /// <param name="sessions">�Ự�б�</param>
        /// <returns>״̬</returns>
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
        /// ���»Ự
        /// </summary>
        /// <param name="message">��Ϣ</param>
        /// <returns>״̬</returns>
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
        /// ���»Ự
        /// </summary>
        /// <param name="session">�Ự</param>
        public void UpdateSession(UserSession session)
        {
            String key = CStr.ConvertIntToStr(session.m_userID) + ":" + session.m_key;
            m_sessions[key] = session;
        }
        #endregion
    }
}
