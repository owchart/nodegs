using System;
using System.Collections.Generic;
using System.Text;
using node.gs;

namespace OwLibCT
{
    public class NewsData
    {
        /// <summary>
        /// ����
        /// </summary>
        public String m_code = "";

        /// <summary>
        /// ��˾����
        /// </summary>
        public int m_companyType;

        /// <summary>
        /// ����
        /// </summary>
        public String m_content = "";

        /// <summary>
        /// ʱ��
        /// </summary>
        public double m_time;

        /// <summary>
        /// ����
        /// </summary>
        public String m_title = "";

        /// <summary>
        /// ����
        /// </summary>
        public int m_type;

        /// <summary>
        /// ��ַ
        /// </summary>
        public String m_url;
    }

    public class NewsDataReq
    {
        /// <summary>
        /// ����
        /// </summary>
        public String m_code = "";


        /// <summary>
        /// ʱ��
        /// </summary>
        public double m_time;

        /// <summary>
        /// ����
        /// </summary>
        public String m_title = "";

        /// <summary>
        /// ����
        /// </summary>
        public int m_type;

        /// <summary>
        /// �������
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
        /// ���ŷ���ID
        /// </summary>
        public const int SERVICEID_NEWS = 13;


        /// <summary>
        /// ��ȡ���ŵķ���ID
        /// </summary>
        public const int FUNCTIONID_NEWS_GETNEWSLIST = 0;

        private int m_socketID = 0;

        /// <summary>
        /// ��ȡ�������׽���ID
        /// </summary>
        public int SocketID
        {
            get { return m_socketID; }
            set { m_socketID = value; }
        }


        /// <summary>
        /// ��ȡ����
        /// </summary>
        /// <param name="requestID">����</param>
        /// <returns>״̬</returns>
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
        /// ͨ������ȡ����
        /// </summary>
        /// <param name="loginInfos">��¼��Ϣ</param>
        /// <param name="body">����</param>
        /// <param name="bodyLength">���峤��</param>
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
        /// ��������
        /// </summary>
        /// <param name="message">��Ϣ</param>
        public override void OnReceive(CMessage message)
        {
            base.OnReceive(message);
            SendToListener(message);
        }

        /// <summary>
        /// ������Ϣ
        /// </summary>
        /// <param name="functionID">����ID</param>
        /// <param name="requestID">����ID</param>
        /// <param name="loginInfos">��Ϣ����</param>
        /// <returns>״̬</returns>
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
