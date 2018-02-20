package nodegsclient.base;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;
import java.util.HashMap;
import java.util.UUID;
import java.util.Map;
import nodegsclient.base.*;

/**
 * Created by joel on 7/28/17.
 */

public class FileTransferService {
    /*
    * 构造方法
    * */
    public FileTransferService(){
        m_listeners = new HashMap<Integer,FileTransferListener>();
        m_runningWork = new HashMap<Integer,String>();
    }
    
    //正在进行监听的监听器
    public Map<Integer,FileTransferListener> m_listeners;

    //服务id(每次上传文件都会自增)
    private int m_maxID = 0;

    //正在运行中的线程
    public Map<Integer,String> m_runningWork;

    /**
     * 上传类是多线程
     */
    private class MultithreadUploadWork implements Runnable{

        private Object m_objs;

        /**
         * 构造方法
         * @param paramx
         */
        public MultithreadUploadWork(Object objs)
        {
            m_objs = objs;
        }

        /**
         * 线程方法开始
         */
        public void run() {
            Object[] obj = (Object[])m_objs;
            int id = (int)obj[0];
            String url = (String)obj[1];
            String filePath = m_runningWork.get(id);//获取文件名字
            FileTransferListener listener = m_listeners.get(id);//获取监听器
            DataInputStream fs = null;
            try {
                    fs = new DataInputStream(new FileInputStream(filePath));
                    URL realUrl = new URL(url);
                    URLConnection conn = realUrl.openConnection();
                    conn.setRequestProperty("accept", "text/html");
                    conn.setRequestProperty("connection", "Keep-Alive");
                    conn.setRequestProperty("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
                    conn.setRequestProperty("Content-Type:", url);
                    conn.setDoOutput(true);
                    conn.setDoInput(true);
                    long currentLength = 0;
                    long totalLength = fs.available();
                    DataOutputStream writer = new DataOutputStream(conn.getOutputStream());
                    byte[] buffer = new byte[10240];
                    while (true)
                    {
                        int len = fs.read(buffer, 0, 10240);
                        writer.write(buffer);
                        if (listener != null)
                        {
                            listener.OnFileTransfer(id, totalLength, currentLength, 0, null);
                        }
                        currentLength += len;
                        if (len <= 0)
                        {
                            break;
                        }
                    }
                    writer.flush();
                    writer.close();
                    DataInputStream reader = new DataInputStream(conn.getInputStream());
                    Binary br = new Binary();
                    while (true) {
                            int data = reader.read();
                            if (data != -1) {
                                    br.WriteByte((byte) data);
                            } else {
                                    break;
                            }
                    }
                    reader.close();
                    byte[] dataArray = br.GetBytes();     
                    br.Close();
                    if (listener != null)
                    {
                        listener.OnFileTransfer(id, totalLength, currentLength, 0, null);
                    }
            } 
            catch (Exception ex)
            {
                if (listener != null)
                {
                    listener.OnFileTransfer(id, 0, 0, 3, ex.getMessage());
                }
            }
            finally
            {
                try
                {
                    if(fs != null)
                    {
                        fs.close();
                    }
                }
                catch(Exception ex)
                {
                    if (listener != null)
                    {
                        listener.OnFileTransfer(id, 0, 0, 3, ex.getMessage());
                    }
                }
            }
        }
    }

    /**
     * 上传文件方法
     * @param url 远程服务器地址
     * @param filePath 本地文件的绝对路径
     * @param listener 监听器
     * @return
     */
    public int Upload(String url, String filePath, FileTransferListener listener)
    {
        int id = m_maxID++;
        m_runningWork.put(id, filePath);
        m_listeners.put(id, listener);
        Object[] objs = { id, url };
        Thread thread = new Thread(new MultithreadUploadWork(objs));
        thread.start();
        return 0;
    }
}
