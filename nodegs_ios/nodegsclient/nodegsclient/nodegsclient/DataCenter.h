/*****************************************************************************\
 *                                                                             *
 * LoginService.h -  Login service functions, types, and definitions           *
 *                                                                             *
 *               Version 1.00 ★                                               *
 *                                                                             *
 *               Copyright (c) 2016-2016, Client. All rights reserved.         *
 *               Created by shaoxu.wang.                                       *
 *                                                                             *
 *******************************************************************************/

#ifndef __DATACENTER_H__
#define __DATACENTER_H__
#pragma once
#include "LoginService.h"

namespace OwLibCT
{
    class DataCenter
    {
    private:
        
        
    public:
        static LoginService* GetLoginService();
        static void StartService();
    };
}

#endif
