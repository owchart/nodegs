/*******************************************************************************************\
*                                                                                           *
* CFunctionBase.cs -  Base functions, types, and definitions.                            *
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
using System.Threading;

namespace OwLib
{
    /// <summary>
    /// 界面相关的库
    /// </summary>
    public class CFunctionBase : CFunction
    {
        /// <summary>
        /// 创建方法
        /// </summary>
        /// <param name="indicator">指标</param>
        /// <param name="id">ID</param>
        /// <param name="name">名称</param>
        public CFunctionBase(CIndicator indicator, int id, String name)
        {
            m_indicator = indicator;
            m_ID = id;
            m_name = name;
        }

        /// <summary>
        /// 指标
        /// </summary>
        public CIndicator m_indicator;

        /// <summary>
        /// 方法
        /// </summary>
        private static string FUNCTIONS = "IN,OUT,SLEEP,TEST";

        /// <summary>
        /// 前缀
        /// </summary>
        private static string PREFIX = "";

        /// <summary>
        /// 开始索引
        /// </summary>
        private const int STARTINDEX = 1000;

        /// <summary>
        /// 计算
        /// </summary>
        /// <param name="var">变量</param>
        /// <returns>结果</returns>
        public override double OnCalculate(CVariable var)
        {
            switch (var.m_functionID)
            {
                case STARTINDEX:
                    return IN(var);
                case STARTINDEX + 1:
                    return OUT(var);
                case STARTINDEX + 2:
                    return SLEEP(var);
                case STARTINDEX + 3:
                    {
                        double value = m_indicator.GetValue(var.m_parameters[0]);
                        return 0;
                    }
                default: return 0;
            }
        }

        /// <summary>
        /// 添加方法
        /// </summary>
        /// <param name="indicator">方法库</param>
        /// <returns>指标</returns>
        public static void AddFunctions(CIndicator indicator)
        {
            string[] functions = FUNCTIONS.Split(new string[] { "," }, StringSplitOptions.RemoveEmptyEntries);
            int functionsSize = functions.Length;
            for (int i = 0; i < functionsSize; i++)
            {
                indicator.AddFunction(new CFunctionBase(indicator, STARTINDEX + i, PREFIX + functions[i]));
            }
        }

        /// <summary>
        /// 输入函数
        /// </summary>
        /// <param name="var">变量</param>
        /// <returns>状态</returns>
        private double IN(CVariable var)
        {
            CVariable newVar = new CVariable(m_indicator);
            newVar.m_expression = "'" + Console.ReadLine() + "'";
            m_indicator.SetVariable(var.m_parameters[0], newVar);
            return 0;
        }

        /// <summary>
        /// 输出函数
        /// </summary>
        /// <param name="var">变量</param>
        /// <returns>状态</returns>
        private double OUT(CVariable var)
        {
            int len = var.m_parameters.Length;
            for (int i = 0; i < len; i++)
            {
                string text = m_indicator.GetText(var.m_parameters[i]);
                Console.Write(text);
            }
            Console.WriteLine("");
            return 0;
        }

        /// <summary>
        /// 睡眠
        /// </summary>
        /// <param name="var">变量</param>
        /// <returns>状态</returns>
        private double SLEEP(CVariable var)
        {
            Thread.Sleep((int)m_indicator.GetValue(var.m_parameters[0]));
            return 1;
        }
    }
}
