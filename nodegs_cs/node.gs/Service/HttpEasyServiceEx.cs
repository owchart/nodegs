using System;
using System.Collections.Generic;
using System.Text;
using OwLibSV;

namespace node.gs
{
    public class HttpEasyServiceEx : HttpEasyService
    {
        public int OnReceive(HttpData data)
        {
            Binary binary = new Binary();
            binary.WriteString("1111");
            data.m_resBytes = binary.GetBytes();
            return -1;
        }
    }
}
