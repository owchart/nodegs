package nodegs.base;

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

import nodegs.base.*;
import java.util.*;

/*
HTTP的GET服务
*/
public class HttpEasyServiceEx implements HttpEasyService
{        
	/*
            请求数据
	 */
	public int OnReceive(HttpData data)
	{
            try
            {
                Binary binary = new Binary();
                binary.WriteString("Mr.AAAA");
                binary.WriteString("13821212121212");
                data.m_resBytes = binary.GetBytes();
            }catch(Exception ex)
            {
            }
		return -1;
	}
}
