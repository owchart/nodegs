package nodegs.service;

//*************************************************************************************\
//*                                                                                      *
//* UserSessionService.cs -  User session service functions, types, and definitions.     *
//*                                                                                      *
//*               Version 1.00 ★                                                        *
//*                                                                                      *
//*               Copyright (c) 2016-2016, Server. All rights reserved.                  *
//*               Created by TangWen.                                                    *
//*                                                                                      *
//**************************************************************************************

import nodegs.base.HttpHardService;
import com.mongodb.*;
import com.mongodb.gridfs.*;
import nodegs.base.*;
import java.util.*;
import gaia.owchart.owchart_android.Base.*;

/** 
 用户状态服务
 
*/
public class UserSessionService extends HttpHardService
{
    /** 
     创建用户状态服务

    */
    public UserSessionService()
    {
        SetServiceID(SERVICEID_USERSESSION);
        m_mongoService = new MongoService();
    }

    /** 
     用户会话服务ID

    */
    public static final int SERVICEID_USERSESSION = 8;

    /** 
     添加用户会话方法ID

    */
    public static final int FUNCTIONID_USERSESSION_ADDSESSIONS = 0;

    /** 
     删除用户会话方法ID

    */
    public static final int FUNCTIONID_USERSESSION_DELETESESSIONS = 1;

    /** 
     获取所有用户会话方法ID

    */
    public static final int FUNCTIONID_USERSESSION_GETSESSIONS = 2;

    /** 
     更新用户会话方法ID

    */
    public static final int FUNCTIONID_USERSESSION_UPDATESESSIONS = 3;
    
    public final String MONGODB_USER_SESSION = "UserSession";
    
    private MongoService m_mongoService;

    /** 
     添加用户会话

     @param message 消息
     @return 状态
    */
    public final int AddSessions(CMessage message)
    {
        java.util.ArrayList<UserSession> sessions = new java.util.ArrayList<UserSession>();
        GetSessions(sessions, message.m_body, message.m_bodyLength);
        int sessionsSize = sessions.size();
        java.util.ArrayList<UserSession> addSessions = new java.util.ArrayList<UserSession>();
        for (int i = 0; i < sessionsSize; i++)
        {
            UserSession receive = sessions.get(i);
            java.util.ArrayList<UserSession> oldSessions = new java.util.ArrayList<UserSession>();
            GetSessions(oldSessions, receive.m_userID, receive.m_key);
            if (oldSessions.size() > 0)
            {
                UpdateSession(receive);
                oldSessions.clear();
            }
            else
            {
                //插入mongo
                GetCollection().insert(UserSession2DBObject(receive));
            }
            addSessions.add(receive);
        }
        int ret = Send(message, addSessions);
        sessions.clear();
        addSessions.clear();
        return ret;
    }
    
    public static UserSession DBObject2UserSession(DBObject DBObject)
    {
        @SuppressWarnings("unchecked")
        Map<String, Object> userMap = (Map<String, Object>) DBObject.toMap();

        UserSession session = new UserSession();                  
        session.m_userID = CStr.ConvertStrToInt(String.valueOf(userMap.get("userID")));
        session.m_key = String.valueOf(userMap.get("key"));
        session.m_key = String.valueOf(userMap.get("value"));
        return session;
    }

    /** 
     删除用户会话

     @param message 消息
     @return 状态
    */
    public final int DeleteSessions(CMessage message)
    {
        java.util.ArrayList<UserSession> sessions = new java.util.ArrayList<UserSession>();
        GetSessions(sessions, message.m_body, message.m_bodyLength);
        int sessionsSize = sessions.size();
        java.util.ArrayList<UserSession> deleteSessions = new java.util.ArrayList<UserSession>();
        for (int i = 0; i < sessionsSize; i++)
        {
            UserSession receive = sessions.get(i);
            //mongo删除
            GetCollection().remove(UserSession2DBObject(receive));
        }
        int ret = Send(message, deleteSessions);
        sessions.clear();
        deleteSessions.clear();
        return ret;
    }

    public DBCollection GetCollection()
    {
        DBCollection collection = m_mongoService.GetCollectionByName(MONGODB_USER_SESSION);
        return collection;
    }
        
    /** 
     获取用户会话信息

     @param session 指标信息
     @param body 包体
     @param bodyLength 包体长度
     @return 状态
    */
    public final int GetSessions(ArrayList<UserSession> sessions, byte[] body, int bodyLength)
    {
        Binary br = new Binary();
        try
        {
            br.Write(body, bodyLength);
            int userSessionSize = br.ReadInt();
            for (int i = 0; i < userSessionSize; i++)
            {
                UserSession session = new UserSession();
                session.m_userID = br.ReadInt();
                session.m_key = br.ReadString();
                session.m_value = br.ReadString();
                sessions.add(session);
            }
            br.Close();
        }
        catch(Exception ex)
        {

        }
        return 1;
    }

    /** 
     获取用户会话信息

     @param message 消息
     @return 状态
    */
    public final int GetSessions(CMessage message)
    {
        UserSession receive = new UserSession();
        java.util.ArrayList<UserSession> sessions = new java.util.ArrayList<UserSession>();
        GetSessions(sessions, message.m_body, message.m_bodyLength);
        receive = sessions.get(0);
        sessions.clear();
        GetSessions(sessions, receive.m_userID, receive.m_key);
        int ret = Send(message, sessions);
        sessions.clear();
        return ret;
    }

    /** 
     获取用户会话信息

     @param session 用户会话信息
     @param userID 用户ID
     @param key 键
    */
    public final int GetSessions(ArrayList<UserSession> sessions, int userID, String key)
    {
        DBCollection userCollection = GetCollection();
        BasicDBObject query =new BasicDBObject();
        query.put("key", key);
        UserSession user = null;

        ArrayList<UserSession> allSession = new ArrayList<UserSession>();
        DBCursor cursor = userCollection.find(query);
        while(cursor.hasNext())
        {
            DBObject dbObject = cursor.next();            
            user = DBObject2UserSession(dbObject);
            allSession.add(user);              
        }

        int size = allSession.size();
        if (key != null && key.length() > 0)
        {
             for (int i = 0; i < size; i++)
             {
                 if (allSession.get(i).m_key.equals(key))
                 {
                     sessions.add(allSession.get(i));
                 }
             }
        }
        else
        {
            if (userID > 0)
            {
                for (int i = 0; i < size; i++)
                {
                    if (allSession.get(i).m_userID == userID)
                    {
                        sessions.add(allSession.get(i));
                    } 
                }
            }
            else
            {
                sessions = allSession;
            }
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

    /** 
     发送会话

    @param message 消息
    @param sessions 会话列表
    @return 状态
   */
    public final int Send(CMessage message, java.util.ArrayList<UserSession> sessions)
    {
         Binary bw = new Binary();
         int size = sessions.size();
         try
         {
             bw.WriteInt(size);
             for (int i = 0; i < size; i++)
             {
                 UserSession session = sessions.get(i);
                 bw.WriteInt(session.m_userID);
                 bw.WriteString(session.m_key);
                 bw.WriteString(session.m_value);
             }
             byte[] bytes = bw.GetBytes();
             message.m_bodyLength = bytes.length;
             message.m_body = bytes;
             bw.Close();
         }
         catch(Exception ex)
         {

         }
         int ret = Send(message);
         return ret;
    }

    /** 
     更新会话

     @param message 消息
     @return 状态
    */
    public final int UpdateSessions(CMessage message)
    {
        ArrayList<UserSession> sessions = new ArrayList<UserSession>();
        GetSessions(sessions, message.m_body, message.m_bodyLength);
        int sessionsSize = sessions.size();
        java.util.ArrayList<UserSession> updateSessions = new java.util.ArrayList<UserSession>();
        for (int i = 0; i < sessionsSize; i++)
        {
            UserSession receive = sessions.get(i);
            UpdateSession(receive);
            updateSessions.add(receive);
        }
        int ret = Send(message, updateSessions);
        sessions.clear();
        updateSessions.clear();
        return ret;
    }

    /** 
     更新会话

     @param session 会话
    */
    public final void UpdateSession(UserSession session)
    {
        GetCollection().update(new BasicDBObject().append("userID", session.m_userID), UserSession2DBObject(session));
    }
    
    public static BasicDBObject UserSession2DBObject(UserSession userSession)
    {
        BasicDBObject object = new BasicDBObject();
        object.put("userID", userSession.m_userID);
        object.put("key", userSession.m_key);
        object.put("value", userSession.m_value);
        return object;
    }
}
