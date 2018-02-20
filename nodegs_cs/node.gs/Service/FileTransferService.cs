/*******************************************************************************************\
*                                                                                           *
* FileTransferService.cs -  File transfer service functions, types, and definitions.                  *
*                                                                                           *
*               Version 1.00  ★★★                                                        *
*                                                                                           *
*               Copyright (c) 2016-2016, Piratecat. All rights reserved.                    *
*               Created by Lord 2016/10/17.                                                  *
*                                                                                           *
********************************************************************************************/


using System;
using System.Collections.Generic;
using System.Text;
using OwLibSV;
using System.IO;

namespace node.gs
{
    /// <summary>
    /// 文件传输类
    /// </summary>
    public class FileTransferService
    {
        /// <summary>
        /// 从HTTP数据读取信息,然后让子类处理
        /// </summary>
        /// <param name="data">数据</param>
        /// <returns>状态</returns>
        public static int OnReceive(HttpData data, HttpFileData fileData)
        {
            FileStream fs = new FileStream(@"C:\Miao\a.zip", FileMode.Append);
            fs.Write(fileData.m_buffer, 0, fileData.m_buffer.Length);
            fs.Close();
            return 0;
        }
    }
}
