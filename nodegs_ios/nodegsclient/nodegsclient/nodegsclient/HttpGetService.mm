#include "stdafx.h"
#include "HttpGetService.h"
namespace OwLibCT
{
    string HttpGetService::Get(string url)
    {
        string buffer = "";
        NSString *nUrl = [NSString stringWithUTF8String:url.c_str()];
        NSString *encodedValue = [nUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        NSURLRequest * urlRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:encodedValue]];
        
        NSURLResponse * response = nil;
        NSError * error = nil;
        NSData * data = [NSURLConnection sendSynchronousRequest:urlRequest
                                              returningResponse:&response
                                                          error:&error];
        NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
        NSString *nBuffer = [[NSString alloc] initWithData:data encoding:enc];
        buffer = [nBuffer UTF8String];
        return buffer;
    }
    
    String HttpGetService::Get(String url)
    {
        string sUrl = "";
        CStrA::wstringTostring(sUrl, url);
        string result = Get(sUrl);
        String resultS = L"";
        CStrA::stringTowstring(resultS, result);
        return resultS;
    }
}
