/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.service;

import com.mongodb.BasicDBObject;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import gaia.owchart.owchart_android.Base.CStr;
import gaia.owchart.owchart_android.Base.RefObject;
import java.util.ArrayList;
import java.util.Map;
import nodegs.base.Binary;
import nodegs.base.CFileA;
import nodegs.base.CMessage;
import nodegs.base.HttpHardService;

/**
 *
 * @author GAIA
 */
public class SecurityService extends HttpHardService{
 
    /// <summary>
     /// 创建证券服务
    /// </summary>
    public SecurityService()
    {
        SetServiceID(SERVICEID_SECURITY);
        m_mongoService = new MongoService();
    }

    /**
     * 证券服务ID
     */
    public final int SERVICEID_SECURITY = 3;

    /**
     * 获取证券信息方法ID
     */
    public final int FUNCTIONID_SECURITY_GETSECURITIES = 0; 
    
    /**
     * 键盘精灵数据库名称
     */
    public final String MONGODB_SECURITY_INFO = "Security";

    /**
     * Mongo服务
     */
    private MongoService m_mongoService;    
    
    /**
     * MongoDB数据转换成Security
     * @param DBObject
     * @return 
     */
    public static Security DBObjectToSecurity(DBObject DBObject)
    {
        @SuppressWarnings("unchecked")
        Map<String, Object> securityMap = (Map<String, Object>) DBObject.toMap();

        Security security = new Security();
        security.m_code = String.valueOf(securityMap.get("code"));
        security.m_name = String.valueOf(securityMap.get("name"));
        security.m_pingyin = String.valueOf(securityMap.get("pingyin"));
        security.m_status = CStr.ConvertStrToInt(String.valueOf(securityMap.get("state")));
        security.m_type = CStr.ConvertStrToInt(String.valueOf(securityMap.get("type")));
        return security;
    }
    
    /**
     * 获取数据库Collection
     * @return 
     */
    public DBCollection GetCollection()
    {
        DBCollection collection = m_mongoService.GetCollectionByName(MONGODB_SECURITY_INFO);
        return collection;
    }    
    
    /**
     * 获取股票信息
     * @param securities 股票信息
     * @param filter 过滤条件
     * @return 状态
     */
    public int GetSecurities(ArrayList<Security> securities, String filter)
    {      
        BasicDBObject query =new BasicDBObject();		
	DBCursor cursor = GetCollection().find(query);
        while(cursor.hasNext())
        {
            DBObject dbObject = cursor.next();
            Security security = DBObjectToSecurity(dbObject);           
            securities.add(security);
        }
        return 1;
    }

    /**
     * 获取所有股票信息
     * @param message 消息
     * @return 状态
     */
    public int GetSecurities(CMessage message)
    {
        try
        {
            Binary br = new Binary();
            br.Write(message.m_body, message.m_bodyLength);
            ArrayList<Security> securites = new ArrayList<Security>();
            br.Close();
            String filter = "";           
            GetSecurities(securites, filter);
            return Send(message, securites);            
        }
        catch(Exception ex)
        {
            return 0;
        }        
    }

    /**
     * 导入股票
     * @param path 路径
     */
    public void ImportSecurities(String path)
    {
        if (CFileA.IsFileExist(path))
        {            
            String content  = "";
            RefObject<String> ref = new RefObject<String>(content);
            CFileA.Read(path, ref);
            content = ref.argvalue;
            String[] strs = content.split("\n");
        }
    }

    /**
     * 接收数据
     * @param message 消息
     */
    @Override
    public void OnReceive(CMessage message)
    {
        super.OnReceive(message);
        switch (message.m_functionID)
        {
            case FUNCTIONID_SECURITY_GETSECURITIES:
                GetSecurities(message);
                break;
        }
    }

    /**
     * 发送类别
     * @param message 消息
     * @param securities 股票列表
     * @return 
     */
    public int Send(CMessage message, ArrayList<Security> securities)
    {
        try
        {
            Binary bw = new Binary();
            int size = securities.size();
            bw.WriteInt(size);
            for (int i = 0; i < size; i++)
            {
                Security security = securities.get(i);
                bw.WriteString(security.m_code);
                if (message.m_functionID == FUNCTIONID_SECURITY_GETSECURITIES)
                {
                    bw.WriteString(security.m_name);
                    bw.WriteString(security.m_pingyin);
                    bw.WriteShort((short)security.m_type);
                    bw.WriteChar((char)0);
                }
            }
            byte[] bytes = bw.GetBytes();
            message.m_bodyLength = bytes.length;
            message.m_body = bytes;
            int ret = Send(message);
            bw.Close();
            return ret;
        }
        catch(Exception ex)
        {
            return 0;
        }        
    }
}
