/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gaia.nodegsclient.service;

import java.util.ArrayList;
import java.util.List;
import gaia.nodegsclient.base.*;

/**
 *
 * @author GAIA
 */
public class LoginService extends BaseService {
    public LoginService()
    {
        SetServiceID(SERVICEID_LOGIN);
    }
    
    public static int SERVICEID_LOGIN = 5;

    /// <summary>
    /// 用户登录的功能ID
    /// </summary>
    public static int FUNCTIONID_LOGIN_LOGIN = 0;

    /// <summary>
    /// 用户登出的功能ID
    /// </summary>
    public static int FUNCTIONID_LOGIN_LOGOUT = 1;

    public final int FUNCTIONID_LOGIN_GETLOGININFOS = 2;
    
    private int m_socketID = 0;

    public final int GetSocketID() { return m_socketID; }
    public final void SetSocketID(int socketID) { m_socketID = socketID;}

    public int GetLoginInfos(int requestID)
    {
        LoginInfo loginInfo = new LoginInfo();
        loginInfo.m_sessionID = GetSessionID();
        ArrayList<LoginInfo> loginInfos = new ArrayList<LoginInfo>();
        loginInfos.add(loginInfo);
        int ret = Send(FUNCTIONID_LOGIN_GETLOGININFOS, requestID, loginInfos);
        loginInfos.clear();
        return ret > 0 ? 1 : 0;
    }   
    
    public static int GetLoginInfos(ArrayList<LoginInfo> loginInfos, byte[] body, int bodyLength)
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


    public int Login(String userName, String passWord, int requestID)
    {
        LoginInfo loginInfo = new LoginInfo();
        loginInfo.m_userName = userName;
        loginInfo.m_passWord = passWord;
        ArrayList<LoginInfo> loginInfos = new ArrayList<LoginInfo>();
        loginInfos.add(loginInfo);
        int ret = Send(FUNCTIONID_LOGIN_LOGIN, requestID, loginInfos);
        loginInfos.clear();
        return ret > 0 ? 1 : 0;
    }

    public int Logout(int requestID)
    {
        LoginInfo loginInfo = new LoginInfo();
        loginInfo.m_sessionID = GetSessionID();
        ArrayList<LoginInfo> loginInfos = new ArrayList<LoginInfo>();
        loginInfos.add(loginInfo);
        int ret = Send(FUNCTIONID_LOGIN_LOGOUT, requestID, loginInfos);
        loginInfos.clear();
        return ret > 0 ? 1 : 0;
    }


    @Override
     public void OnReceive(CMessage message)
    {
        super.OnReceive(message);
        SendToListener(message);
    }
     

    public int Send(int functionID, int requestID, List<LoginInfo> loginInfos)
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
            int ret = Send(new CMessage(GetGroupID(), GetServiceID(), functionID, GetSessionID(), requestID, GetSocketID(), 0, GetCompressType(), bytes.length, bytes));
            bw.Close();
            return ret;
        }
        catch(Exception ex)
        {

        }            

        return 0;
    }
}
