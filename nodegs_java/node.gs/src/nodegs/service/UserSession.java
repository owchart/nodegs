package nodegs.service;

/** 
 用户会话信息
 
*/
public class UserSession
{
	/** 
	 会话键
	 
	*/
	public String m_key = "";

	/** 
	 用户ID
	 
	*/
	public int m_userID;

	/** 
	 会话值
	 
	*/
	public String m_value = "";
        
    public String toString()
    {
        return "UserID [userID=" + m_userID + ", key=" + m_key + ", value=" + m_value + "]";
    }
}
