using System;
using System.Collections.Generic;
using System.Text;
using node.gs;

namespace OwLibCT
{
    public class NewsData
    {
        /// <summary>
        /// 代码
        /// </summary>
        public String m_code = "";

        /// <summary>
        /// 公司类型
        /// </summary>
        public int m_companyType;

        /// <summary>
        /// 内容
        /// </summary>
        public String m_content = "";

        /// <summary>
        /// 时间
        /// </summary>
        public double m_time;

        /// <summary>
        /// 标题
        /// </summary>
        public String m_title = "";

        /// <summary>
        /// 类型
        /// </summary>
        public int m_type;

        /// <summary>
        /// 地址
        /// </summary>
        public String m_url;
    }

    public class NewsDataReq
    {
        /// <summary>
        /// 代码
        /// </summary>
        public String m_code = "";


        /// <summary>
        /// 时间
        /// </summary>
        public double m_time;

        /// <summary>
        /// 标题
        /// </summary>
        public String m_title = "";

        /// <summary>
        /// 类型
        /// </summary>
        public int m_type;

        /// <summary>
        /// 最大条数
        /// </summary>
        public int m_maxCount;
    }

    public class NewsService : HttpPostService
    {
        public NewsService()
        {
            ServiceID = SERVICEID_NEWS;
        }

        /// <summary>
        /// 新闻服务ID
        /// </summary>
        public const int SERVICEID_NEWS = 13;


        /// <summary>
        /// 获取新闻的方法ID
        /// </summary>
        public const int FUNCTIONID_NEWS_GETNEWSLIST = 0;

        private int m_socketID = 0;

        /// <summary>
        /// 获取或设置套接字ID
        /// </summary>
        public int SocketID
        {
            get { return m_socketID; }
            set { m_socketID = value; }
        }


        /// <summary>
        /// 获取新闻
        /// </summary>
        /// <param name="requestID">请求</param>
        /// <returns>状态</returns>
        public int GetNews(String code, double time, String title, int type, int maxCount, int requestID)
        {
            NewsDataReq req = new NewsDataReq();
            req.m_code = code;
            req.m_time = time;
            req.m_title = title;
            req.m_type = type;
            req.m_maxCount = maxCount;
            List<NewsDataReq> reqs = new List<NewsDataReq>();
            reqs.Add(req);
            int ret = Send(FUNCTIONID_NEWS_GETNEWSLIST, requestID, reqs);
            reqs.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// 通过流获取新闻
        /// </summary>
        /// <param name="loginInfos">登录信息</param>
        /// <param name="body">包体</param>
        /// <param name="bodyLength">包体长度</param>
        public static int GetNews(List<NewsData> newsDatas, byte[] body, int bodyLength)
        {
            Binary br = new Binary();
            br.Write(body, bodyLength);
            int size = br.ReadInt();
            for (int i = 0; i < size; i++)
            {
                NewsData news = new NewsData();
                news.m_code = br.ReadString();
                news.m_companyType = br.ReadInt();
                news.m_content = br.ReadString();
                news.m_time = br.ReadDouble();
                news.m_title = br.ReadString();
                news.m_type = br.ReadInt();
                news.m_url = br.ReadString();
                newsDatas.Add(news);
            }

            br.Close();
            return 1;
        }

        /// <summary>
        /// 接收数据
        /// </summary>
        /// <param name="message">消息</param>
        public override void OnReceive(CMessage message)
        {
            base.OnReceive(message);
            SendToListener(message);
        }

        /// <summary>
        /// 发送消息
        /// </summary>
        /// <param name="functionID">方法ID</param>
        /// <param name="requestID">请求ID</param>
        /// <param name="loginInfos">信息集合</param>
        /// <returns>状态</returns>
        public int Send(int functionID, int requestID, List<NewsDataReq> newsInfos)
        {
            Binary bw = new Binary();
            int size = newsInfos.Count;
            bw.WriteInt(size);
            for (int i = 0; i < size; i++)
            {
                NewsDataReq req = newsInfos[i];
                bw.WriteString(req.m_code);
                bw.WriteDouble(req.m_time);
                bw.WriteString(req.m_title);
                bw.WriteInt(req.m_type);
                bw.WriteInt(req.m_maxCount);
            }
            byte[] bytes = bw.GetBytes();
            int ret = Send(new CMessage(GroupID, ServiceID, functionID, SessionID, requestID, m_socketID, 0, CompressType, bytes.Length, bytes));
            bw.Close();
            return ret;
        }
    }
}
