package nodegsclient;

import nodegsclient.base.*;
import nodegsclient.service.DataCenter;

/**
 *
 * @author GAIA_Todd
 */
public class NodeGsClient {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try
        {
        Binary br = new Binary();
        br.WriteInt(30000);
        for (int i = 0; i < 30000; i++)
        {
            br.WriteFloat(1);
            br.WriteFloat(1);
            br.WriteFloat(1);
            br.WriteFloat(1);
            br.WriteDouble(1);
            br.WriteDouble(1);
        }
        Binary bw = new Binary();
        byte[] bytes = br.GetBytes();
        bw.Write(bytes, bytes.length);
        int size = bw.ReadInt();
        for (int i = 0; i < size; i++)
        {
            bw.ReadFloat();
            bw.ReadFloat();
            bw.ReadFloat();
            bw.ReadFloat();
            bw.ReadDouble();
            bw.ReadDouble();
        }
        }catch(Exception ex)
        {
        }
        DataCenter.StartService();
        int socketID = BaseService.ConnectToServer(0, "127.0.0.1", 9965, "", 0, "", "", "", 30000);
	DataCenter.GetLoginService().SetSocketID(socketID);
        DataCenter.GetLoginService().GetLoginInfos(10000);
        while(true)
        {
            String str = "Hello server!";
            DataCenter.GetHttpPostService().SetUrl("http://localhost:8080/post/");
            str.getBytes();
        }
    }    
}
