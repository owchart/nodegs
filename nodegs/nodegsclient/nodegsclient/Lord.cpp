// pyctp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "Service\HttpPostService.h"
#include "Service\Binary.h"
using namespace std;
using namespace OwLibCT;

static void Do()
{
	Binary br;
    for(int i = 0; i < 1000000; i++)
    {
        String x = L"1111111";
        br.WriteString(x);
    }
}

int _tmain(int argc, char* argv[])
{
	//XXXXXXXXX xxxxx;
	//xxxxx.Read(L"C:\\Miao\\X\\owchart\\win\\owmath.dll");
	//vector<String> paths;
	//XXXXXXXXX::GetFiles(L"C:\\Miao\\X\\owchart\\win", &paths);

	std::locale::global(std::locale(""));
	Do();
	cout << "1111";
	HttpPostService postService;
	for(int i = 0; i < 100000; i++)
	{
		String str = postService.Post(L"http://hqa.sinajs.cn/list=sh601857");
		cout << str.c_str() << "\r\n";
	}
	while(true)
	{
		char *input = new char[1024];
		cin >> input;
		if(strcmp(input, "exit") == 0)
		{
			break;
		}
	}
	return 0;
}


