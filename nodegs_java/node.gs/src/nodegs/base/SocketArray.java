package nodegs.base;

/** 
 套接字连接组信息
 
*/
public class SocketArray
{
	/** 
	 套接字ID组
	 
	*/
	private java.util.ArrayList<Integer> m_sockets = new java.util.ArrayList<Integer>();

	/** 
	 添加套接字ID
	 
	 @param socketID 套接字ID
	*/
	public final void AddSocket(int socketID)
	{
		int socketsSize = m_sockets.size();
		for (int i = 0; i < socketsSize; i++)
		{
			if (m_sockets.get(i) == socketID)
			{
				return;
			}
		}
		m_sockets.add(socketID);
	}

	/** 
	 获取套接字列表
	 
	 @param socketList 套接字列表
	*/
	public final void GetSocketList(java.util.ArrayList<Integer> socketList)
	{
		int socketsSize = m_sockets.size();
		for (int i = 0; i < socketsSize; i++)
		{
			socketList.add(m_sockets.get(i));
		}
	}

	/** 
	 移除套接字ID
	 
	 @param socketID 套接字ID
	*/
	public final void RemoveSocket(int socketID)
	{
		m_sockets.remove(socketID);
	}
}