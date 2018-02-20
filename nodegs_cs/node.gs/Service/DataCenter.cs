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
using OwLibSV;

namespace node.gs
{
    /// <summary>
    /// ������������
    /// </summary>
    public class DataCenter
    {
        #region Lord 2016/3/10

        private static EventService m_eventService;

        /// <summary>
        /// ��ȡEvent��GET����
        /// </summary>
        public static EventService EventService
        {
            get { return DataCenter.m_eventService; }
        }

        private static bool m_isAppAlive = true;

        /// <summary>
        /// ��ȡ�����ó����Ƿ���
        /// </summary>
        public static bool IsAppAlive
        {
            get { return DataCenter.m_isAppAlive; }
            set { DataCenter.m_isAppAlive = value; }
        }

        private static Dictionary<String, HttpEasyService> m_httpEasyServices = new Dictionary<string, HttpEasyService>();

        /// <summary>
        /// ��ȡ������HTTP�ļ򵥷���
        /// </summary>
        public static Dictionary<String, HttpEasyService> HttpEasyServices
        {
            get { return DataCenter.m_httpEasyServices; }
            set { DataCenter.m_httpEasyServices = value; }
        }

        private static HttpHardService m_httpHardService;

        /// <summary>
        /// ��ȡ������HTTP�����ѷ���
        /// </summary>
        public static HttpHardService HttpHardService
        {
            get { return DataCenter.m_httpHardService; }
        }

        private static NodeService m_nodeService;

        /// <summary>
        /// ��ȡ������Node����
        /// </summary>
        public static NodeService NodeService
        {
            get { return DataCenter.m_nodeService; }
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
        /// <param name="fileName">�ļ���</param>
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
