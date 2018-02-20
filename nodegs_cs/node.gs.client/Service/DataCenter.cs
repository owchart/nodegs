/*****************************************************************************\
*                                                                             *
* DataCenter.cs -  Data center functions, types, and definitions.             *
*                                                                             *
*               Version 1.00  ����                                          *
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
using OwLibCT;
using node.gs.client;

namespace node.gs
{
    /// <summary>
    /// ������������
    /// </summary>
    public class DataCenter
    {
        #region Lord 2016/3/10
        private static bool m_isAppAlive = true;

        /// <summary>
        /// ��ȡ�����ó����Ƿ���
        /// </summary>
        public static bool IsAppAlive
        {
            get { return DataCenter.m_isAppAlive; }
            set { DataCenter.m_isAppAlive = value; }
        }

        private static EventService m_eventService;

        /// <summary>
        /// ��ȡEvent��GET����
        /// </summary>
        public static EventService EventService
        {
            get { return DataCenter.m_eventService; }
        }

        private static HttpGetService m_httpGetService;

        /// <summary>
        /// ��ȡHTTP��GET����
        /// </summary>
        public static HttpGetService HttpGetService
        {
            get { return DataCenter.m_httpGetService; }
        }

        private static HttpPostService m_httpPostService;

        /// <summary>
        /// ��ȡHTTP��������
        /// </summary>
        public static HttpPostService HttpPostService
        {
            get { return DataCenter.m_httpPostService; }
        }

        /// <summary>
        /// ��ȡ���ŷ���
        /// </summary>
        private static NewsService m_newsService;
        public static NewsService NewsService
        {
            get { return DataCenter.m_newsService; }
        }
        
        private static LoginService m_loginService;
        public static LoginService LoginService
        {
            get { return DataCenter.m_loginService; }
        }

        private static UserSessionService m_userSessionService;

        /// <summary>
        /// ��ȡ�û��Ự����
        /// </summary>
        public static UserSessionService UserSessionService
        {
            get { return DataCenter.m_userSessionService; }
        }

        private static UserSecurityService m_userSecurityService;

        /// <summary>
        /// ��ȡ�û���ѡ�ɷ���
        /// </summary>
        public static UserSecurityService UserSecurityService
        {
            get { return DataCenter.m_userSecurityService; }
        }

        /// <summary>
        /// ��ȡ����·��
        /// </summary>
        /// <returns>����·��</returns>
        public static String GetAppPath()
        {
            return Application.StartupPath;
        }

        /// <summary>
        /// ��������
        /// </summary>
        public static void StartService()
        {
            m_eventService = new EventService();
            m_httpGetService = new HttpGetService();
            m_httpPostService = new HttpPostService();
            m_loginService = new LoginService();
            m_newsService = new NewsService();
            m_userSessionService = new UserSessionService();
            m_userSecurityService = new UserSecurityService();
            BaseService.AddService(m_eventService);
            BaseService.AddService(m_httpPostService);
            BaseService.AddService(m_loginService);
            BaseService.AddService(m_newsService);
            BaseService.AddService(m_userSessionService);
            BaseService.AddService(m_userSecurityService);
        }
        #endregion
    }
}
