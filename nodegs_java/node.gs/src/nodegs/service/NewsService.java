/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.service;

import java.util.ArrayList;
import nodegs.base.BaseService;
import nodegs.base.Binary;
import nodegs.base.CFileA;
import nodegs.base.CMessage;
import nodegs.base.CStrA;
import com.mongodb.*;
import owchart.owlib.Base.CMathLib;
import owchart.owlib.Base.CStr;
import java.util.Map;
import static nodegs.service.LoginService.DBObject2LoginInfo;
import nodegs.base.HttpHardService;
/**
 *
 * @author GAIA
 */
public class NewsService extends HttpHardService{
    
    /// <summary>
    /// 创建股票过滤服务
    /// </summary>
    public NewsService()
    {        
        SetServiceID(SERVICEID_NEWS);
        m_mongoService = new MongoService();
    }
    
    /**
     * 新闻服务ID
     */
    public final int SERVICEID_NEWS = 13;

    /**
     * 获取新闻的方法ID
     */
    public final int FUNCTIONID_NEWS_GETNEWSLIST = 0;
    
    public final String MONGODB_NEWS = "news";

    private MongoService m_mongoService;

    public static NewsData DBObject2NewsData(DBObject DBObject)
    {
        @SuppressWarnings("unchecked")
        Map<String, Object> userMap = (Map<String, Object>) DBObject.toMap();

        NewsData newsData = new NewsData();
        newsData.m_code = String.valueOf(userMap.get("m_code"));
//        newsData.m_companyType;
        newsData.m_content = String.valueOf(userMap.get("m_content"));
        
        String sDTTime = String.valueOf(userMap.get("m_time"));
        newsData.m_time = CStrA.ConvertDateTimeToDouble(sDTTime);
        newsData.m_title = String.valueOf(userMap.get("m_title"));
        newsData.m_type = CStr.ConvertStrToInt(String.valueOf(userMap.get("m_type")));
        newsData.m_url = String.valueOf(userMap.get("m_url"));        
        return newsData;
    }
    
    public DBCollection GetCollection()
    {
        DBCollection collection = m_mongoService.GetCollectionByName(MONGODB_NEWS);
        return collection;
    }
        
    /**
     * 获取新闻
     * @param code 代码
     * @param companyType 公司类型
     * @param maxSize 最大数量
     * @param newsDatas 新闻数据
     * @return 状态
     */
    public int GetNews(String code, int type, int maxSize, ArrayList<NewsData> newsDatas)
    {
        BasicDBObject query =new BasicDBObject();
        query.put("m_code", code);
        query.put("m_type", type);
		
        int count = 0;
	DBCursor cursor = GetCollection().find(query);
        while(cursor.hasNext() && count < maxSize)
        {
            DBObject dbObject = cursor.next();
            NewsData loginInfo = DBObject2NewsData(dbObject);           
            newsDatas.add(loginInfo);
            count++;
        }  

        return 1;
    }

    /**
     * 获取新闻
     * @param message 消息
     * @return  状态
     */
    public int GetNews(CMessage message)
    {
        ArrayList<NewsData> newsDatas = new ArrayList<>();
        ArrayList<NewsDataReq> reqs = new ArrayList<NewsDataReq>();
        GetNewsReq(reqs, message.m_body, message.m_bodyLength);
        for (int i = 0; i < reqs.size(); i++)
        {
            NewsDataReq req = reqs.get(0);
            GetNews(req.m_code, req.m_type, req.m_maxCount, newsDatas);          
        }
        
        Send(message, newsDatas);
        newsDatas.clear();
        return 1;
    }
        
    public int GetNewsReq(ArrayList<NewsDataReq> newsDatas, byte[] body, int bodyLength)
    {
        try
        {
            Binary br = new Binary();
            br.Write(body, bodyLength);
            int newsSize = br.ReadInt();
            for (int i = 0; i < newsSize; i++)
            {
                NewsDataReq newsData = new NewsDataReq();
                newsData.m_code = br.ReadString();
                newsData.m_time = br.ReadDouble();
                newsData.m_title = br.ReadString();
                newsData.m_type = br.ReadInt();
                newsData.m_maxCount = br.ReadInt();
                newsDatas.add(newsData);
            }
            br.Close();        
        }
        catch(Exception ex)
        {
            
        }        
        return 1;
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
            case FUNCTIONID_NEWS_GETNEWSLIST:
                GetNews(message);
                break;
        }
    }
    
    /**
     * 发送消息
     * @param message 消息
     * @param datas 数据
     * @return  状态
     */
    public int Send(CMessage message, ArrayList<NewsData> datas)
    {
        try
        {
            Binary bw = new Binary();
            int size = datas.size();
            bw.WriteInt(size);
            for (int i = 0; i < size; i++)
            {
                NewsData data = datas.get(i);
                bw.WriteString(data.m_code);
                bw.WriteInt(data.m_companyType);
                bw.WriteString(data.m_content);
                bw.WriteDouble((double)data.m_time);
                bw.WriteString(data.m_title);
                bw.WriteInt(data.m_type);
                bw.WriteString(data.m_url);
            }
            byte[] bytes = bw.GetBytes();
            message.m_body = bytes;
            message.m_bodyLength = bytes.length;
            int ret = Send(message);
            bw.Close();
            return ret;
        }
        catch(Exception ex)
        {
            return 1;
        }        
    }
}
