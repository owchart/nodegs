// owserver.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DataCenter.h"
#include "BaseService.h"
using namespace OwLibSV;

int _tmain(int argc, char* argv[])
{
	std::locale::global(std::locale(""));



	string fileName = DataCenter::GetAppDir() + "\\test.js";
	DataCenter::StartService(fileName);
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


