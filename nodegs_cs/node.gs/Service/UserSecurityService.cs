using System;
using System.Collections.Generic;
using System.Text;
using node.gs;

namespace OwLibSV
{
    /// <summary>
    /// ��ѡ�����
    /// </summary>
    public class UserSecurityCategory
    {
        #region Lord 2016/12/22
        /// <summary>
        /// ���ID
        /// </summary>
        public String m_categoryID = "";

        /// <summary>
        /// ��Ʊ����
        /// </summary>
        public String m_codes = "";

        /// <summary>
        /// �������
        /// </summary>
        public String m_name = "";

        /// <summary>
        /// �б�˳��
        /// </summary>
        public int m_orderNum;

        /// <summary>
        /// ���
        /// </summary>
        public int m_type;

        /// <summary>
        /// �û�ID
        /// </summary>
        public int m_userID;
        #endregion
    }

    /// <summary>
    /// ��ѡ�ɷ���
    /// </summary>
    public class UserSecurityService : HttpHardService
    {
        #region Lord 2016/12/19
        /// <summary>
        /// ������ѡ�ɷ���
        /// </summary>
        public UserSecurityService()
        {
            ServiceID = SERVICEID_USERSECURITY;
        }

        /// <summary>
        /// ��ѡ�ɷ���ID
        /// </summary>
        public const int SERVICEID_USERSECURITY = 1;

        /// <summary>
        /// �����𷽷�ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_ADDCATEGORIES = 0;

        /// <summary>
        /// ɾ����𷽷�ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_DELETECATEGORIES = 1;

        /// <summary>
        /// ��ȡ���������Ϣ����ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_GETCATEGORIES = 2;

        /// <summary>
        /// ���������Ϣ����ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_UPDATECATEGORIES = 3;

        /// <summary>
        /// ���֤ȯ����ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_ADDSECURITIES = 4;

        /// <summary>
        /// ɾ��֤ȯ����ID
        /// </summary>
        public const int FUNCTIONID_USERSECURITY_DELETESECURITIES = 5;


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
        /// �����ѡ�����
        /// </summary>
        /// <param name="requestID">����ID</param>
        /// <param name="category">���</param>
        /// <returns>״̬</returns>
        public int AddCategory(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_ADDCATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// ���֤ȯ
        /// </summary>
        /// <param name="requestID">����ID</param>
        /// <param name="category">���</param>
        /// <returns>״̬</returns>
        public int AddSecurities(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_ADDSECURITIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// ɾ�����
        /// </summary>
        /// <param name="requestID">����ID</param>
        /// <param name="category">���</param>
        /// <returns>״̬</returns>
        public int DeleteCategory(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_DELETECATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// ɾ��֤ȯ
        /// </summary>
        /// <param name="requestID">����ID</param>
        /// <param name="category">���</param>
        /// <returns>״̬</returns>
        public int DeleteSecurities(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_DELETESECURITIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// ��ȡ�����Ϣ
        /// </summary>
        /// <param name="requestID">����ID</param>
        /// <param name="category">���</param>
        /// <returns>״̬</returns>
        public int GetCategories(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_GETCATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        /// <summary>
        /// ͨ������ȡ�����Ϣ�б�
        /// </summary>
        /// <param name="categories">�����Ϣ</param>
        /// <param name="body">����</param>
        /// <param name="bodyLength">���峤��</param>
        /// <returns>״̬</returns>
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
        /// <param name="categories">��𼯺�</param>
        /// <returns>״̬</returns>
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
        /// ���������Ϣ
        /// </summary>
        /// <param name="requestID">����ID</param>
        /// <param name="category">���</param>
        /// <returns>״̬</returns>
        public int UpdateCategory(int requestID, UserSecurityCategory category)
        {
            List<UserSecurityCategory> categories = new List<UserSecurityCategory>();
            categories.Add(category);
            int ret = Send(FUNCTIONID_USERSECURITY_UPDATECATEGORIES, requestID, categories);
            categories.Clear();
            return ret > 0 ? 1 : 0;
        }

        ///// <summary>
        ///// ����ָ�������
        ///// </summary>
        ///// <param name="requestID">����ID</param>
        ///// <param name="userID">�û�ID</param>
        ///// <param name="ids">ID</param>
        ///// <returns>״̬</returns>
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
