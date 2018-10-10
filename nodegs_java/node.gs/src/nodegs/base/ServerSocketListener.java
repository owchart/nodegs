package nodegs.base;

import owchart.owlib.Sock.SocketListener;

/**
 * Created by gaia-todd on 2017/2/17.
 */

public class ServerSocketListener implements SocketListener
{
    public ServerSocketListener()
    {

    }
    public void CallBack(int socketID, int localSID, byte []str, int len)
    {
        BaseService.CallBack(socketID, localSID, str, len);
    }
    public void WriteLog(int socketID, int localSID, int state, String log)
    {
        BaseService.WriteLog(socketID, localSID, state, log);
    }
}