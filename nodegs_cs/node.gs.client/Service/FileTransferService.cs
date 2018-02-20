/*****************************************************************************\
*                                                                             *
* FileTransferService.cs -  FileTransfer service functions, types, and definitions*
*                                                                             *
*               Version 1.00 ������                                       *
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
    /// �ļ����������
    /// </summary>
    public interface FileTransferListener
    {
        /// <summary>
        /// �ļ�����ص�
        /// </summary>
        /// <param name="id">ID</param>
        /// <param name="totalLength">�ܳ���</param>
        /// <param name="currentLength">��ǰ����</param>
        /// <param name="errorID">�����</param>
        /// <param name="errorMsg">������Ϣ</param>
        /// <returns>״̬</returns>
        int OnFileTransfer(int id, long totalLength, long currentLength, int errorID, String errorMsg);
    }

    /// <summary>
    /// �ļ�������
    /// </summary>
    public class FileTransferService
    {
        /// <summary>
        /// �ļ��������
        /// </summary>
        public FileTransferService()
        {
            m_listeners = new Dictionary<int, FileTransferListener>();
            m_runningWork = new Dictionary<int, String>();
        }

        /// <summary>
        /// ���������̵߳ļ�����
        /// </summary>
        public Dictionary<int, FileTransferListener> m_listeners;

        /// <summary>
        /// ����id,ÿ���½��������ֵ�ͻ�����1
        /// </summary>
        private int m_maxID = 0;

        /// <summary>
        /// ���������е��ϴ��߳�
        /// </summary>
        public Dictionary<int, String> m_runningWork;

        /// <summary>
        /// ���߳��ļ��ϴ�����
        /// </summary>
        /// <param name="param">���з����Ĳ���,��ʵ��ֵΪ{����id,�ϴ�url}</param>
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
        /// ָ��Զ�̵�ַ�ͱ����ļ�����·�����ļ����䵽Զ�̼����
        /// </summary>
        /// <param name="filePath">�����ļ��ľ���·��</param>
        /// <param name="url">Զ�̵�ַ</param>
        /// <param name="listener">�����ϴ�����ļ�����</param>
        /// <returns>����0��ʾ���������ɹ�</returns>
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
