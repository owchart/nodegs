package nodegs.base;

//*************************************************************************************\
//*                                                                                      *
//* HttpService.cs -  HttpService service functions, types, and definitions.     *
//*                                                                                      *
//*               Version 1.00 �                                                      *
//*                                                                                      *
//*               Copyright (c) 2016-2016, Server. All rights reserved.                  *
//*               Created by Lord.                                                    *
//*                                                                                      *
//**************************************************************************************

import nodegs.service.DataCenter;
import nodegs.base.*;
import java.util.*;
import owchart.owlib.Base.*;
import owchart.owlib.Grid.*;

/*
HTTP流服务
*/
public class HttpHardService extends BaseService
{
    /*
        创建服务
    */
    public HttpHardService()
    {
            SetServiceID(SERVICEID_HTTPPOST);
    }

    public static final int SERVICEID_HTTPPOST = 20;

    public static final int FUNCTIONID_HTTPPOST_TEST = 0;

    @Override
    public void OnReceive(CMessage message)
    {
        super.OnReceive(message);
        SendToListener(message);
    }

    @Override
    public int Send(CMessage message)
    {
        int ret = -1;
        try
        {
            Binary bw = new Binary();
            byte[] body = message.m_body;
            int bodyLength = message.m_bodyLength;
            int uncBodyLength = bodyLength;
            byte[] zipBody = null;
            if (message.m_compressType == COMPRESSTYPE_GZIP)
            {
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
            if (message.m_compressType == COMPRESSTYPE_GZIP)
            {
                bw.WriteBytes(zipBody);
            }else
            {
                bw.WriteBytes(body);
            }
            byte[] bytes = bw.GetBytes();
            synchronized (DataCenter.GetNodeService().m_httpDatas)
            {
                DataCenter.GetNodeService().m_httpDatas.get(message.m_socketID).m_resBytes = bytes;
            }
            ret = bytes.length;
            bw.Close();
        }
        catch (Exception ex)
        {

        }
        return ret;
    }
}
