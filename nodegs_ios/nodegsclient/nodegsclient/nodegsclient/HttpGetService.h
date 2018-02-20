//*************************************************************************************\
//*                                                                                      *
//* HttpGetService.h -  Http get service functions, types, and definitions.              *
//*                                                                                      *
//*               Version 1.00 �                                                      *
//*                                                                                      *
//*               Copyright (c) 2016-2016, Server. All rights reserved.                  *
//*               Created by Lord.                                                    *
//*                                                                                      *
//**************************************************************************************

#ifndef __HTTPGETSERVICE_H__
#define __HTTPGETSERVICE_H__
#pragma once
#include "stdafx.h"
namespace OwLibCT
{
    class HttpGetService
    {
    public:
        string Get(string url);
        String Get(String url);
    };
}


#endif
