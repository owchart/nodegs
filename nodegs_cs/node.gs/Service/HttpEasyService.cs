/*******************************************************************************************\
*                                                                                           *
* HttpGetService.cs -  Http get service functions, types, and definitions.                  *
*                                                                                           *
*               Version 1.00  ����                                                        *
*                                                                                           *
*               Copyright (c) 2016-2016, Piratecat. All rights reserved.                    *
*               Created by Lord 2016/10/17.                                                  *
*                                                                                           *
********************************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using OwLibSV;
using System.IO.Compression;
using OwLib;

namespace node.gs
{
    /// <summary>
    /// HTTP��GET����
    /// </summary>
    public interface HttpEasyService
    {
        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="data">��������</param>
        /// <returns>����</returns>
        int OnReceive(HttpData data);
    }
}
