/*******************************************************************************\
 *                                                                               *
 * GaiaScript.h - Gaia script functions, types, and definitions.             *
 *                                                                               *
 *               Version 6.00 ¡ï¡ï¡ï                                                *
 *                                                                               *
 *               Copyright (c) 2016-2016, Piratecat. All rights reserved.        *
 *               Created by Lord 2016/12/2.                                      *
 *                                                                               *
 ********************************************************************************/

#ifndef __GAIASCRIPT_H__
#define __GAIASCRIPT_H__
#pragma once
#include "stdafx.h"
#include "CFunctionEx.h"

class GaiaScript : public UIScriptA
{
private:
    CIndicator *m_gaia;
    UIXmlA *m_xml;
public:
    GaiaScript(UIXmlA *xml);
    virtual ~GaiaScript();
public:
    virtual String CallFunction(const String& function);
    String GetProperty(const String& name, const String& propertyName);
    String GetSender();
    void SetProperty(const String& name, const String& propertyName, const String& propertyValue);
    virtual void SetText(const String& text);
};

#endif
