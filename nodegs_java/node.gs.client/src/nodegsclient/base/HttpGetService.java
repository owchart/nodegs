package nodegsclient.base;

//*************************************************************************************\
//*                                                                                      *
//* HttpGetService.cs -  Http get service functions, types, and definitions.             *
//*                                                                                      *
//*               Version 1.00 �                                                      *
//*                                                                                      *
//*               Copyright (c) 2016-2016, Server. All rights reserved.                  *
//*               Created by Lord.                                                    *
//*                                                                                      *
//**************************************************************************************

import java.io.*;
import java.net.*;

/*
HTTP的GET服务
*/
public class HttpGetService {
	/*
	 * 创建服务
	 */
	public HttpGetService() {
	}

	/*
	 * 请求HTTP数据
	 */
	public String GET(String url) {
		String result = "";
		BufferedReader reader = null;
		try {
			URL realUrl = new URL(url);
			URLConnection connection = realUrl.openConnection();
			connection.setRequestProperty("accept", "text/html");
			connection.setRequestProperty("connection", "Keep-Alive");
			connection.setRequestProperty("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
			connection.connect();
			reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
			String line;
			while ((line = reader.readLine()) != null) {
				result += line;
			}
		} catch (Exception e) {
			System.out.println("发送GET请求出现异常！" + e);
			e.printStackTrace();
		}
		// 使用finally块来关闭输入流
		finally {
			try {
				if (reader != null) {
					reader.close();
				}
			} catch (Exception e2) {
				e2.printStackTrace();
			}
		}
		return result;
	}
}
