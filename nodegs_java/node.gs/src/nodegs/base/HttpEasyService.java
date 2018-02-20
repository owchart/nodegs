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
public interface HttpEasyService
{
        
	/*
            请求数据
	 */
	int OnReceive(HttpData data);
}
