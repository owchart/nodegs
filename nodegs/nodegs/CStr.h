/*****************************************************************************\
*                                                                             *
* Str.h -      String functions                                               *
*                                                                             *
*               Version 4.00 ¡ï¡ï¡ï¡ï¡ï                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's OwChart. All rights reserved. *
*                                                                             *
*******************************************************************************/

#ifndef __CSTRA_H__
#define __CSTRA_H__
#pragma once
#include "stdafx.h"
#include "objbase.h"
#pragma comment(lib,"ole32.lib") 
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib,"zlib1.lib") 

namespace OwLibSV
{
	class CodeConvert_Win
	{
	public:
		CodeConvert_Win(const char* input, unsigned int fromCodePage, unsigned int toCodePage);
		~CodeConvert_Win() {
			delete [] wcharBuf;        
			delete [] charBuf;    
			};    
			const char * ToString() {    
				return charBuf;    
			};
	private:    
		  wchar_t * wcharBuf;    
		  char * charBuf;
	};

	class CStrA
	{
	public:
		static void ANSCToUnicode(string& out, const char* inSrc);
		static int Compress(Byte *data, uLong ndata, Byte *zdata, uLong *nzdata);
		static void Contact(wchar_t *str, LPCTSTR str1, LPCTSTR str2 = L"", LPCTSTR str3 = L"");
		static string ConvertDBCodeToFileName(const string& code);
		static string ConvertDBCodeToSinaCode(const string& code);
		static string ConvertDBCodeToTencentCode(const string& code);
		static String ConvertSinaCodeToDBCode(const String& code);
		static double ConvertStrToDouble(const wchar_t *str);
		static int ConvertStrToInt(const wchar_t *str);
		static String ConvertTencentCodeToDBCode(const String& code);
		static int DeCompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
		static String GetDBString(const String& strSrc);
		static string GetGuid();
		static void GetFormatDate(double date, wchar_t *str);
		static int GetStringCapacity(const string& str);
		static int GetStringCapacity(const String& str);
		static void GetValueByDigit(double value, int digit, wchar_t *str);
		static int GZCompress(Byte *data, uLong ndata, Byte *zdata, uLong *nzdata);
		static int GZDeCompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
		static double M129(int tm_year, int tm_mon, int tm_mday, int tm_hour, int tm_min, int tm_sec, int tm_msec);
		static void M130(double num, int *tm_year, int *tm_mon, int *tm_mday, int *tm_hour, int *tm_min, int *tm_sec, int *tm_msec);
		static string Replace(const string& str, const string& src, const string& dest);
		static String Replace(const String& str, const String& src, const String& dest);
		static vector<string> Split(string str, string pattern);
		static vector<String> Split(String str, String pattern);
		static void stringTowstring(String &strDest, const string& strSrc);
		static String ToLower(const String& str);
		static String ToUpper(const String& str);
		static void UnicodeToANSC(string& out, const char* inSrc);
		static string UrlEncode(const std::string& szToEncode);
		static string UrlDecode(const std::string& szToDecode);
		static void wstringTostring(string &strDest, const String& strSrc);
	};
}
#endif