/*****************************************************************************\
*                                                                             *
* FileTransferService.cs -  FileTransfer service functions, types, and definitions*
*                                                                             *
*               Version 1.00 ★★★★★                                       *
*                                                                             *
*               Copyright (c) 2016-2017, Client. All rights reserved.         *
*               Created by Joel.                                              *
*                                                                             *
*******************************************************************************/
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using OwLibCT;
using System.Threading;
using System.Net;

namespace node.gs
{
    /// <summary>
    /// 文件传输监听器
    /// </summary>
    public interface FileTransferListener
    {
        /// <summary>
        /// 文件传输回调
        /// </summary>
        /// <param name="id">ID</param>
        /// <param name="totalLength">总长度</param>
        /// <param name="currentLength">当前长度</param>
        /// <param name="errorID">错误号</param>
        /// <param name="errorMsg">错误信息</param>
        /// <returns>状态</returns>
        int OnFileTransfer(int id, long totalLength, long currentLength, int errorID, String errorMsg);
    }

    /// <summary>
    /// 文件传输类
    /// </summary>
    public class FileTransferService
    {
        /// <summary>
        /// 文件传输服务
        /// </summary>
        public FileTransferService()
        {
            m_listeners = new Dictionary<int, FileTransferListener>();
            m_runningWork = new Dictionary<int, String>();
        }

        /// <summary>
        /// 正在运行线程的监听器
        /// </summary>
        public Dictionary<int, FileTransferListener> m_listeners;

        /// <summary>
        /// 任务id,每次新建任务这个值就会自增1
        /// </summary>
        private int m_maxID = 0;

        /// <summary>
        /// 正在运行中的上传线程
        /// </summary>
        public Dictionary<int, String> m_runningWork;

        /// <summary>
        /// 多线程文件上传方法
        /// </summary>
        /// <param name="param">呼叫方法的参数,其实际值为{任务id,上传url}</param>
        public void MultithreadUploadWork(object param)
        {
            object[] obj = (object[])param;
            int id = (int)obj[0];
            String url = (String)obj[1];
            String filePath = m_runningWork[id];
            FileTransferListener listener = m_listeners[id];
            HttpWebRequest request = null;
            Stream reader = null;
            HttpWebResponse response = null;
            FileStream fs = null;
            try
            {
                fs = new FileStream(filePath, FileMode.Open);
                request = (HttpWebRequest)WebRequest.Create(url);
                request.Method = "POST";
                request.ContentType = "application/octet-stream";
                byte[] buffer = new byte[10240];
                long currentLength = 0;
                long totalLength = fs.Length;
                request.ContentLength = totalLength;
                Stream writer = request.GetRequestStream();
                while (true)
                {
                    int len = fs.Read(buffer, 0, 10240);
                    writer.Write(buffer, 0, len);
                    if (listener != null)
                    {
                        listener.OnFileTransfer(id, totalLength, currentLength, 0, null);
                    }
                    currentLength += len;
                    if (len <= 0)
                    {
                        break;
                    }
                }
                writer.Close();
                response = (HttpWebResponse)request.GetResponse();
                reader = response.GetResponseStream();
                long contentLength = response.ContentLength;
                byte[] recvDatas = new byte[contentLength];
                reader.Read(recvDatas, 0, (int)contentLength);
                if (listener != null)
                {
                    listener.OnFileTransfer(id, totalLength, currentLength, 0, null);
                }
            }
            catch (Exception ex)
            {
                if (listener != null)
                {
                    listener.OnFileTransfer(id, 0, 0, 3, ex.Message);
                }
            }
            finally
            {
                if (fs != null)
                {
                    fs.Dispose();
                }
                if (response != null)
                {
                    response.Close();
                }
                if (reader != null)
                {
                    reader.Close();
                }
            }
        }

        /// <summary>
        /// 指定远程地址和本地文件绝对路径将文件传输到远程计算机
        /// </summary>
        /// <param name="filePath">本地文件的绝对路径</param>
        /// <param name="url">远程地址</param>
        /// <param name="listener">本次上传任务的监听器</param>
        /// <returns>返回0表示任务启动成功</returns>
        public int Upload(String filePath, String url, FileTransferListener listener)
        {
            int id = m_maxID++;
            m_runningWork.Add(id, filePath);
            m_listeners.Add(id, listener);
            Thread thread = new Thread(new ParameterizedThreadStart(MultithreadUploadWork));
            thread.Name = "" + id;
            object[] obj = { id, url };
            thread.Start(obj);
            return 0;
        }
    }
}
