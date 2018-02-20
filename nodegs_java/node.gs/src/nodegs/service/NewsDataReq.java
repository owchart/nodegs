/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package nodegs.service;

/**
 *
 * @author GAIA
 */
public class NewsDataReq {
    /**
    * 代码
    */
    public String m_code = "";
    
    /**
    * 时间
    */
    public double m_time;

    /**
     * 标题
     */
    public String m_title = "";

    /**
     * 类型
     */
    public int m_type;
    
    /**
     * 最大条数
     */
    public int m_maxCount;
}
