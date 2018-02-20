using System;
using System.Collections.Generic;
using System.Text;
using OwLib;
using System.Threading;

namespace node.gs
{
    class Program
    {
        static void Main(string[] args)
        {
            INativeBase native = NativeHandler.CreateNative();
            String script = "TEST(100);TEST(100);TEST(100);";
            CIndicator indicator = CFunctionEx.CreateIndicator(script, native);
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            for (int i = 0; i < 1000000; i++)
            {
                CMathElement ce = new CMathElement();
            }
            sw.Stop();
            Console.WriteLine(sw.ElapsedMilliseconds);
            Console.ReadLine();
            string appPath = DataCenter.GetAppPath();
            string fileName = appPath + "\\test.js";
            if (args != null && args.Length > 0)
            {
                fileName = appPath + "\\" + args[0];
            }

            DataCenter.StartService(fileName);
        }
    }
}
