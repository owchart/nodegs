using System;
using System.Collections.Generic;
using System.Text;
using OwLibCT;
using System.Threading;
using System.IO;

namespace node.gs
{
    class Program
    {
        static void Main(string[] args)
        {
            HttpPostService h = new HttpPostService();
            String str = h.Post("http://192.168.88.101:8084/cashoutinmanagerservice?accessToken=0&function=getcashinoutprice&traderid=TA0000NG", "");
            DataCenter.StartService();
            Console.ReadLine();
        }
    }
}
