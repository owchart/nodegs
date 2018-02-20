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
public class GroupTradeRatioTable
{
    /**
    *组编号
    */
    public String m_groupID;

    /**
    *交易账户
    */
    public String m_traderID;

    /**
    *品种代码：
     * 058 股票
     * 059 基金 
     * 060 债券 
     * 061 权证 
     * 062 期货 
     * 063 指数 
     * 067 现货
     *       外汇
     * 099 板块(Block)
     * 100 合约
    */
    public String m_productCode;

    /**
    *市场代码：
     * 069001001 上海证券交易所
     * 069001002 深圳证券交易所
     * 069001004 三板市场
     * 069001005 上海期货交易所 
     * 069001006 渤海商品交易所 
     * 069001007 大连商品交易所 
     * 069001008 郑州商品交易所 
     * 069001009 中国金融期货交易所 
     * 069001010 中国银行间市场 
     * 069001011 中国外汇交易市场 
     * 069001012 上海黄金交易所 
     * 069002001 香港商品交易所 
     * 069002002 香港金银业贸易场 
     * 069002003 香港银行间市场 
     * 069002004 香港证券交易所 
     * 069003 台湾
     * 070001 美国证券交易所 
     * 070002 纽约证券交易所 
     * 070003 纽约商业期货交易所 
     * 070004 纽约泛欧交易所 
     * 070005 纽约商品交易所
     * 070006001 美国纳斯达克市场 
     * 070006002 纳斯达克迪拜交易所 
     * 070006003 纳斯达克柜台交易市场 
     * 070007 美国洲际交易所 
     * 070008 芝加哥期权交易所 
     * 070009 芝加哥商业交易所
     * 071001 欧洲期货交易所 
     * 071002 伦敦黄金市场 
     * 071003 伦敦银行间市场 
     * 071004 伦敦金属交易所 
     * 071005 伦敦国际石油交易所 
     * 071006 伦敦国际金融期货期权交易所 
     * 072001 全球能源市场 
     * 072002 全球外汇交易市场 
     * 073001 巴西期货交易所 
     * 074001 韩国证券交易所 
     * 074002 印度国家证券交易所 
     * 074003 悉尼期货交易所 
     * 074004001 东京商品交易所 
     * 074004002 大阪证券交易所 
     * 074004003 东京证券交易所 
     * 074005001 新加坡银行间市场 
     * 074005002 新加坡交易所 
     * 075001 南非约翰内斯堡证券交易所
    */
    public String m_tradeMarketCode;

    /**
    *包含关系：
     * 0：指定品种以内
     * 1：与指定品种同一市场取反集
    */
    public int m_cotainType;

    /**
    *品种名称或者具体合约号
    */
    public String m_productValue;

    /**
    *组交易账户间配额方式
     * 1：按比例
     * 2：按手数
    */
    public int m_ratioType;

    /**
    *配额
     * 比例填写加小数点，如0.3=30%，手数直接填写数字
    */
    public double m_ratioValue;

    /**
    *合约到期时间(期货、期权等使用)
     * 年/月/日
    */
    public long m_endDate;

    /**
    *时间戳
    */
    public long m_timestamp;

    /**
    *最后操作数据的用户ID
    */
    public String m_lastModifyUser;
}
