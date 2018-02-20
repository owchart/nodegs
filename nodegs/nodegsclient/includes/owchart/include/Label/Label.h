/*****************************************************************************\
*                                                                             *
* Label.h -  Label functions, types, and definitions                          *
*                                                                             *
*               Version 4.00 ¡ï¡ï¡ï                                           *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's label. All rights reserved.   *
*                                                                             *
*******************************************************************************/

#ifndef __LABEL_H__
#define __LABEL_H__
#pragma once
#include "..\\..\\stdafx.h"

namespace OwLib
{
	class LabelA : public ControlA
	{
	protected:
		ContentAlignmentA m_textAlign;
	public:
		LabelA();
		virtual ~LabelA();
		virtual ContentAlignmentA GetTextAlign();
		virtual void SetTextAlign(ContentAlignmentA textAlign);
	public:
		virtual String GetControlType();
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		virtual void OnPaintForeground(CPaint *paint, const RECT& clipRect);
		virtual void OnPrePaint(CPaint *paint, const RECT& clipRect);
		virtual void SetProperty(const String& name, const String& value);
	};
}
#endif