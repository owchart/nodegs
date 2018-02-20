#include "HttpGetService.h"
#include "Binary.h"
#include "..\\CStr.h"
#include "..\\CurlHttp.h"

namespace OwLibCT
{
    string HttpGetService::Get(const string url)
    {
		CurlHttp *http = CurlHttp::GetInstance(0);
		return http->Get(url, false, 10000);
    }
}
