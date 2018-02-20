/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegsclient.service;

import java.util.ArrayList;
import nodegsclient.base.Binary;
import nodegsclient.base.CMessage;
import nodegsclient.base.HttpPostService;

/**
 *
 * @author GAIA
 */
public class UserSecurityService extends HttpPostService {
        public UserSecurityService()
        {
            SetServiceID(SERVICEID_USERSECURITY);
        }
        
        /**
         * 自选股服务ID
        */
        public final int SERVICEID_USERSECURITY = 1;

        /**
         * 添加类别方法ID
         */
        public final int FUNCTIONID_USERSECURITY_ADDCATEGORIES = 0;

        /**
         * 删除类别方法ID
         */
        public final int FUNCTIONID_USERSECURITY_DELETECATEGORIES = 1;

        /**
         * 获取所有类别信息方法ID
         */
        public final int FUNCTIONID_USERSECURITY_GETCATEGORIES = 2;

        /**
         * 更新类别信息方法ID
         */
        public final int FUNCTIONID_USERSECURITY_UPDATECATEGORIES = 3;

        /**
         * 添加证券方法ID
         */
        public final int FUNCTIONID_USERSECURITY_ADDSECURITIES = 4;

        /**
         * 删除证券方法ID
         */
        public final int FUNCTIONID_USERSECURITY_DELETESECURITIES = 5;

        /**
         * 更新类别排序方法ID
         */
        public final int FUNCTIONID_USERSECURITY_UPDTAEORDERNUM = 6;
                
        /**
         * 添加自选股类别
         * @param requestID 请求ID
         * @param category 类别
         * @return 状态
         */
        public int AddCategory(int requestID, UserSecurityCategory category)
        {
            ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
            categories.add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_ADDCATEGORIES, requestID, categories);
            categories.clear();
            return ret > 0 ? 1 : 0;
        }
        
        /**
         * 添加证券
         * @param requestID 请求ID
         * @param category 类别
         * @return  状态
         */
        public int AddSecurities(int requestID, UserSecurityCategory category)
        {
            ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
            categories.add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_ADDSECURITIES, requestID, categories);
            categories.clear();
            return ret > 0 ? 1 : 0;
        }
        
        /**
         * 删除类别
         * @param requestID 请求ID
         * @param category 类别
         * @return  状态
         */
        public int DeleteCategory(int requestID, UserSecurityCategory category)
        {
            ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
            categories.add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_DELETECATEGORIES, requestID, categories);
            categories.clear();
            return ret > 0 ? 1 : 0;
        }
        
        /**
         * 删除证券
         * @param requestID 请求ID
         * @param category 类别
         * @return  状态
         */
        public int DeleteSecurities(int requestID, UserSecurityCategory category)
        {
            ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
            categories.add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_DELETESECURITIES, requestID, categories);
            categories.clear();
            return ret > 0 ? 1 : 0;
        }
        
        /**
         * 获取类别信息
         * @param requestID 请求ID
         * @param category 类别
         * @return  状态
         */
        public int GetCategories(int requestID, UserSecurityCategory category)
        {
            ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
            categories.add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_GETCATEGORIES, requestID, categories);
            categories.clear();
            return ret > 0 ? 1 : 0;
        }

        /**
         * 通过流获取类别信息列表
         * @param categories 类别信息
         * @param body 包体
         * @param bodyLength 包体长度
         * @return  状态
         */
        public static int GetCategories(ArrayList<UserSecurityCategory> categories, byte[] body, int bodyLength)
        {
            try
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
                        categories.add(category);
                    }
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
         */
        @Override
        public void OnReceive(CMessage message)
        {
            super.OnReceive(message);
            SendToListener(message);
        }
        /**
         * 发送消息
         * @param functionID 方法ID
         * @param requestID 请求ID
         * @param categories 类别集合
         * @return 状态
         */
        public int Send(int functionID, int requestID, ArrayList<UserSecurityCategory> categories)
        {
            try
            {
                Binary bw = new Binary();
                int categoriesSize = categories.size();
                bw.WriteInt(categoriesSize);
                for (int i = 0; i < categoriesSize; i++)
                {
                    UserSecurityCategory category = categories.get(i);
                    bw.WriteString(category.m_categoryID);
                    bw.WriteInt(category.m_userID);
                    bw.WriteString(category.m_name);
                    bw.WriteString(category.m_codes);
                    bw.WriteInt(category.m_type);
                    bw.WriteInt(category.m_orderNum);
                }
                byte[] bytes = bw.GetBytes();
                int ret = Send(new CMessage(GetGroupID(), GetServiceID(), functionID, GetSessionID(), requestID, 0, 0, GetCompressType(), bytes.length, bytes));
                bw.Close();
                return ret;
            }
            catch(Exception ex)
            {
            }        
            return 0;
        }
        
        /**
         * 更新类别信息
         * @param requestID 请求ID
         * @param category 类别
         * @return 状态
         */
        public int UpdateCategory(int requestID, UserSecurityCategory category)
        {
            ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
            categories.add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_UPDATECATEGORIES, requestID, categories);
            categories.clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// 更新指标的排序
        /// </summary>
        /// <param name="requestID">请求ID</param>
        /// <param name="userID">用户ID</param>
        /// <param name="ids">ID</param>
        /// <returns>状态</returns>
        /**
         * 更新指标的排序
         * @param requestID 请求ID
         * @param userID 用户ID
         * @param ids IDS
         * @return 状态
         */
        public int UpdateOrderNum(int requestID, int userID, ArrayList<String> ids)
        {
            String str = "";
            int idsSize = ids.size();
            for (int i = 0; i < idsSize; i++)
            {
                str += ids.get(i);
                if (i != idsSize - 1)
                {
                    str += ",";
                }
            }
            UserSecurityCategory category = new UserSecurityCategory();
            category.m_categoryID = str;
            category.m_userID = userID;
            ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
            categories.add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_UPDTAEORDERNUM, requestID, categories);
            categories.clear();
            return ret > 0 ? 1 : 0;
        }
}
