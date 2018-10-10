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
            LoginService loginService = new LoginService();
            BaseService.AddService(loginService);
            int socketID = BaseService.Connect("127.0.0.1", 16666);
            loginService.SocketID = socketID;
            loginService.Login("21212", "212112", 0);
            Console.ReadLine();
        }
    }
}
