/*******************************************************************************************\
 *                                                                                           *
 * CFunctionBase.cs -  Base functions, types, and definitions.                            *
 *                                                                                           *
 *               Version 1.00  ¡ï¡ï¡ï                                                       *
 *                                                                                           *
 *               Copyright (c) 2016-2016, Piratecat. All rights reserved.                    *
 *               Created by Lord 2016/10/17.                                                  *
 *                                                                                           *
 ********************************************************************************************/

#ifndef __CFUNCTIONBASE_H__
#define __CFUNCTIONBASE_H__
#pragma once
#include "stdafx.h"

namespace OwLibSV
{
	class CFunctionBase : public CFunction
	{
	private:
		CIndicator *m_indicator;
	public:
		CFunctionBase(CIndicator *indicator, int cid, const String& name);
		virtual ~CFunctionBase();
	public:
		static void AddFunctions(CIndicator *indicator);
		virtual double OnCalculate(CVariable *var);
	public:
		double INPUT(CVariable *var);
		double OUTPUT(CVariable *var);
		double SLEEP(CVariable *var);
	};
}

#endif

