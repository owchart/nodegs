//LORD_CFILE_CPP_S5:LIWEI,50%;
/*****************************************************************************\
*                                                                             *
* CFile.h -      File functions                                               *
*                                                                             *
*               Version 1.00 ������                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's OwChart. All rights reserved. *
*                                                                             *
*******************************************************************************/

#ifndef __CFILE_H__
#define __CFILE_H__
#pragma once
#include "stdafx.h"
#include "io.h"
#include <direct.h>
#include <fstream>
#include <sys/stat.h>

namespace OwLib
{
	class CFileA
	{
	public:
		//׷������
		static bool Append(const char *file, const char *content);
		//׷������-���ַ�
		static bool Append(const String &file, const String &content);
		//����Ŀ¼
		static void CreateDirectory(const char *dir);
		//����Ŀ¼-���ַ�
		static void CreateDirectory(const String &dir);
		//�ж�Ŀ¼�Ƿ����
		static bool IsDirectoryExist(const char *dir);
		//�ж�Ŀ¼�Ƿ����-���ַ�
		static bool IsDirectoryExist(const String &dir);
		//�ļ��Ƿ����
		static bool IsFileExist(const char *file);
		//�ļ��Ƿ����-���ַ�
		static bool IsFileExist(const String &file);
		//��ȡĿ¼
		static bool GetDirectories(const char *dir, vector<string> *dirs);
		//��ȡĿ¼-���ַ�
		static bool GetDirectories(const String &file, vector<String> *dirs);
		//��ȡ�ļ�����
		static int GetFileLength(const char *file);
		//��ȡ�ļ�����-���ַ�
		static int GetFileLength(const String &file);
		//��ȡ�ļ�
		static bool GetFiles(const char *dir, vector<string> *files);
		//��ȡ�ļ�-���ַ�
		static bool GetFiles(const String &dir, vector<String> *files);
		//��ȡ�ļ�״̬
		static int GetFileState(const char *file, struct stat *buf);
		//��ȡ�ļ�״̬-���ַ�
		static int GetFileState(const String &file, struct stat *buf);
		//��ȡ�ļ�
		static bool Read(const char *file, string *content);
		//��ȡ�ļ�-���ַ�
		static bool Read(const String &file, String *content);
		//��ȡ�ļ���
		static bool ReadLine(ifstream *fs, string *lineContent, int lineLength);
		//�Ƴ��ļ�
		static void RemoveFile(const char *file);
		//�Ƴ��ļ�-���ַ� 
		static void RemoveFile(const String &file);
		//д���ļ�
		static bool Write(const char *file, const char *content);
		//д���ļ�-���ַ�
		static bool Write(const String &file, const String &content);
	};
}
#endif