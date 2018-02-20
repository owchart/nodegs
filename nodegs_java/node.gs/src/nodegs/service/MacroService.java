/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.service;

import java.util.ArrayList;
import nodegs.base.BaseService;
import nodegs.base.Binary;
import nodegs.base.CMessage;

/**
 *
 * @author GAIA
 */
public class MacroService extends BaseService{
    /// <summary>
    /// 创建股票过滤服务
    /// </summary>
    public MacroService()
    {        
        SetServiceID(SERVICEID_MACRO);
    }

    /// <summary>
    /// 锁
    /// </summary>
    private Object m_lock = new Object();
    
    /// <summary>
    /// 宏定义服务ID
    /// </summary>
    public final int SERVICEID_MACRO = 9;

    /// <summary>
    /// 添加宏方法ID
    /// </summary>
    public final int FUNCTIONID_MACRO_ADDMACROS = 0;

    /// <summary>
    /// 删除宏方法ID
    /// </summary>
    public final int FUNCTIONID_MACRO_DELETEMACROS = 1;

    /// <summary>
    /// 执行宏方法ID
    /// </summary>
    public final int FUNCTIONID_MACRO_EXECUTEMACROS = 2;

    /// <summary>
    /// 获取所有宏方法ID
    /// </summary>
    public final int FUNCTIONID_MACRO_GETMACROS = 3;

    /// <summary>
    /// 更新宏方法ID
    /// </summary>
    public final int FUNCTIONID_MACRO_UPDATEMACROS = 4;

    /// <summary>
    /// 更新宏排序方法ID
    /// </summary>
    public final int FUNCTIONID_MACRO_UPDATEORDERNUM = 5;

    /**
     * 添加宏
     * @param message 消息
     * @return  状态
     */
    public int AddMacros(CMessage message)
    {
        ArrayList<Macro> macros = new ArrayList<Macro>();
        GetMacros(macros, message.m_body, message.m_bodyLength);
        int macrosSize = macros.size();
        ArrayList<Macro> addMacros = new ArrayList<Macro>();
        for (int i = 0; i < macrosSize; i++)
        {
            Macro receive = macros.get(i);
            addMacros.add(receive);
        }
        int ret = Send(message, addMacros, true);
        macros.clear();
        addMacros.clear();
        return ret;
    }

    /// <summary>
    /// 删除宏
    /// </summary>
    /// <param name="message">消息</param>
    /// <returns>状态</returns>
    /**
     * 删除宏
     * @param message 消息
     * @return  状态
     */
    public int DeleteMacros(CMessage message)
    {
        ArrayList<Macro> macros = new ArrayList<Macro>();
        GetMacros(macros, message.m_body, message.m_bodyLength);
        int macrosSize = macros.size();
        ArrayList<Macro> deleteMacros = new ArrayList<Macro>();
        for (int i = 0; i < macrosSize; i++)
        {
            Macro receive = macros.get(i);
            deleteMacros.add(receive);
        }
        int ret = Send(message, deleteMacros, true);
        macros.clear();
        deleteMacros.clear();
        return ret;
    }

    /// <summary>
    /// 执行宏
    /// </summary>
    /// <param name="message">消息</param>
    /// <returns>状态</returns>
    /**
     * 执行宏
     * @param message 消息
     * @return  状态
     */
    public int ExecuteMacros(CMessage message)
    {
        ArrayList<Macro> macros = new ArrayList<Macro>();
        GetMacros(macros, message.m_body, message.m_bodyLength);
        int ret = Send(message, macros, true);
        macros.clear();
        return ret;
    }

    /// <summary>
    /// 获取宏信息
    /// </summary>
    /// <param name="templates">宏信息</param>
    /// <param name="body">包体</param>
    /// <param name="bodyLength">包体长度</param>
    /// <returns>状态</returns>
    /**
     * 获取宏信息
     * @param macros 宏信息
     * @param body 包体
     * @param bodyLength 包体长度
     * @return  状态
     */
    public int GetMacros(ArrayList<Macro> macros, byte[] body, int bodyLength)
    {
        try
        { 
            Binary br = new Binary();
            br.Write(body, bodyLength);
            int templatesSize = br.ReadInt();
            for (int i = 0; i < templatesSize; i++)
            {
                Macro macro = new Macro();
                macros.add(macro);
            }
            br.Close();
            return 1;
        }
        catch(Exception ex)
        {
            return 0;
        }       
    }

    /**
     * 获取宏信息
     * @param macros 宏信息
     * @param userID 用户ID
     * @param macroID 模板ID
     * @return  状态
     */
    public int GetMacros(ArrayList<Macro> macros, int userID, String macroID)
    {        
        Macro macro = new Macro();
        macro.m_script = "";
        macros.add(macro);
        return 1;
    }

    /**
     * 获取宏信息
     * @param message 消息
     * @return 状态
     */
    public int GetMacros(CMessage message)
    {
        Macro receive = new Macro();
        ArrayList<Macro> macros = new ArrayList<Macro>();
        GetMacros(macros, message.m_body, message.m_bodyLength);
        receive = macros.get(0);
        macros.clear();
        int ret = Send(message, macros, false);
        macros.clear();
        return ret;
    }
    
    /**
     * 接收数据
     * @param message  消息
     */
    @Override
    public void OnReceive(CMessage message)
    {
        super.OnReceive(message);
        switch (message.m_functionID)
        {
            case FUNCTIONID_MACRO_ADDMACROS:
                AddMacros(message);
                break;
            case FUNCTIONID_MACRO_DELETEMACROS:
                DeleteMacros(message);
                break;
            case FUNCTIONID_MACRO_EXECUTEMACROS:
                ExecuteMacros(message);
                break;
            case FUNCTIONID_MACRO_GETMACROS:
                GetMacros(message);
                break;
            case FUNCTIONID_MACRO_UPDATEMACROS:
                UpdateMacros(message);
                break;
            case FUNCTIONID_MACRO_UPDATEORDERNUM:
                UpdateOrderNum(message);
                break;
        }
    }
    
    /**
     * 发送宏
     * @param message 消息
     * @param macros 宏列表
     * @param broadCast 是否广播
     * @return 状态
     */
    public int Send(CMessage message, ArrayList<Macro> macros, Boolean broadCast)
    {
        try
        {
            Binary bw = new Binary();
            int size = macros.size();
            bw.WriteInt(size);
            for (int i = 0; i < size; i++)
            {
                Macro macro = macros.get(i);
                bw.WriteString(macro.m_script);
            }
            byte[] bytes = bw.GetBytes();
            message.m_bodyLength = bytes.length;
            message.m_body = bytes;
            int userID = 0;            
        }
        catch(Exception ex)
        {
        }
        return 1;
    }

    /**
     * 更新宏
     * @param message 消息
     * @return  状态
     */
    public int UpdateMacros(CMessage message)
    {
        ArrayList<Macro> macros = new ArrayList<Macro>();
        GetMacros(macros, message.m_body, message.m_bodyLength);
        int macrosSize = macros.size();
        ArrayList<Macro> updateMacros = new ArrayList<Macro>();
        for (int i = 0; i < macrosSize; i++)
        {
            Macro receive = macros.get(i);
            UpdateMacro(receive);
            updateMacros.add(receive);
        }
        int ret = Send(message, updateMacros, true);
        macros.clear();
        updateMacros.clear();
        return ret;
    }
    
    /**
     * 更新宏
     * @param macro 宏信息
     */
    public void UpdateMacro(Macro macro)
    {
    }
    
    /**
     * 更新排序字段
     * @param message 消息
     * @return 状态
     */
    public int UpdateOrderNum(CMessage message)
    {
        ArrayList<Macro> macros = new ArrayList<Macro>();
        GetMacros(macros, message.m_body, message.m_bodyLength);
        int macrosSize = macros.size();
        ArrayList<Macro> updateMacros = new ArrayList<Macro>();
        for (int i = 0; i < macrosSize; i++)
        {
            Macro receive = macros.get(i);
            updateMacros.add(receive);
        }
        int ret = Send(message, updateMacros, true);
        macros.clear();
        updateMacros.clear();
        return ret;
    }
}
