package nodegs.service;
import nodegs.base.*;
import java.util.HashMap;
import nodegs.base.BaseService;
//****************************************************************************\
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
        private static HashMap<String, HttpEasyService> m_httpEasyServices = new HashMap<String, HttpEasyService>();
        
        /*
        获取HTTP的GET服务
        */
        public static HashMap<String, HttpEasyService> GetHttpGetServices()
        {
            return m_httpEasyServices;
        }
    
	private static HttpHardService m_httpHardService;

        /*
        获取HTTP的POST服务
        */
	public static HttpHardService GetHttpHardService()
	{
		return m_httpHardService;
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
        
        private static LoginService m_loginService;
        public static LoginService GetLoginService()
        {
            return m_loginService;
        }
        
        private static NewsService m_newsService;
        public static NewsService GetNewsService()
        {
            return m_newsService;
        }

	private static NodeService m_nodeService;

	/** 
	 获取或设置Node服务
	 
	*/
	public static NodeService GetNodeService()
	{
		return DataCenter.m_nodeService;
	}
        
        private static UserSessionService m_userSessionService;
        
        /**
         * 获取用户会话服务
         */
        public static UserSessionService GetUserSessionService()
        {
            return m_userSessionService;
        }

        private static UserSecurityService m_userSecurityService;

        /// <summary>
        /// 获取用户自选股服务
        /// </summary>
        public static UserSecurityService GetUserSecurityService()
        {
            return m_userSecurityService;
        }
        
        private static SecurityService m_securityService;
        public static SecurityService GetSecurityService()
        {
            return m_securityService;
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
	 
	 @param fileName 文件�?
	*/
	public static void StartService(String fileName)
	{
            m_httpEasyServices.put("easy", new HttpEasyServiceEx());
            int socketID = BaseService.StartServer(8081);
            m_eventService = new EventService();              
            m_httpHardService = new HttpHardService();
            m_loginService = new LoginService();
            m_loginService.SetSocketID(socketID);
            m_newsService = new NewsService();
            m_userSessionService = new UserSessionService();
            m_eventService.SetSocketID(socketID);
            m_userSessionService.SetSocketID(socketID);
            m_userSecurityService = new UserSecurityService();
            m_userSecurityService.SetSocketID(socketID);
            m_securityService = new SecurityService();
            m_securityService.SetSocketID(socketID);
            BaseService.AddService(m_eventService);
            BaseService.AddService(m_httpHardService);
            BaseService.AddService(m_userSessionService);
            BaseService.AddService(m_loginService);
            BaseService.AddService(m_userSecurityService);
            BaseService.AddService(m_newsService);
            BaseService.AddService(m_securityService);
            m_nodeService = new NodeService(fileName);
            try
            {
                m_nodeService.Start();
            }
            catch(Exception ex)
            {
            }
	}
}
