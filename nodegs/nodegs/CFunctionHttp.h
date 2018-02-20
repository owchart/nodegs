/*******************************************************************************************\
 *                                                                                           *
 * CFunctionHttp.cs -  CFunctionHttp functions, types, and definitions.                      *
 *                                                                                           *
 *               Version 1.00  ¡ï¡ï¡ï                                                        *
 *                                                                                           *
 *               Copyright (c) 2016-2016, Piratecat. All rights reserved.                    *
 *               Created by Lord 2016/10/17.                                                 *
 *                                                                                           *
 ********************************************************************************************/

#ifndef __CFUNCTIONHTTP_H__
#define __CFUNCTIONHTTP_H__
#pragma once
#include "stdafx.h"
#include "NodeService.h"
#include "HttpEasyService.h"
#include "DataCenter.h"

namespace OwLibSV
{
	class CFunctionHttp : public CFunction
	{
	private:
		HttpData *m_data;
		CIndicator *m_indicator;
	public:
		CFunctionHttp(CIndicator *indicator, int cid, const String& name);
		virtual ~CFunctionHttp();
		HttpData* GetHttpData();
		void SetHttpData(HttpData *data);
	public:
		static void AddFunctions(CIndicator *indicator);
		virtual double OnCalculate(CVariable *var);
	public:
		double HTTP_ADDPORT(CVariable *var);
		double HTTP_CHECKSCRIPT(CVariable *var);
		double HTTP_CLOSE(CVariable *var);
		double HTTP_EASYREQUEST(CVariable *var);
		double HTTP_GETCONTENTTYPE(CVariable *var);
		double HTTP_GETREMOTEIP(CVariable *var);
		double HTTP_GETREMOTEPORT(CVariable *var);
		double HTTP_GETREQUESTMETHOD(CVariable *var);
		double HTTP_GETREQUESTURL(CVariable *var);
		double HTTP_GETSERVICENAME(CVariable *var);
		double HTTP_HARDREQUEST(CVariable *var);
		double HTTP_POSTREQUEST(CVariable *var);
		double HTTP_POSTSTRING(CVariable *var);
		double HTTP_QUERYSTRING(CVariable *var);
		double HTTP_SETSTATUSCODE(CVariable *var);
		double HTTP_WRITE(CVariable *var);
	};
}
#endif

