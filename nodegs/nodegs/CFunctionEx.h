/*******************************************************************************************\
 *                                                                                           *
 * CFunctionEx.cs -  Indicator functions, types, and definitions.                            *
 *                                                                                           *
 *               Version 1.00  ¡ï¡ï¡ï                                                      *
 *                                                                                           *
 *               Copyright (c) 2016-2016, Piratecat. All rights reserved.                    *
 *               Created by Lord 2016/10/17.                                                  *
 *                                                                                           *
 ********************************************************************************************/

#ifndef __CFUNCTIONEX_H__
#define __CFUNCTIONEX_H__
#pragma once
#include "stdafx.h"

namespace OwLibSV
{
	class CFunctionEx : public CFunction
	{
	private:
		CIndicator *m_indicator;
		NativeBase *m_native;
	public:
		CFunctionEx(CIndicator *indicator, int cid, const String& name, NativeBase *native);
		virtual ~CFunctionEx();
	public:
		static CIndicator* CreateIndicator(const String& script, NativeBase *native);
		virtual double OnCalculate(CVariable *var);
	};
}
#endif

