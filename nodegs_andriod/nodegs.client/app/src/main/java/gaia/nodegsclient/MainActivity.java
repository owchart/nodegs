package gaia.nodegsclient;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import gaia.nodegsclient.base.*;
import gaia.nodegsclient.business.*;

import java.util.ArrayList;
public class MainActivity extends AppCompatActivity implements Runnable {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Thread t = new Thread(this);
        t.start();
    }

    public void run()
    {
        
        DataCenter.StartService();

        int requestID = 1;
        CMessage message = new CMessage();
        int socketID = BaseService.ConnectToServer(0, "192.168.86.105", 8081,"", 0, "", "", "", 30000);
        DataCenter.GetLoginService().SetSocketID(socketID);

        DataCenter.GetLoginService().RegisterWait(requestID, message);

        DataCenter.GetLoginService().Login("UserName1", "123456", requestID);
        DataCenter.GetLoginService().WaitMessage(requestID, 1000000);

        ArrayList<LoginInfo> infos = new ArrayList<LoginInfo>();
        LoginService.GetLoginInfos(infos, message.m_body, message.m_bodyLength);

        int a = 0;
    }
}
