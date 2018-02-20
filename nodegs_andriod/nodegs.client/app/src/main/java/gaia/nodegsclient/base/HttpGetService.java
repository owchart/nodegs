package gaia.nodegsclient.base;

//*************************************************************************************\
//*                                                                                      *
//* HttpGetService.cs -  Http get service functions, types, and definitions.             *
//*                                                                                      *
//*               Version 1.00 �                                                      *
//*                                                                                      *
//*               Copyright (c) 2016-2016, Server. All rights reserved.                  *
//*               Created by Lord.                                                    *
//*                                                                                      *
//**************************************************************************************

import java.io.*;
import java.net.*;
import java.util.HashMap;

import gaia.owchart.owchart_android.Base.RefObject;

public class HttpGetService
{
    /**
     * 异步Http方法
     */
    public class HttpGetSender implements Runnable
    {
        String m_rurl;
        int m_sessionKey;
        public void run()
        {
            AddResponseToPackage(m_sessionKey,null);
            String response = Get(m_rurl);
            if(response != null)
            {
                AddResponseToPackage(m_sessionKey, response);
            }
        }
    }
    /*
        创建服务
    */
    public HttpGetService()
    {

    }

    //Http响应数据
    private final HashMap<Integer,String> m_httpsGetResponseData = new HashMap<>();

    //设置值
    private void AddResponseToPackage(int sessionKey, String value)
    {
        synchronized (m_httpsGetResponseData)
        {
            m_httpsGetResponseData.put(sessionKey, value);
        }
    }

    /*
    请求HTTP数据
    */
    public int Get(int requestID, String url)
    {
        if(!m_httpsGetResponseData.containsKey(requestID))
        {
            if(url != null && (!url.trim().equals("")))
            {
                HttpGetSender s = new HttpGetSender();
                s.m_rurl = url;
                s.m_sessionKey = requestID;
                new Thread(s).start();

            }else
            {
                return 1;
            }
            return 0;
        }
        return 1;
    }

    private String Get(String url)
    {
        String result = "";
        BufferedReader reader = null;
        try
        {
            URL realUrl = new URL(url);
            HttpURLConnection connection = (HttpURLConnection)realUrl.openConnection();
            connection.setRequestMethod("GET");
            connection.setRequestProperty("Charset", "GB2312");
            connection.setRequestProperty("accept", "text/html");
            connection.setRequestProperty("connection", "Keep-Alive");
            connection.setRequestProperty("user-agent",
                    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
            connection.connect();
            if(connection.getResponseCode() == 200){
                reader = new BufferedReader(new InputStreamReader(
                        connection.getInputStream()));
                String line;
                while ((line = reader.readLine()) != null) {
                    result += line;
                }
            }
        } catch (Exception e)
        {
            return null;
        }
        // 使用finally块来关闭输入流
        finally
        {
            try
            {
                if (reader != null)
                {
                    reader.close();
                }
            } catch (Exception e2)
            {

            }
        }
        return result;
    }

    //发送http方法
    public String GET(String url)
    {
        int requestID = BaseService.GetRequestID();
        if(url != null && (!url.equals("")))
        {
            Get(requestID, url);
            RefObject<String> ref = new RefObject<>(null);
            if(WaitMessage(requestID, 10000, ref) == 1)
            {
                return ref.argvalue;
            }
        }
        return null;
    }

    /**
     * 释放等待者
     * @param requestID
     */
    public void ReleaseWaiter(int requestID)
    {
        synchronized (m_httpsGetResponseData)
        {
            if (m_httpsGetResponseData.containsKey(requestID))
            {
                m_httpsGetResponseData.remove(requestID);
            }
        }
    }

    /**
     * 等待消息
     * @param requestId
     * @param waitTime
     * @return
     */
    public int WaitMessage(int requestId, long waitTime, RefObject<String> ref)
    {
        int isContain = 0;
        try
        {
            while (waitTime > 10)
            {
                synchronized (m_httpsGetResponseData)
                {
                    if (m_httpsGetResponseData.containsKey(requestId))
                    {
                        String ret;
                        if ((ret = m_httpsGetResponseData.get(requestId)) != null)
                        {
                            ref.argvalue = ret;
                            isContain = 1;
                            break;
                        }
                    }
                }
                waitTime -= 10;
                Thread.sleep(10);
            }
            ReleaseWaiter(requestId);
        } catch (Exception ex)
        {
            isContain = 9;
        }
        return isContain;
    }
}
