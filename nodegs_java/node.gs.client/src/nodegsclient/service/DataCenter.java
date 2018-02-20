package nodegsclient.service;

//****************************************************************************\

import nodegsclient.base.BaseService;
import nodegsclient.base.EventService;
import nodegsclient.base.HttpGetService;
import nodegsclient.base.HttpPostService;
import nodegsclient.base.UserSessionService;

//*                                                                             *
//* DataCenter.java -  Data center functions, types, and definitions.             *
//*                                                                             *
//*               Version 1.00  ★★                                       *
//*                                                                             *
//*               Copyright (c) 2016-2016, OwPlan. All rights reserved.      *
//*               Created by Lord 2016/3/10.                                    *
//*                                                                             *
//******************************************************************************


/** 
 处理行情数据
 
*/
public class DataCenter
{
    private static EventService m_eventService;
    
    public static EventService GetEventService()
    {
        return m_eventService;
    }    
    private static HttpGetService m_httpGetService;

    /*
    获取HTTP的GET服务
    */
    public static HttpGetService GetHttpGetService()
    {
        return m_httpGetService;
    }

    private static HttpPostService m_httpPostService;

    /*
    获取HTTP的POST服务
    */
    public static HttpPostService GetHttpPostService()
    {
            return m_httpPostService;
    }

    private static LoginService m_loginService;
    public static LoginService GetLoginService()
    {
        return m_loginService;
    }

    private static boolean m_isAppAlive = true;

    /** 
     获取或设置程序是否存在

    */
    public static boolean IsAppAlive()
    {
            return DataCenter.m_isAppAlive;
    }
    public static void SetAppAlive(boolean value)
    {
            DataCenter.m_isAppAlive = value;
    }

    private static UserSessionService m_userSessionService;

    /**
     * 获取用户会话服务
     */
    public static UserSessionService GetUserSessionService()
    {
        return m_userSessionService;
    }

    /** 
     获取程序路径

     @return 程序路径
    */
    public static String GetAppPath()
    {
        return System.getProperty("user.dir");
    }

    /** 
     启动服务

     @param fileName 文件�
    */
    public static void StartService()
    {
        m_eventService = new EventService();
        m_httpGetService = new HttpGetService();
        m_httpPostService = new HttpPostService();
        m_loginService = new LoginService();
        m_userSessionService = new UserSessionService();
        BaseService.AddService(m_eventService);
        BaseService.AddService(m_httpPostService);
        BaseService.AddService(m_loginService);
        BaseService.AddService(m_userSessionService);
    }
}
