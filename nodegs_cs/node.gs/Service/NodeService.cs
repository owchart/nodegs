/*****************************************************************************\
*                                                                             *
* NodeService.cs -   Node service functions, types, and definitions.             *
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
using System.Net;
using System.IO;
using System.Web;
using System.Threading;
using OwLib;
using System.Net.Sockets;
using OwLibSV;

namespace node.gs
{
    /// <summary>
    /// Http数据
    /// </summary>
    public class HttpData
    {
        public bool m_close;
        public int m_contentLength;
        public String m_contentType = "";
        public byte[] m_body;
        public String m_method = "";
        public Dictionary<string, string> m_parameters = new Dictionary<string, string>();
        public byte[] m_resBytes;
        public String m_resStr;
        public String m_remoteIP;
        public int m_remotePort;
        public int m_socketID;
        public int m_statusCode = 200;
        public String m_url = "";
    }

    /// <summary>
    /// 文件Http服务
    /// </summary>
    public class HttpFileData
    {
        /// <summary>
        /// 流
        /// </summary>
        public byte[] m_buffer;

        /// <summary>
        /// 已收到长度
        /// </summary>
        public long m_recvLength;

        /// <summary>
        /// 总长度
        /// </summary>
        public long m_totalLength;
    }

    /// <summary>
    /// Node服务
    /// </summary>
    public class NodeService
    {
        /// <summary>
        /// 创建Node服务
        /// </summary>
        /// <param name="fileName">文件名</param>
        public NodeService(string fileName)
        {
            m_fileName = fileName;
        }

        /// <summary>
        /// 文件名称
        /// </summary>
        private String m_fileName;

        /// <summary>
        /// 脚本集合
        /// </summary>
        public Stack<CIndicator> m_indicators = new Stack<CIndicator>();

        /// <summary>
        /// 数据集合
        /// </summary>
        public Dictionary<int, HttpData> m_httpDatas = new Dictionary<int, HttpData>();

        private CIndicator m_indicator;

        /// <summary>
        /// 获取主脚本
        /// </summary>
        public CIndicator Indicator
        {
            get { return m_indicator; }
        }

        private Socket m_listener;

        /// <summary>
        /// 获取监听对象
        /// </summary>
        public Socket Listener
        {
            get { return m_listener; }
        }

        private int m_port = 8080;

        /// <summary>
        /// 获取或设置端口
        /// </summary>
        public int Port
        {
            get { return m_port; }
            set { m_port = value; }
        }

        private string m_script;

        /// <summary>
        /// 获取脚本
        /// </summary>
        public string Script
        {
            get { return m_script; }
        }

        private INativeBase m_native;

        /// <summary>
        /// 获取XML对象
        /// </summary>
        public INativeBase Native
        {
            get { return m_native; }
        }

        private bool m_useScript;

        /// <summary>
        /// 是否使用脚本
        /// </summary>
        public bool UseScript
        {
            get { return m_useScript; }
        }

        /// <summary>
        /// 检查脚本
        /// </summary>
        public void CheckScript()
        {
            String newScript = "";
            CFileA.Read(m_fileName, ref newScript);
            if (m_script != newScript)
            {
                Console.WriteLine("检测到脚本被修改...");
                m_script = newScript;
                lock (m_indicators)
                {
                    while (true)
                    {
                        try
                        {
                            CIndicator indicator = m_indicators.Pop();
                            indicator.Dispose();
                        }
                        catch
                        {
                            break;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 接受请求
        /// </summary>
        /// <param name="param">参数</param>
        private static void ReadData(object param)
        {
            Socket socket = (Socket)param;
            NodeService nodeService = DataCenter.NodeService;
            int newSocketID = (int)socket.Handle;
            try
            {
                byte[] buffer = new byte[102400];
                int len = socket.Receive(buffer);
                MemoryStream memoryStream = new MemoryStream(buffer);
                StreamReader reader = new StreamReader(memoryStream);
                HttpData data = new HttpData();
                data.m_remoteIP = ((IPEndPoint)socket.RemoteEndPoint).Address.ToString();
                data.m_remotePort = ((IPEndPoint)socket.RemoteEndPoint).Port;   
                String requestHeader;
                int contentLength = 0;
                string parameters = "";
                while ((requestHeader = reader.ReadLine()) != null && !String.IsNullOrEmpty(requestHeader))
                {
                    String lowerHeader = requestHeader.ToLower();
                    if (lowerHeader.IndexOf("get") == 0)
                    {
                        int end = lowerHeader.IndexOf("http/");
                        data.m_method = "GET";
                        parameters = requestHeader.Substring(5, end - 6);
                    }
                    else if (lowerHeader.IndexOf("post") == 0)
                    {
                        int end = lowerHeader.IndexOf("http/");
                        data.m_method = "POST";
                        parameters = requestHeader.Substring(5, end - 6);
                    }
                    else if (lowerHeader.IndexOf("accept: ") == 0)
                    {
                        try
                        {
                            data.m_contentType = requestHeader.Substring(8, requestHeader.IndexOf(',') - 8);
                        }
                        catch { }
                    }
                    else if (lowerHeader.IndexOf("content-type:") == 0)
                    {
                        data.m_contentType = requestHeader.Substring(14);
                    }
                    else if (lowerHeader.IndexOf("host:") == 0)
                    {
                        data.m_url = requestHeader.Substring(requestHeader.IndexOf(':') + 2);
                    }
                    else if (lowerHeader.IndexOf("content-length") == 0)
                    {
                        int begin = lowerHeader.IndexOf("content-length:") + "content-length:".Length;
                        String postParamterLength = requestHeader.Substring(begin).Trim();
                        contentLength = Convert.ToInt32(postParamterLength);
                    }
                }
                if (contentLength > 0)
                {
                    if (data.m_contentType != null && data.m_contentType == "application/octet-stream(test)")
                    {
                        HttpFileData fileHttpData = new HttpFileData();
                        int recvLength = 0;
                        while (recvLength < contentLength)
                        {
                            byte[] bytes = new byte[10240];
                            recvLength += socket.Receive(bytes);
                            fileHttpData.m_recvLength = recvLength;
                            fileHttpData.m_totalLength = contentLength;
                            fileHttpData.m_buffer = bytes;
                            FileTransferService.OnReceive(data, fileHttpData);
                        }
                    }
                    else
                    {
                        int idx = 0, ide = 0;
                        data.m_body = new byte[contentLength];
                        while (idx < contentLength)
                        {
                            int recvData = reader.Read();
                            if (recvData != -1)
                            {
                                if (recvData != 0)
                                {
                                    ide++;
                                }
                                idx++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        reader.Close();
                        memoryStream.Dispose();
                        if (ide == 0)
                        {
                            socket.Receive(data.m_body);
                        }
                        else
                        {
                            for (int i = 0; i < contentLength; i++)
                            {
                                data.m_body[i] = buffer[len - contentLength + i];
                            }
                        }
                    }
                    data.m_contentLength = contentLength;
                }
                else
                {
                    reader.Close();
                    memoryStream.Dispose();
                }
                if (data.m_method.Length == 0)
                {
                    return;
                }
                int cindex = parameters.IndexOf('?');
                if (cindex != -1)
                {
                    data.m_url = data.m_url + "/" + parameters;
                    parameters = parameters.Substring(cindex + 1);
                    String[] strs = parameters.Split(new string[] { "&" }, StringSplitOptions.RemoveEmptyEntries);
                    int strsSize = strs.Length;
                    for (int i = 0; i < strsSize; i++)
                    {
                        String[] subStrs = strs[i].Split(new string[] { "=" }, StringSplitOptions.RemoveEmptyEntries);
                        data.m_parameters[subStrs[0].ToLower()] = subStrs[1];
                    }
                }
                else
                {
                    data.m_url += "/" + parameters;
                }
                CIndicator indicator = null;
                if (nodeService.UseScript)
                {
                    try
                    {
                        lock (nodeService.m_indicators)
                        {
                            indicator = nodeService.m_indicators.Pop();
                        }
                    }
                    catch
                    {
                        indicator = CFunctionEx.CreateIndicator(nodeService.Script, nodeService.Native);
                    }
                    List<CFunction> functions = indicator.GetFunctions();
                    int functionsSize = functions.Count;
                    for (int i = 0; i < functionsSize; i++)
                    {
                        CFunctionHttp function = functions[i] as CFunctionHttp;
                        if (function != null)
                        {
                            function.m_data = data;
                        }
                    }
                }
                data.m_socketID = newSocketID;
                lock (nodeService.m_httpDatas)
                {
                    nodeService.m_httpDatas[newSocketID] = data;
                }
                if (indicator != null)
                {
                    indicator.CallFunction("ONHTTPREQUEST();");
                }
                if (data.m_close)
                {
                    return;
                }
                int resContentLength = 0;
                if (data.m_resBytes != null)
                {
                    resContentLength = data.m_resBytes.Length;
                }
                else
                {
                    if (data.m_resStr != null)
                    {
                        resContentLength = Encoding.Default.GetBytes(data.m_resStr).Length;
                    }
                }
                StringBuilder bld = new StringBuilder();
                bld.Append("HTTP/1.0 " + data.m_statusCode.ToString() + " OK\r\n");
                bld.Append(String.Format("Content-Length: {0}\r\n", resContentLength));
                bld.Append("Connection: close\r\n\r\n");
                if (data.m_resBytes != null)
                {
                    socket.Send(Encoding.Default.GetBytes(bld.ToString()));
                    socket.Send(data.m_resBytes);
                }
                else
                {
                    bld.Append(data.m_resStr);
                    socket.Send(Encoding.Default.GetBytes(bld.ToString()));
                }
                if (indicator != null)
                {
                    lock (nodeService.m_indicators)
                    {
                        nodeService.m_indicators.Push(indicator);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message + "\r\n" + ex.StackTrace);
            }
            finally
            {
                lock (nodeService.m_httpDatas)
                {
                    nodeService.m_httpDatas.Remove(newSocketID);
                }
                socket.Close();
            }
        }

        /// <summary>
        /// 启动监听
        /// </summary>
        public void Start()
        {
            m_useScript = CFileA.IsFileExist(m_fileName);
            if (m_useScript)
            {
                m_native = NativeHandler.CreateNative();
                CFileA.Read(m_fileName, ref m_script);
                m_indicator = CFunctionEx.CreateIndicator(m_script, m_native);
                Console.WriteLine(m_script);
            }
            try
            {
                //string host = "127.0.0.1";
                //IPAddress ip = IPAddress.Parse(host);
                if (m_indicator != null)
                {
                    m_indicator.CallFunction("ONHTTPSERVERSTARTING('" + m_fileName + "');");
                }
                IPEndPoint ipe = new IPEndPoint(IPAddress.Any, m_port);

                m_listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                m_listener.Bind(ipe);
                m_listener.Listen(0);
            }
            catch (Exception ex)
            {
                if (m_indicator != null)
                {
                    m_indicator.CallFunction("ONHTTPSERVERSTARTFAIL('" + ex.Message + "\r\n" + ex.StackTrace + "');");
                }
                return;
            }
            int minThreadNum = 0, portThreadNum = 0, maxThreadNum = 0;
            ThreadPool.GetMaxThreads(out maxThreadNum, out portThreadNum);
            ThreadPool.GetMinThreads(out minThreadNum, out portThreadNum);
            if (m_indicator != null)
            {
                m_indicator.CallFunction("ONHTTPSERVERSTART(" + CStr.ConvertIntToStr(maxThreadNum) + "," + CStr.ConvertIntToStr(minThreadNum) + ");");
            }
            while (DataCenter.IsAppAlive)
            {
                Socket socket = m_listener.Accept();
                ThreadPool.QueueUserWorkItem(new WaitCallback(ReadData), socket);
            }
            m_listener.Close();
        }
    }
}