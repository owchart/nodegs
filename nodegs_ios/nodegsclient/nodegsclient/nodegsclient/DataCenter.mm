#include "stdafx.h"
#include "DataCenter.h"
namespace OwLibCT
{
    LoginService *m_loginService;
    LoginService* DataCenter::GetLoginService()
    {
        return m_loginService;
    }
    
    void DataCenter::StartService()
    {
        m_loginService = new LoginService;
        BaseService::AddService(m_loginService);
    }
}
