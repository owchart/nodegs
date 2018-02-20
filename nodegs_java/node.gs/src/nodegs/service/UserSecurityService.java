/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.service;

import com.mongodb.*;
import gaia.owchart.owchart_android.Base.CStr;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;
import nodegs.base.Binary;
import nodegs.base.CMessage;
import nodegs.base.HttpHardService;

/**
 *
 * @author GAIA
 */
public class UserSecurityService extends HttpHardService
{   
    /*
        创建服务
    */
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

    public final String MONGODB_USER_CATAGORY = "UserCatagory";

    private MongoService m_mongoService;
        
    public DBCollection GetCollection()
    {
        DBCollection collection = m_mongoService.GetCollectionByName(MONGODB_USER_CATAGORY);
        return collection;
    }
        
    /**
     * 添加自选股类别
     * @param message 消息
     * @return  状态
     */
    public int AddCategories(CMessage message)
    {
        ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
        GetCategories(categories, message.m_body, message.m_bodyLength);
        int categoriesSize = categories.size();
        ArrayList<UserSecurityCategory> addCategories = new ArrayList<UserSecurityCategory>();
        for (int i = 0; i < categoriesSize; i++)
        {
            UserSecurityCategory receive = categories.get(i);
            receive.m_categoryID = UUID.randomUUID().toString();
            
            GetCollection().insert(UserSecurityCategory2DBObject(receive));
            
            addCategories.add(receive);
        }
        int ret = Send(message, addCategories);
        categories.clear();
        addCategories.clear();
        return ret;
    }
        
    /**
     * 添加证券
     * @param message 消息
     * @return 状态
     */
    public int AddSecurities(CMessage message)
    {
        UserSecurityCategory receive = new UserSecurityCategory();
        ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
        GetCategories(categories, message.m_body, message.m_bodyLength);
        receive = categories.get(0);
        categories.clear();            
        GetCategories(categories, receive.m_userID, receive.m_categoryID);
        int categoriesSize = categories.size();
        if (categoriesSize > 0)
        {
            ArrayList<String> addedCodes = new ArrayList<String>();
            UserSecurityCategory category = categories.get(0);
            String[] newCodes = receive.m_codes.split(",");
            int newSize = newCodes.length;
            if (category.m_codes != null && category.m_codes.length() > 0)
            {
                String[] oldCodes = category.m_codes.split(",");
                HashMap<String, String> oldCodesMap = new HashMap<String, String>();
                int oldSize = oldCodes.length;
                for (int i = 0; i < oldSize; i++)
                {
                    oldCodesMap.put(oldCodes[i], "");
                }
                for (int i = 0; i < newSize; i++)
                {
                    if (!oldCodesMap.containsKey(newCodes[i]))
                    {
                        addedCodes.add(newCodes[i]);
                        category.m_codes += "," + newCodes[i];
                    }
                }
            }
            else
            {
                category.m_codes = receive.m_codes;
                for (int i = 0; i < newSize; i++)
                {
                    addedCodes.add(newCodes[i]);
                }
            }
            UpdateCategory(category);
            String sendCodes = "";
            int addedCodesSize = addedCodes.size();
            for (int i = 0; i < addedCodesSize; i++)
            {
                sendCodes += addedCodes.get(i);
                if (i != addedCodesSize - 1)
                {
                    sendCodes += ",";
                }
            }
            category.m_codes = sendCodes;
            int ret = Send(message, categories);
            categories.clear();
            return ret;
        }
        return -1;
    }
        
    public static UserSecurityCategory DBObject2UserSecurityCategory(DBObject DBObject)
    {
        @SuppressWarnings("unchecked")
        Map<String, Object> userMap = (Map<String, Object>) DBObject.toMap();

        UserSecurityCategory userSecurityCategory = new UserSecurityCategory();  
        userSecurityCategory.m_categoryID = String.valueOf(userMap.get("m_categoryID"));
        userSecurityCategory.m_codes = String.valueOf(userMap.get("m_codes"));
        userSecurityCategory.m_name = String.valueOf(userMap.get("m_name"));
        userSecurityCategory.m_orderNum = Integer.valueOf(String.valueOf(userMap.get("m_orderNum")));
        userSecurityCategory.m_type = Integer.valueOf(String.valueOf(userMap.get("m_type")));
        userSecurityCategory.m_userID = Integer.valueOf(String.valueOf(userMap.get("m_userID")));
        return userSecurityCategory;
    }

    /**
     * 删除类别
     * @param message 消息
     * @return 状态
     */
    public int DeleteCategories(CMessage message)
    {
        ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
        GetCategories(categories, message.m_body, message.m_bodyLength);
        int categoriesSize = categories.size();
        ArrayList<UserSecurityCategory> deleteCategories = new ArrayList<UserSecurityCategory>();
        for (int i = 0; i < categoriesSize; i++)
        {
            UserSecurityCategory receive = categories.get(i);    

            GetCollection().remove(UserSecurityCategory2DBObject(receive));
            
            deleteCategories.add(receive);
        }
        int ret = Send(message, deleteCategories);
        categories.clear();
        deleteCategories.clear();
        return ret;
    }

    /**
     * 删除证券
     * @param message
     * @return 
     */
    public int DeleteSecurities(CMessage message)
    {         
        UserSecurityCategory receive = new UserSecurityCategory(); 
        ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
        GetCategories(categories, message.m_body, message.m_bodyLength);
        int categoriesSize = categories.size();
        ArrayList<UserSecurityCategory> deleteCategories = new ArrayList<UserSecurityCategory>();
         if (categoriesSize > 0)
        {
            UserSecurityCategory category = categories.get(0);
            ArrayList<String> deletedCodes = new ArrayList<String>();
            if (category.m_codes != null && category.m_codes.length() > 0)
            {
                String[] newCodes = receive.m_codes.split(",");
                String[] oldCodes = category.m_codes.split(",");
                HashMap<String, String> oldCodesMap = new HashMap<String, String>();
                HashMap<String, String> newCodesMap = new HashMap<String, String>();
                int oldSize = oldCodes.length;
                for (int i = 0; i < oldSize; i++)
                {
                    oldCodesMap.put(oldCodes[i], "");
                }
                int newSize = newCodes.length;
                for (int i = 0; i < newSize; i++)
                {
                    newCodesMap.put(newCodes[i], "");
                }
                for (String code : newCodesMap.keySet())
                {
                    if (oldCodesMap.containsKey(code))
                    {
                        deletedCodes.add(code);
                        oldCodesMap.remove(code);
                    }
                }
                int pos = 0;
                oldSize = oldCodesMap.size();
                String newSaveCodes = "";
                for (String code : oldCodesMap.keySet())
                {
                    newSaveCodes += code;
                    if (pos != oldSize - 1)
                    {
                        newSaveCodes += ",";
                    }
                    pos++;
                }
                category.m_codes = newSaveCodes;
            }
            UpdateCategory(category);
            String sendCodes = "";
            int deletedCodesSize = deletedCodes.size();
            for (int i = 0; i < deletedCodesSize; i++)
            {
                sendCodes += deletedCodes.get(i);
                if (i != deletedCodesSize - 1)
                {
                    sendCodes += ",";
                }
            }
            category.m_codes = sendCodes;
            int ret = Send(message, categories);
            categories.clear();
            return ret;
        }
        return -1;
    }
        
    /**
     *  获取类别信息
     * @param categories 类别
     * @param body 包体
     * @param bodyLength  包体长度
     */
    private void GetCategories(ArrayList<UserSecurityCategory> categories, byte[] body, int bodyLength)
    {
        try
        {
            Binary br = new Binary();
            br.Write(body, bodyLength);
            int categoriesSize = br.ReadInt();
            for (int i = 0; i < categoriesSize; i++)
            {
                UserSecurityCategory category = new UserSecurityCategory();
                category.m_categoryID = br.ReadString();
                category.m_userID = br.ReadInt();
                category.m_name = br.ReadString();
                category.m_codes = br.ReadString();
                category.m_type = br.ReadInt();
                category.m_orderNum = br.ReadInt();
                if (category.m_categoryID == "primary")
                {
                    category.m_categoryID = GetPrimaryCategoryID(category.m_userID);
                    if (category.m_categoryID.length() > 0)
                    {
                        categories.add(category);
                    }
                }
                else
                {
                    categories.add(category);
                }
            }
            br.Close();                
        }
        catch(Exception ex)
        {
        }            
    }
        
    /**
     *  获取类别信息
     * @param categories 类别信息
     * @param userID 用户ID
     * @param categoryID 类别ID
     * @return  状态
     */
    public int GetCategories(ArrayList<UserSecurityCategory> categories, int userID, String categoryID)
    {
        DBCollection userCollection = GetCollection();
        BasicDBObject query =new BasicDBObject();
        if(categoryID.length() != 0)
        {
            query.put("m_categoryID", categoryID);
        }
        UserSecurityCategory category = null;

        ArrayList<UserSecurityCategory> allCategory = new ArrayList<UserSecurityCategory>();
        DBCursor cursor = userCollection.find(query);
        while(cursor.hasNext())
        {
            DBObject dbObject = cursor.next();            
            category = DBObject2UserSecurityCategory(dbObject);
            allCategory.add(category);              
        }
        
        int size = allCategory.size();
        if (categoryID != null && categoryID.length() > 0)
        {
            for (int i = 0; i < size; i++)
            {
                if (allCategory.get(i).m_categoryID.equals(categoryID))
                {
                    categories.add(allCategory.get(i));
                }
            }           
        }
        else
        {
            if (userID > 0)
            {
                for (int i = 0; i < size; i++)
                {
                    if (allCategory.get(i).m_userID == userID)
                    {
                        categories.add(allCategory.get(i));
                    } 
                }
            }
            else
            {
                categories = allCategory;
            }            
        }
        return 1;
    }
        
    /**
     * 获取所有类别信息
     * @param message 消息
     * @return 状态
     */
    public int GetCategories(CMessage message)
    {
        UserSecurityCategory receive = new UserSecurityCategory();
        ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
        GetCategories(categories, message.m_body, message.m_bodyLength);
        receive = categories.get(0);
        categories.clear();
        GetCategories(categories, receive.m_userID, receive.m_categoryID);
        int ret = Send(message, categories);
        return ret;
    }
        
    /**
     * 获取主要的类别ID
     * @param userID 用户ID
     * @return 类别ID
     */
    public String GetPrimaryCategoryID(int userID)
    {
        String categoryID = "111111";
        return categoryID;
    }

    @Override
    public void OnReceive(CMessage message)
    {
        super.OnReceive(message);
        switch (message.m_functionID)
        {
            case FUNCTIONID_USERSECURITY_ADDCATEGORIES:
                AddCategories(message);
                break;
            case FUNCTIONID_USERSECURITY_DELETECATEGORIES:
                DeleteCategories(message);
                break;
            case FUNCTIONID_USERSECURITY_GETCATEGORIES:
                GetCategories(message);
                break;
            case FUNCTIONID_USERSECURITY_UPDATECATEGORIES:
                UpdateCategories(message);
                break;
            case FUNCTIONID_USERSECURITY_ADDSECURITIES:
                AddSecurities(message);
                break;
            case FUNCTIONID_USERSECURITY_DELETESECURITIES:
                DeleteSecurities(message);
                break;
            case FUNCTIONID_USERSECURITY_UPDTAEORDERNUM:
                UpdateOrderNum(message);
                break;
        }
    }

    /// <summary>
    /// 发送类别
    /// </summary>
    /// <param name="message">消息</param>
    /// <param name="categories">类别列表</param>
    public int Send(CMessage message, ArrayList<UserSecurityCategory> categories)
    {
        try
        {
            Binary bw = new Binary();
            int size = categories.size();
            bw.WriteInt(size);
            for (int i = 0; i < size; i++)
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
            message.m_bodyLength = bytes.length;
            message.m_body = bytes;
            int ret = Send(message);
            bw.Close();
            return ret;
        }
        catch(Exception ex)
        {
        }
        return 1;
    }

    /**
     *  更新类别
     * @param message 消息
     * @return 状态
     */
    public int UpdateCategories(CMessage message)
    {
        ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
        GetCategories(categories, message.m_body, message.m_bodyLength);
        int categoriesSize = categories.size();
        ArrayList<UserSecurityCategory> updateCategories = new ArrayList<UserSecurityCategory>();
        for (int i = 0; i < categoriesSize; i++)
        {
            UserSecurityCategory receive = categories.get(i);
            UpdateCategory(receive);
            updateCategories.add(receive);
        }
        int ret = Send(message, updateCategories);
        categories.clear();
        updateCategories.clear();
        return ret;
    }

    /**
     * 更新类别信息
     * @param category 类别信息
     */
    public void UpdateCategory(UserSecurityCategory category)
    {
        GetCollection().update(new BasicDBObject().append("m_categoryID", category.m_categoryID), UserSecurityCategory2DBObject(category));
    }

    /**
     * 更新排序字段
     * @param message 消息
     * @return 状态
     */
    public int UpdateOrderNum(CMessage message)
    {
        ArrayList<UserSecurityCategory> categories = new ArrayList<UserSecurityCategory>();
        GetCategories(categories, message.m_body, message.m_bodyLength);
        int categoriesSize = categories.size();
        ArrayList<UserSecurityCategory> updateCategories = new ArrayList<UserSecurityCategory>();
        for (int i = 0; i < categoriesSize; i++)
        {
            UserSecurityCategory receive = categories.get(i);
            String[] ids = receive.m_categoryID.split(",");
            int idsSize = ids.length;
            for (int j = 0; j < idsSize; j++)
            {
                receive.m_orderNum = j;

                GetCollection().insert(UserSecurityCategory2DBObject(receive));
            }
            updateCategories.add(receive);
        }
        int ret = Send(message, updateCategories);
        categories.clear();
        updateCategories.clear();
        return ret;
    }    
        
    public static BasicDBObject UserSecurityCategory2DBObject(UserSecurityCategory category)
    {
        BasicDBObject object = new BasicDBObject();
        object.put("m_categoryID", category.m_categoryID);
        object.put("m_codes", category.m_codes);
        object.put("m_name", category.m_name);
        object.put("m_orderNum", category.m_orderNum);
        object.put("m_type", category.m_type);
        object.put("m_userID", category.m_userID);
        return object;
    }
}
