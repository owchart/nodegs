// pyctp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "Service\HttpPostService.h"
#include "Service\Binary.h"
#include "Service\LoginService.h"
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
	LoginService *loginService = new LoginService;
	BaseService::AddService(loginService);
	int socketID = BaseService::ConnectToServer(0, 0, "127.0.0.1", 16666, "", 0, "", "", "", 0);
	loginService->SetSocketID(socketID);
	loginService->Login(L"21212", L"212112", 0);
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


