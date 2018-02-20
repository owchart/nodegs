/*****************************************************************************\
*                                                                             *
* DataCenter.cs -  Data center functions, types, and definitions.             *
*                                                                             *
*               Version 1.00  ★★★                                          *
*                                                                             *
*               Copyright (c) 2016-2016, OwPlan. All rights reserved.      *
*               Created by Lord 2016/3/10.                                    *
*                                                                             *
*******************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using OwLib;
using System.IO;
using System.Net;
using System.Threading;
using System.Xml;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Reflection;
using System.Windows;
using System.Windows.Forms;
using OwLibSV;

namespace node.gs
{
    /// <summary>
    /// 处理行情数据
    /// </summary>
    public class DataCenter
    {
        #region Lord 2016/3/10

        private static EventService m_eventService;

        /// <summary>
        /// 获取Event的GET服务
        /// </summary>
        public static EventService EventService
        {
            get { return DataCenter.m_eventService; }
        }

        private static bool m_isAppAlive = true;

        /// <summary>
        /// 获取或设置程序是否存活
        /// </summary>
        public static bool IsAppAlive
        {
            get { return DataCenter.m_isAppAlive; }
            set { DataCenter.m_isAppAlive = value; }
        }

        private static Dictionary<String, HttpEasyService> m_httpEasyServices = new Dictionary<string, HttpEasyService>();

        /// <summary>
        /// 获取或设置HTTP的简单服务
        /// </summary>
        public static Dictionary<String, HttpEasyService> HttpEasyServices
        {
            get { return DataCenter.m_httpEasyServices; }
            set { DataCenter.m_httpEasyServices = value; }
        }

        private static HttpHardService m_httpHardService;

        /// <summary>
        /// 获取或设置HTTP的困难服务
        /// </summary>
        public static HttpHardService HttpHardService
        {
            get { return DataCenter.m_httpHardService; }
        }

        private static NodeService m_nodeService;

        /// <summary>
        /// 获取或设置Node服务
        /// </summary>
        public static NodeService NodeService
        {
            get { return DataCenter.m_nodeService; }
        }

        /// <summary>
        /// 获取程序路径
        /// </summary>
        /// <returns>程序路径</returns>
        public static String GetAppPath()
        {
            return Application.StartupPath;
        }

        /// <summary>
        /// 启动服务
        /// </summary>
        /// <param name="fileName">文件名</param>
        public static void StartService(string fileName)
        {
            m_httpEasyServices["easy"] = new HttpEasyServiceEx();
            m_httpHardService = new HttpHardService();
            BaseService.AddService(m_httpHardService);
            int socketID = BaseService.StartServer(0, 8084);
            m_eventService = new EventService();
            m_eventService.SocketID = socketID;
            //m_userSessionService.SocketID = socketID;
            BaseService.AddService(m_eventService);
            m_nodeService = new NodeService(fileName);
            m_nodeService.Start();
        }
        #endregion
    }
}
