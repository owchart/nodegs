/*******************************************************************************************\
*                                                                                           *
* CFunctionEx.cs -  Indicator functions, types, and definitions.                            *
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
using OwLib;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.Net;
using System.Web;
using System.IO;

namespace node.gs
{
    /// <summary>
    /// ��ʾ����
    /// </summary>
    public class CFunctionEx : CFunction
    {
        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="indicator">ָ��</param>
        /// <param name="id">ID</param>
        /// <param name="name">����</param>
        /// <param name="native">XML����</param>
        public CFunctionEx(CIndicator indicator, int id, String name, INativeBase native)
        {
            m_indicator = indicator;
            m_ID = id;
            m_name = name;
            m_native = native;
        }

        /// <summary>
        /// ָ��
        /// </summary>
        public CIndicator m_indicator;

        /// <summary>
        /// XML����
        /// </summary>
        public INativeBase m_native;

        /// <summary>
        /// ���з���
        /// </summary>
        private const String FUNCTIONS = "CREATETHREAD,ISAPPALIVE";

        /// <summary>
        /// ��ʼ����
        /// </summary>
        private const int STARTINDEX = 1000000;

        /// <summary>
        /// ����
        /// </summary>
        /// <param name="var">����</param>
        /// <returns>���</returns>
        public override double OnCalculate(CVariable var)
        {
            switch (var.m_functionID)
            {
                case STARTINDEX:
                    return CREATETHREAD(var);
                case STARTINDEX + 1:
                    return ISAPPALIVE(var);
                default:
                    return 0;
            }
        }

        /// <summary>
        /// ����ָ��
        /// </summary>
        /// <param name="native">������</param>
        /// <param name="script">�ű�</param>
        /// <param name="xml">XML</param>
        /// <returns>ָ��</returns>
        public static CIndicator CreateIndicator(String script, INativeBase native)
        {
            CIndicator indicator = native.CreateIndicator();
            CTable table = native.CreateTable();
            indicator.DataSource = table;
            CFunctionBase.AddFunctions(indicator);
            CFunctionHttp.AddFunctions(indicator);
            int index = STARTINDEX;
            string[] functions = FUNCTIONS.Split(new string[] { "," }, StringSplitOptions.RemoveEmptyEntries);
            int functionsSize = functions.Length;
            for (int i = 0; i < functionsSize; i++)
            {
                indicator.AddFunction(new CFunctionEx(indicator, index + i, functions[i], native));
            }
            indicator.Script = script;
            table.AddColumn(0);
            table.Set(0, 0, 0);
            indicator.OnCalculate(0);
            return indicator;
        }

        /// <summary>
        /// �����߳�
        /// </summary>
        /// <param name="param">����</param>
        private void CreateThread(object param)
        {
            CIndicator indicator = CreateIndicator(DataCenter.NodeService.Script, m_native);
            indicator.CallFunction(param.ToString());
            indicator.Dispose();
        }

        /// <summary>
        /// �����߳�
        /// </summary>
        /// <param name="var">����</param>
        /// <returns>״̬</returns>
        private double CREATETHREAD(CVariable var)
        {
            Thread thread = new Thread(new ParameterizedThreadStart(CreateThread));
            thread.Start(m_indicator.GetText(var.m_parameters[0]));
            return 0;
        }

        /// <summary>
        /// ��֤�����Ƿ���
        /// </summary>
        /// <param name="var">����</param>
        /// <returns>״̬</returns>
        private double ISAPPALIVE(CVariable var)
        {
            return DataCenter.IsAppAlive ? 1 : 0;
        }
    }
}
