/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.service;

import java.net.UnknownHostException;

import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.Mongo;
import com.mongodb.MongoClient;
import com.mongodb.MongoException;

public class MongoService
{
    private static Mongo mongo = null;
    private static MongoClient mongoClient = null;
    
    private static DB db = null;
	
    /**
     * 初始化
     */
    public static void init()
    {
        try
        {
            mongo = new Mongo("192.168.88.101", 27017);
        }
        catch (UnknownHostException e)
        {
            System.out.println(e.toString());
        }
        catch (MongoException e)
        {
            System.out.println(e.toString());
        }

        if (null != mongo)
        {
            db = mongo.getDB("newsData");
            db.authenticate("user1", "111111".toCharArray());
        }
    }
	
    public static Mongo getMongo()
    {
        if (null == mongo)
        {
            init();
        }

        return mongo;
    }
	
    /**
     * 获取DB 
     * @return DB
     */
    public static DB getDB()
    {
        if (null == mongo)
        {
            init();
        }

        return db;
    }
	
    /**
     * 获取User Collection
     * @return DBCollection
     */
    public static DBCollection GetCollectionByName(String name)
    {
        if (null == db)
        {
            db = getDB();
        }

        if (null != db)
        {
            return db.getCollection(name);
        }

        return null;
    }
}