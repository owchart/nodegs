/*****************************************************************************\
*                                                                             *
* Str.cs -    Str functions, types, and definitions                           *
*                                                                             *
*               Version 1.00 ������                                       *
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
        /// ��ȡ֤ȯ���ļ�����
        /// </summary>
        /// <param name="code">����</param>
        /// <returns>�ļ�����</returns>
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
        /// �ַ���ת��Ϊ������
        /// </summary>
        /// <param name="str">�ַ���</param>
        /// <returns>��ֵ</returns>
        public static double ConvertStrToDouble(String str)
        {
            double value = 0;
            double.TryParse(str, out value);
            return value;
        }

        /// <summary>
        /// �ַ���ת��Ϊ����
        /// </summary>
        /// <param name="str">�ַ���</param>
        /// <returns>��ֵ</returns>
        public static int ConvertStrToInt(String str)
        {
            int value = 0;
            int.TryParse(str, out value);
            return value;
        }

        /// <summary>
        /// ��ȡ���ݿ�ת���ַ���
        /// </summary>
        /// <param name="str">�ַ���</param>
        /// <returns>ת���ַ���</returns>
        public static String GetDBString(String str)
        {
            return str.Replace("'", "''");
        }
        #endregion
    }
}
