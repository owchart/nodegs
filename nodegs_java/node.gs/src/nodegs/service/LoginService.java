/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.service;
import com.mongodb.*;
import gaia.owchart.owchart_android.Base.CStr;
import nodegs.base.*;
import java.util.*;
/**
 *
 * @author GAIA
 */
public class LoginService extends BaseService {
    public LoginService()
    {
        SetServiceID(SERVICEID_LOGIN);
        m_mongoService = new MongoService();
    }
    
    public static int SERVICEID_LOGIN = 5;
    
    /**
     * 用户登录的功能ID
     */
    public final int FUNCTIONID_LOGIN_LOGIN = 0;

    /**
     * 用户登出的功能ID
     */
    public final int FUNCTIONID_LOGIN_LOGOUT = 1;
        
    /**
     * 获取用户信息
     */
    public final int FUNCTIONID_LOGIN_GETLOGININFOS = 2;
    
    public final String MONGODB_LOGIN_INFO = "LoginInfo";

    private MongoService m_mongoService;
    
    /**
     * 用户登录
     * @param message 消息
     * @return 状态
     */
    public void AddLoginInfo(CMessage message)
    {
        LoginInfo receive = new LoginInfo();
        ArrayList<LoginInfo> loginInfos = new ArrayList<LoginInfo>();
        GetLoginInfos(loginInfos, message.m_body, message.m_bodyLength);
        receive = loginInfos.get(0);
        loginInfos.clear();
        GetLoginInfoByUserName(loginInfos, receive.m_userName);
        int loginInfosSize = loginInfos.size();
        if (loginInfosSize == 0)
        {
             //插入mongo
            GetCollection().insert(LoginInfo2DBObject(receive));
        }
    }
    
    public static LoginInfo DBObject2LoginInfo(DBObject DBObject)
    {
        @SuppressWarnings("unchecked")
        Map<String, Object> userMap = (Map<String, Object>) DBObject.toMap();

        LoginInfo loginInfo = new LoginInfo();
        loginInfo.m_userName = String.valueOf(userMap.get("userName"));
        loginInfo.m_userID = CStr.ConvertStrToInt(String.valueOf(userMap.get("userID")));
        loginInfo.m_type = CStr.ConvertStrToInt(String.valueOf(userMap.get("type")));
        loginInfo.m_state = CStr.ConvertStrToInt(String.valueOf(userMap.get("state")));
        loginInfo.m_sessionID = CStr.ConvertStrToInt(String.valueOf(userMap.get("sessionID")));
        loginInfo.m_passWord = String.valueOf(userMap.get("passWord"));
        loginInfo.m_nickName = String.valueOf(userMap.get("nickName"));
        loginInfo.m_maxUsers = CStr.ConvertStrToInt(String.valueOf(userMap.get("maxUsers")));
        return loginInfo;
    }
    
    public DBCollection GetCollection()
    {
        DBCollection collection = m_mongoService.GetCollectionByName(MONGODB_LOGIN_INFO);
        return collection;
    }
    
    /*
    根据用户名称获取登录信息
    */
    public int GetLoginInfoByUserName(List<LoginInfo> loginInfos, String userName)
    {
        BasicDBObject query =new BasicDBObject();
        query.put("userName", userName);
		
	DBCursor cursor = GetCollection().find(query);
        while(cursor.hasNext())
        {
            DBObject dbObject = cursor.next();
            LoginInfo loginInfo = DBObject2LoginInfo(dbObject);           
            loginInfos.add(loginInfo);
        }        
            
        return 1;
    }
    
    public int GetLoginInfos(CMessage message)
    {
        ArrayList<LoginInfo> loginInfos = new ArrayList<LoginInfo>();
        GetLoginInfos(loginInfos, message.m_body, message.m_bodyLength);
        String userName = loginInfos.get(0).m_userName;
        loginInfos.clear();
        GetLoginInfoByUserName(loginInfos, userName);
        Send(message, loginInfos);
        loginInfos.clear();
        return 1;
    }
    
    public int GetLoginInfos(ArrayList<LoginInfo> loginInfos, byte[] body, int bodyLength)
    {
        try {
            Binary br = new Binary();
            br.Write(body, bodyLength);
            int loginInfosSize = br.ReadInt();
            for (int i = 0; i < loginInfosSize; i++)
            {
                LoginInfo loginInfo = new LoginInfo();
                loginInfo.m_userID = br.ReadInt();
                loginInfo.m_userName = br.ReadString();
                loginInfo.m_passWord = br.ReadString();
                loginInfo.m_nickName = br.ReadString();
                loginInfo.m_type = br.ReadInt();
                loginInfo.m_state = br.ReadInt();
                loginInfo.m_sessionID = br.ReadInt();
                loginInfos.add(loginInfo);
            }
            br.Close();        
        } catch(Exception ex) {
            
        }        

        return 1;
    }
    
    public int Login(CMessage message)
    {
        LoginInfo receive = new LoginInfo();
        ArrayList<LoginInfo> loginInfos = new ArrayList<LoginInfo>();
        GetLoginInfos(loginInfos, message.m_body, message.m_bodyLength);
        receive = loginInfos.get(0);
        loginInfos.clear();
        GetLoginInfoByUserName(loginInfos, receive.m_userName);
        int loginInfosSize = loginInfos.size();
        if (loginInfosSize == 0)
        {
            AddLoginInfo(message);
            GetLoginInfoByUserName(loginInfos, receive.m_userName);
            loginInfosSize = loginInfos.size();
        }
        else if (loginInfosSize == 1)
        {
            LoginInfo info = loginInfos.get(0);
            if (info.m_passWord.equals(receive.m_passWord))
            {
                receive.m_userID = info.m_userID;
                receive.m_type = info.m_type;
                receive.m_state = info.m_state;
                receive.m_sessionID = info.m_sessionID;
                receive.m_nickName = info.m_nickName;
                receive.m_maxUsers = info.m_maxUsers;
            }
        }
        
        loginInfos.clear();
        receive.m_state = 1;
        loginInfos.add(receive);
        int ret = Send(message, loginInfos);
        loginInfos.clear();
        return ret;
    }
        
    public static BasicDBObject LoginInfo2DBObject(LoginInfo info)
    {
        BasicDBObject object = new BasicDBObject();
        object.put("userName", info.m_userName);
        object.put("userID", info.m_userID);
        object.put("type", info.m_type);
        object.put("state", info.m_state);
        object.put("sessionID", info.m_sessionID);
        object.put("passWord", info.m_passWord);
        object.put("nickName", info.m_nickName);
        object.put("maxUsers", info.m_maxUsers);
        
        return object;
    }

    /**
     * 用户登出
     * @param message 消息
     * @return 状态
     */

    public int Logout(CMessage message)
    {
        LoginInfo receive = new LoginInfo();
        ArrayList<LoginInfo> loginInfos = new ArrayList<LoginInfo>();
        GetLoginInfos(loginInfos, message.m_body, message.m_bodyLength);
        receive = loginInfos.get(0);
        
        loginInfos.clear();
        loginInfos.add(receive);
        int ret = Send(message, loginInfos);
        loginInfos.clear();
        return ret;
    }
        
    
    @Override
     public void OnReceive(CMessage message)
    {
        super.OnReceive(message);
        switch (message.m_functionID)
        {
            case FUNCTIONID_LOGIN_LOGIN:
                Login(message);
                break;
            case FUNCTIONID_LOGIN_LOGOUT:
                Logout(message);
                break;
            case FUNCTIONID_LOGIN_GETLOGININFOS:
                GetLoginInfos(message);
                break;
        }
    }

    public int Send(CMessage message, List<LoginInfo> loginInfos)
    {
        try
        {
            Binary bw = new Binary();
            int size = loginInfos.size();
            bw.WriteInt(size);
            for (int i = 0; i < size; i++)
            {
                LoginInfo loginInfo = loginInfos.get(i);
                bw.WriteInt(loginInfo.m_userID);
                bw.WriteString(loginInfo.m_userName);
                bw.WriteString(loginInfo.m_passWord);
                bw.WriteString(loginInfo.m_nickName);
                bw.WriteInt(loginInfo.m_type);
                bw.WriteInt(loginInfo.m_state);
                bw.WriteInt(loginInfo.m_sessionID);
            }
            byte[] bytes = bw.GetBytes();
            message.m_bodyLength = bytes.length;
            message.m_body = bytes;
            int ret = Send(message);
            bw.Close();
            return ret;
        }
        catch(Exception ex)
        {
        }            

        return 0;
    }
}
