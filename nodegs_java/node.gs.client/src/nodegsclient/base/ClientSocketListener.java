package nodegsclient.base;
import gaia.owchart.owchart_android.Sock.SocketListener;

/**
 * Created by gaia-todd on 2017/2/17.
 */

public class ClientSocketListener implements SocketListener
{
    public ClientSocketListener()
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
