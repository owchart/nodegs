#include "stdafx.h"
#include "CStrA.h"

namespace OwLibCT
{
	string CStrA::ConvertDBCodeToFileName(const string& code)
	{
		string fileName = code;
		if (fileName.find(".") != -1)
        {
            fileName = fileName.substr(fileName.find('.') + 1) + fileName.substr(0, fileName.find('.'));
        }
		fileName += ".txt";
		return fileName;
	}
    
    double CStrA::ConvertStrToDouble(const wchar_t *str)
    {
        return wcstof(str, 0);
    }

    int CStrA::ConvertStrToInt(const wchar_t *str)
    {
        return wcstol(str, 0, 0);
    }
    
	void CStrA::GetFormatDate(double date, wchar_t *str)
	{
		int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, msecond = 0;
        M130(date, &year, &month, &day, &hour, &minute, &second, &msecond);
		if(hour != 0)
		{
			swprintf(str, 99, L"%02d:%02d", hour, minute);
		}
		else
		{
			swprintf(str, 99, L"%d-%02d-%02d", year, month, day);
		}
	}
    
	String CStrA::GetDBString(const String& strSrc)
	{
		String str = Replace(strSrc, L"'", L"''");
		return str;
	}
    
	string CStrA::GetGuid()
	{
		static char buf[64] = {0};
		return buf;
	}
    
	int CStrA::GetStringCapacity(const string& str)
	{
		return (int)str.length() + 1 + sizeof(double);
	}
    
	void CStrA::GetValueByDigit(double value, int digit, wchar_t *str)
	{
		if(!isnan(value))
		{
			if(digit == 0)
			{
				double newValue = round(value);
				if(abs(value - newValue) < 1)
				{
					value = newValue;
				}
			}
			switch(digit)
			{
                case 0:
                    swprintf(str, 99, L"%d", (int)value);
                    break;
                case 1:
                    swprintf(str, 99, L"%.1f", value);
                    break;
                case 2:
                    swprintf(str, 99, L"%.2f", value);
                    break;
                case 3:
                    swprintf(str, 99, L"%.3f", value);
                    break;
                case 4:
                    swprintf(str, 99, L"%.4f", value);
                    break;
                case 5:
                    swprintf(str, 99, L"%.5f", value);
                    break;
                default:
                    swprintf(str, 99, L"%f", value);
			}
		}
		str = 0;
	}
    
	int CStrA::GetStringCapacity(const String& str)
	{
		return ((int)str.length() + 1) * 2 + sizeof(double);
	}
    
    /* Compress gzip data */
    /* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
    int CStrA::GZCompress(Byte *data, uLong ndata, Byte *zdata, uLong *nzdata)
    {
        z_stream c_stream;
        int err = 0;
        
        if(data && ndata > 0)
        {
            c_stream.zalloc = Z_NULL;
            c_stream.zfree = Z_NULL;
            c_stream.opaque = Z_NULL;
            //只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
            if(deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                            MAX_WBITS + 16, 6, Z_DEFAULT_STRATEGY) != Z_OK)
            {
                return -1;
            }
            
            c_stream.next_in  = data;
            c_stream.avail_in  = ndata;
            c_stream.next_out = zdata;
            c_stream.avail_out  = *nzdata;
            while(c_stream.avail_in != 0 && c_stream.total_out < *nzdata)
            {
                if(deflate(&c_stream, Z_NO_FLUSH) != Z_OK)
                {
                    return -1;
                }
            }
            
            if(c_stream.avail_in != 0)
            {
                return c_stream.avail_in;
            }
            
            for(;;)
            {
                if((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END)
                {
                    break;
                }
                if(err != Z_OK)
                {
                    return -1;
                }
            }
            
            if(deflateEnd(&c_stream) != Z_OK)
            {
                return -1;
            }
            *nzdata = c_stream.total_out;
            return 0;
        }
        return -1;
    }
    
    /* Uncompress gzip data */
    /* zdata 数据 nzdata 原数据长度 data 解压后数据 ndata 解压后长度 */
    int CStrA::GZDeCompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
    {
        int err = 0;
        z_stream d_stream = {0}; /* decompression stream */
        static char dummy_head[2] =
        {
            0x8 + 0x7 * 0x10,
            (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
        };
        
        d_stream.zalloc = Z_NULL;
        d_stream.zfree = Z_NULL;
        d_stream.opaque = Z_NULL;
        d_stream.next_in  = zdata;
        d_stream.avail_in = 0;
        d_stream.next_out = data;
        //只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
        if(inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK)
        {
            return -1;
        }
        
        //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
        while(d_stream.total_out < *ndata && d_stream.total_in < nzdata)
        {
            d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
            if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END)
            {
                break;
            }
            if(err != Z_OK)
            {
                if(err == Z_DATA_ERROR)
                {
                    d_stream.next_in = (Bytef*) dummy_head;
                    d_stream.avail_in = sizeof(dummy_head);
                    
                    if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
                    {
                        return -1;
                    }
                } else
                {
                    return -1;
                }
            }
        }
        if(inflateEnd(&d_stream) != Z_OK)
        {
            return -1;
        }
        *ndata = d_stream.total_out;
        return 0;
    }
    
	double CStrA::M129(int tm_year, int tm_mon, int tm_mday, int tm_hour, int tm_min, int tm_sec, int tm_msec)
	{
        struct tm t;
        time_t tn;
        t.tm_sec = tm_sec;
        t.tm_min = tm_min;
        t.tm_hour = tm_hour;
        t.tm_mday = tm_mday;
        t.tm_mon = tm_mon - 1;
        t.tm_year = tm_year - 1900;
        tn = mktime(&t);
        return (long)tn + (double)tm_msec / 1000 + 28800;
	}
    
	void CStrA::M130(double num, int *tm_year, int *tm_mon, int *tm_mday, int *tm_hour, int *tm_min, int *tm_sec, int *tm_msec)
	{
        time_t tn = (long)num;
        struct tm* t = gmtime(&tn);
        *tm_sec = t->tm_sec;
        *tm_min = t->tm_min;
        *tm_hour = t->tm_hour;
        *tm_mday = t->tm_mday;
        *tm_mon = t->tm_mon + 1;
        *tm_year = t->tm_year + 1900;
        *tm_msec = (long)((num * 1000 - floor(num) * 1000));
	}
    
	string CStrA::Replace(const string& str, const string& src, const string& dest)
	{
		string newStr = str;
		std::string::size_type pos = 0;
		while( (pos = newStr.find(src, pos)) != string::npos )
		{
			newStr.replace(pos, src.length(), dest);
			pos += dest.length();
		}
		return newStr;
	}
    
	String CStrA::Replace(const String& str, const String& src, const String& dest)
	{
		String newStr = str;
		String::size_type pos = 0;
		while( (pos = newStr.find(src, pos)) != string::npos )
		{
			newStr.replace(pos, src.length(), dest);
			pos += dest.length();
		}
		return newStr;
	}
    
	vector<string> CStrA::Split(string str, string pattern)
	{
        int pos = -1;
        vector<string> result;
        string newStr = str;
        newStr += pattern;
        int size = (int)str.size();
        for(int i = 0; i < size; i++)
        {
			pos = (int)newStr.find(pattern, i);
			if((int)pos <= size)
			{
				string s = newStr.substr(i, pos - i);
				if(s.length() > 0)
				{
					result.push_back(s);
				}
				i = pos + (int)pattern.size() - 1;
			}
		}
		return result;
	}
    
	vector<String> CStrA::Split(String str, String pattern)
	{
        int pos = -1;
        vector<String> result;
        String newStr = str;
        newStr += pattern;
        int size = (int)str.size();
        for(int i = 0; i < size; i++)
        {
			pos = (int)newStr.find(pattern, i);
			if((int)pos <= size)
			{
				String s = newStr.substr(i, pos - i);
				result.push_back(s);
				i = pos + (int)pattern.size() - 1;
			}
		}
		return result;
	}
    
	void CStrA::stringTowstring(String &strDest, const string& strSrc)
	{
#if TARGET_RT_BIG_ENDIAN
        const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
        const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif
        @autoreleasepool {
        NSString *str = [NSString stringWithUTF8String:strSrc.c_str()];
        NSData *asData = [str dataUsingEncoding:kEncoding_wchar_t];
        strDest = String((wchar_t*)[asData bytes], [asData length] / sizeof(wchar_t));
        }
	}
    
	String CStrA::ToLower(const String& str)
	{
        String lowerStr = str;
        transform(str.begin(), str.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
	}
    
	String CStrA::ToUpper(const String& str)
	{
        String upperStr = str;
        transform(str.begin(), str.end(), upperStr.begin(), ::toupper);
        return upperStr;
	}
    
	void CStrA::wstringTostring(string &strDest, const String& strSrc)
	{
#if TARGET_RT_BIG_ENDIAN
        const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
        const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif
        @autoreleasepool {
        char* data = (char*)strSrc.data();
        unsigned size = strSrc.size() * sizeof(wchar_t);
        NSString* result = [[NSString alloc] initWithBytes:data length:size encoding:kEncoding_wchar_t];
        strDest = [result UTF8String];
        }
	}
}
