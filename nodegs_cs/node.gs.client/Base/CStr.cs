/*****************************************************************************\
*                                                                             *
* Str.cs -    Str functions, types, and definitions                           *
*                                                                             *
*               Version 1.00 ★★★★★                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Client. All rights reserved.         *
*               Created by Lord.                                              *
*                                                                             *
*******************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace OwLibCT
{
    public class CStrA
    {
        #region Lord 2016/1/20
        /// <summary>
        /// 获取证券的文件名称
        /// </summary>
        /// <param name="code">代码</param>
        /// <returns>文件名称</returns>
        public static String ConvertDBCodeToFileName(String code)
        {
            String fileName = code;
            if (fileName.IndexOf(".") != -1)
            {
                fileName = fileName.Substring(fileName.IndexOf('.') + 1) + fileName.Substring(0, fileName.IndexOf('.'));
            }
            fileName += ".txt";
            return fileName;
        }

        /// <summary>
        /// 字符串转换为浮点型
        /// </summary>
        /// <param name="str">字符串</param>
        /// <returns>数值</returns>
        public static double ConvertStrToDouble(String str)
        {
            double value = 0;
            double.TryParse(str, out value);
            return value;
        }

        /// <summary>
        /// 字符串转换为整形
        /// </summary>
        /// <param name="str">字符串</param>
        /// <returns>数值</returns>
        public static int ConvertStrToInt(String str)
        {
            int value = 0;
            int.TryParse(str, out value);
            return value;
        }

        /// <summary>
        /// 获取数据库转义字符串
        /// </summary>
        /// <param name="str">字符串</param>
        /// <returns>转义字符串</returns>
        public static String GetDBString(String str)
        {
            return str.Replace("'", "''");
        }
        #endregion
    }
}
