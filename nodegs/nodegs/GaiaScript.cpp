#include "stdafx.h"
#include "GaiaScript.h"

GaiaScript::GaiaScript(NativeBase *native) : UIScriptA(xml)
{
    m_gaia = 0;
    m_xml = xml;
}

GaiaScript::~GaiaScript()
{
    if(m_gaia)
    {
        delete m_gaia;
        m_gaia = 0;
    }
    m_xml = 0;
}

String GaiaScript::CallFunction(const String& function)
{
    double value = m_gaia->CallFunction(function);
    return CStr::ConvertDoubleToStr(value);
}

String GaiaScript::GetProperty(const String& name, const String& propertyName)
{
    if (m_xml)
    {
        ControlA *control = m_xml->FindControl(name);
        if (control)
        {
            String value = L"", type = L"";
            control->GetProperty(propertyName, &value, &type);
            return value;
        }
    }
    return L"";
}

String GaiaScript::GetSender()
{
    if (m_xml)
    {
        UIEventA *uiEvent = m_xml->GetEvent();
        if (uiEvent)
        {
            return uiEvent->GetSender();
        }
    }
    return 0;
}

void GaiaScript::SetProperty(const String& name, const String& propertyName, const String& propertyValue)
{
    if (m_xml)
    {
        ControlA *control = m_xml->FindControl(name);
        if (control)
        {
            control->SetProperty(propertyName, propertyValue);
        }
    }
}

void GaiaScript::SetText(const String& text)
{
	m_gaia = CFunctionEx::CreateIndicator(text, m_xml);
}
