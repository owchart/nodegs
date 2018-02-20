using System;
using System.Collections.Generic;
using System.Text;
using node.gs;

namespace OwLibSV
{
    /// <summary>
    /// 自选股类别
    /// </summary>
    public class UserSecurityCategory
    {
        #region Lord 2016/12/22
        /// <summary>
        /// 类别ID
        /// </summary>
        public String m_categoryID = "";

        /// <summary>
        /// 股票代码
        /// </summary>
        public String m_codes = "";

        /// <summary>
        /// 类别名称
        /// </summary>
        public String m_name = "";

        /// <summary>
        /// 列表顺序
        /// </summary>
        public int m_orderNum;

        /// <summary>
        /// 类别
        /// </summary>
        public int m_type;

        /// <summary>
        /// 用户ID
        /// </summary>
        public int m_userID;
        #endregion
    }

    /// <summary>
    /// 自选股服务
    /// </summary>
    public class UserSecurityService : HttpHardService
    {
        #region Lord 2016/12/19
        /// <summary>
        /// 创建自选股服务
        /// </summary>
        public UserSecurityService()
        {
            ServiceID = SERVICEID_USERSECURITY;
        }

        /// <summary>
        /// 自选股服务ID
        /// </summary>
        public const int SERVICEID_USERSECURITY = 1;

        /// <summary>
        /// 添加类别方法ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_ADDCATEGORIES = 0;

        /// <summary>
        /// 删除类别方法ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_DELETECATEGORIES = 1;

        /// <summary>
        /// 获取所有类别信息方法ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_GETCATEGORIES = 2;

        /// <summary>
        /// 更新类别信息方法ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_UPDATECATEGORIES = 3;

        /// <summary>
        /// 添加证券方法ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_ADDSECURITIES = 4;

        /// <summary>
        /// 删除证券方法ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_DELETESECURITIES = 5;


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
        /// 添加自选股类别
        /// </summary>
        /// <param name="requestID">请求ID</param>
        /// <param name="category">类别</param>
        /// <returns>状态</returns>
        public int AddCategory(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_ADDCATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// 添加证券
        /// </summary>
        /// <param name="requestID">请求ID</param>
        /// <param name="category">类别</param>
        /// <returns>状态</returns>
        public int AddSecurities(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_ADDSECURITIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// 删除类别
        /// </summary>
        /// <param name="requestID">请求ID</param>
        /// <param name="category">类别</param>
        /// <returns>状态</returns>
        public int DeleteCategory(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_DELETECATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// 删除证券
        /// </summary>
        /// <param name="requestID">请求ID</param>
        /// <param name="category">类别</param>
        /// <returns>状态</returns>
        public int DeleteSecurities(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_DELETESECURITIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// 获取类别信息
        /// </summary>
        /// <param name="requestID">请求ID</param>
        /// <param name="category">类别</param>
        /// <returns>状态</returns>
        public int GetCategories(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_GETCATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// 通过流获取类别信息列表
        /// </summary>
        /// <param name="categories">类别信息</param>
        /// <param name="body">包体</param>
        /// <param name="bodyLength">包体长度</param>
        /// <returns>状态</returns>
        public static int GetCategories(List<UserSecurityCategory> categories, byte[] body, int bodyLength)
        {
            Binary br = new Binary();
            br.Write(body, bodyLength);
            int size = br.ReadInt();
            if (size > 0)
            {
                for (int i = 0; i < size; i++)
                {
                    UserSecurityCategory category = new UserSecurityCategory();
                    category.m_categoryID = br.ReadString();
                    category.m_userID = br.ReadInt();
                    category.m_name = br.ReadString();
                    category.m_codes = br.ReadString();
                    category.m_type = br.ReadInt();
                    category.m_orderNum = br.ReadInt();
                    categories.Add(category);
                }
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
        /// <param name="categories">类别集合</param>
        /// <returns>状态</returns>
        public int Send(int functionID, int requestID, List<UserSecurityCategory> categories)
        {
            Binary bw = new Binary();
            int categoriesSize = categories.Count;
            bw.WriteInt(categoriesSize);
            for (int i = 0; i < categoriesSize; i++)
            {
                UserSecurityCategory category = categories[i];
                bw.WriteString(category.m_categoryID);
                bw.WriteInt(category.m_userID);
                bw.WriteString(category.m_name);
                bw.WriteString(category.m_codes);
                bw.WriteInt(category.m_type);
                bw.WriteInt(category.m_orderNum);
            }
            byte[] bytes = bw.GetBytes();
            int ret = Send(new CMessage(GroupID, ServiceID, functionID, SessionID, requestID, m_socketID, 0, CompressType, bytes.Length, bytes));
            bw.Close();
            return ret;
        }

        /// <summary>
        /// 更新类别信息
        /// </summary>
        /// <param name="requestID">请求ID</param>
        /// <param name="category">类别</param>
        /// <returns>状态</returns>
        public int UpdateCategory(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_UPDATECATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        ///// <summary>
        ///// 更新指标的排序
        ///// </summary>
        ///// <param name="requestID">请求ID</param>
        ///// <param name="userID">用户ID</param>
        ///// <param name="ids">ID</param>
        ///// <returns>状态</returns>
        //public int UpdateOrderNum(int requestID, int userID, List<String> ids)
        //{
        //    String str = "";
        //    int idsSize = ids.Count;
        //    for (int i = 0; i < idsSize; i++)
        //    {
        //        str += ids[i];
        //        if (i != idsSize - 1)
        //        {
        //            str += ",";
        //        }
        //    }
        //    UserSecurityCategory category = new UserSecurityCategory();
        //    category.m_categoryID = str;
        //    category.m_userID = userID;
        //    List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
        //    categories.Add(category);
        //    int ret = Send(FUNCTIONID_USERSECURITY_UPDTAEORDERNUM, requestID, categories);
        //    categories.Clear();
        //    return ret > 0 ? 1 : 0;
        //}
        #endregion
    }
}
