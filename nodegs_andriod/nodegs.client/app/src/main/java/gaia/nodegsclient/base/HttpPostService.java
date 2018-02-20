package gaia.nodegsclient.base;

//*************************************************************************************\
//*                                                                                      *
//* HttpService.cs -  HttpService service functions, types, and definitions.     *
//*                                                                                      *
//*               Version 1.00 �                                                      *
//*                                                                                      *
//*               Copyright (c) 2016-2016, Server. All rights reserved.                  *
//*               Created by TangWen.                                                    *
//*                                                                                      *
//**************************************************************************************

import java.io.*;
import java.net.*;
import java.util.*;

/*
HTTP流服务
*/
public class HttpPostService extends BaseService
{
    private class HttpUnSyncPoster implements Runnable
    {
        byte[] m_data;
        String m_url;
        int requestID;
        public void run()
        {
            byte[] bytes = PostA(m_url,m_data);
            if(bytes != null)
            {
                CMessage ms = new CMessage();
                ms.m_body = bytes;
                AddUnSyncDataPackage(requestID,ms);
            }
        }
    }
    /*
       创建服务
    */
    public HttpPostService()
    {

    }

    public static ArrayList<CMessage> m_messages = new ArrayList<>();

    private boolean m_isSyncSend;

    public boolean GetIsSyncSend(){
        return m_isSyncSend;
    }
    public void SetIsSyncSend(boolean isSyncSend){
        m_isSyncSend = isSyncSend;
    }

    private int m_timeout = 10;
    public int GetTimeout() {
        return m_timeout;
    }

    public void SetTimeout(int timeout) {
        m_timeout = timeout;
    }

    private String m_url;

    public String GetUrl()
    {
        return m_url;
    }
    public void SetUrl(String value)
    {
        m_url = value;
    }

    private HashMap<Integer,CMessage> m_unSyncDataPackage = new HashMap<Integer,CMessage>();

    private void AddUnSyncDataPackage(int requestID, CMessage ms)
    {
        if(ms.m_body != null)
        {
            synchronized (m_unSyncDataPackage)
            {
                m_unSyncDataPackage.put(requestID, ms);
            }
        }
    }
    /*
	 * 发送POST数据
	 */
    public String Post(String url)
    {
        return Post(url, "");
    }



    /*
	* 发送POST数据
	*/
    public String Post(String url, String data)
    {
        byte[] sendDatas = data.getBytes();
        byte[] bytes = PostA(url, sendDatas);
        if(bytes != null)
        {
            return bytes.toString();
        }
        else
        {
            return null;
        }
    }

    /*
     * 发送POST数据
     */
    public byte[] Post(String url, byte[] sendDatas)
    {
        return UnSyncPost(url,sendDatas);
    }


    private byte[] PostA(String url, byte[] sendDatas) {
        try {
            URL realUrl = new URL(url);
            URLConnection conn = realUrl.openConnection();
            conn.setConnectTimeout(m_timeout);
            conn.setRequestProperty("accept", "text/html");
            conn.setRequestProperty("connection", "Keep-Alive");
            conn.setRequestProperty("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
            conn.setDoOutput(true);
            conn.setDoInput(true);
            if(sendDatas != null)
            {
                DataOutputStream writer = new DataOutputStream(conn.getOutputStream());
                writer.write(sendDatas);
                writer.flush();
                writer.close();
            }
            DataInputStream reader = new DataInputStream(conn.getInputStream());
            Binary br = new Binary();
            while (true) {
                int data = reader.read();
                if (data != -1) {
                    br.WriteByte((byte) data);
                } else {
                    break;
                }
            }
            byte[] dataArray = br.GetBytes();
            br.Close();
            return dataArray;
        } catch (Exception ex)
        {
            return null;
        }
    }

    /**
     * 释放等待者
     * @param requestID
     */
    public void ReleaseWaiter(int requestID)
    {
        synchronized (m_unSyncDataPackage)
        {
            if (m_unSyncDataPackage.containsKey(requestID))
            {
                m_unSyncDataPackage.remove(requestID);
            }
        }
    }

    @Override
    public int Send(CMessage message)
    {
        synchronized(m_messages)
        {
            m_messages.add(message);
        }

        if(!m_isSyncSend){
            new Thread(new Runnable() {
                @Override
                public void run() {
                    CMessage message = null;
                    synchronized (m_messages) {
                        message = m_messages.get(0);
                    }
                    if (message == null) {
                        return;
                    }
                    SendRequest(message);
                }
            }).start();
            return 1;
        }else
        {
            return SendRequest(message);
        }
    }

    public int Send(String url,byte[] bytes,int requestID)
    {
        synchronized (m_unSyncDataPackage)
        {
            if(m_unSyncDataPackage.containsKey(requestID))
            {
                return 1;
            }
            HttpUnSyncPoster p = new HttpUnSyncPoster();
            p.m_data = bytes;
            p.m_url = url;
            p.requestID = requestID;
            new Thread(p).start();
        }
        return 0;
    }

    /**
     * 发送请求
     */
    public int SendRequest(CMessage message) {
        try {
            int ret = -1;
            Binary bw = new Binary();
            byte[] body = message.m_body;
            int bodyLength = message.m_bodyLength;
            int uncBodyLength = bodyLength;
            byte[] zipBody = null;
            if (message.m_compressType == COMPRESSTYPE_GZIP) {
                zipBody = CStrA.GZip(body);
                bodyLength = zipBody.length;
            }
            int len = 4 * 4 + bodyLength + 2 * 3 + 1 * 2;
            bw.WriteInt(len);
            bw.WriteShort((short) message.m_groupID);
            bw.WriteShort((short) message.m_serviceID);
            bw.WriteShort((short) message.m_functionID);
            bw.WriteInt(message.m_sessionID);
            bw.WriteInt(message.m_requestID);
            bw.WriteChar((char) message.m_state);
            bw.WriteChar((char) message.m_compressType);
            bw.WriteInt(uncBodyLength);
            if (message.m_compressType == COMPRESSTYPE_GZIP) {
                bw.WriteBytes(zipBody);
            } else {
                bw.WriteBytes(body);
            }
            byte[] bytes = bw.GetBytes();
            URL realUrl = new URL(m_url);
            URLConnection conn = realUrl.openConnection();
            conn.setRequestProperty("accept", "text/html");
            conn.setRequestProperty("connection", "Keep-Alive");
            conn.setRequestProperty("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
            conn.setDoOutput(true);
            conn.setDoInput(true);
            DataOutputStream writer = new DataOutputStream(conn.getOutputStream());
            writer.write(bytes);
            writer.flush();
            writer.close();
            DataInputStream reader = new DataInputStream(conn.getInputStream());
            Binary br = new Binary();
            while (true) {
                int data = reader.read();
                if (data != -1) {
                    br.WriteByte((byte) data);
                } else {
                    break;
                }
            }
            byte[] dataArray = br.GetBytes();
            ret = dataArray.length;
            m_upFlow += ret;
            BaseService.CallBack(message.m_socketID, 0, dataArray, ret);
            br.Close();
            bw.Close();
            reader.close();
            return ret;
        } catch (Exception ex) {
            return 0;
        }
    }

    /**
     * 异步发送POST
     * @param url
     * @param postData
     * @return
     */
    public byte[] UnSyncPost(String url, byte[] postData)
    {
        int requestID = BaseService.GetRequestID();
        if(Send(url,postData,requestID) == 0)
        {
            CMessage ret = new CMessage();
            if(WaitMessage(requestID,10000, ret) ==1)
            {
                return ret.m_body;
            }
        }
        return null;
    }

    /**
     * 等待消息
     * @param requestID
     * @param waitTime
     * @param ms
     * @return
     */
    public int WaitMessage(int requestID, long waitTime, CMessage ms)
    {
        int isContain = 0;
        try
        {
            while (waitTime > 10)
            {
                synchronized (m_unSyncDataPackage)
                {
                    if (m_unSyncDataPackage.containsKey(requestID))
                    {
                        CMessage s;
                        if ((s = m_unSyncDataPackage.get(requestID))!= null)
                        {
                            if(s.m_body != null)
                            {
                                ms.m_body = s.m_body;
                                s = null;
                            }
                            isContain = 1;
                            break;
                        }
                    } 
                }
                waitTime -= 10;
                Thread.sleep(10);
            }
            ReleaseWaiter(requestID);
        } catch (Exception ex)
        {
            isContain = 9;
        }
        return isContain;
    }
}
