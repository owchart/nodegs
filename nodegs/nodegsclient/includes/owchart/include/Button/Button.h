/*****************************************************************************\
*                                                                             *
* Button.h -  Button functions, types, and definitions                        *
*                                                                             *
*               Version 4.00 ¡ï¡ï¡ï¡ï¡ï                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's button. All rights reserved.  *
*               Checked 2016/10/8 by Lord.                                    *
*                                                                             *
*******************************************************************************/

#ifndef __BUTTON_H__
#define __BUTTON_H__
#pragma once
#include "..\\..\\stdafx.h"
#include "..\\Base\\CStr.h"

namespace OwLib
{
	class ButtonA:public ControlA
	{
	protected:
		String m_disabledBackImage;
		String m_hoveredBackImage;
		String m_pushedBackImage;
		ContentAlignmentA m_textAlign;
	protected:
		virtual _int64 GetPaintingBackColor();
		virtual String GetPaintingBackImage();
	public:
		ButtonA();
		virtual ~ButtonA();
		virtual String GetDisabledBackImage();
		virtual void SetDisabledBackImage(const String& disabledBackImage);
		virtual String GetHoveredBackImage();
		virtual void SetHoveredBackImage(const String& hoveredBackImage);
		virtual String GetPushedBackImage();
		virtual void SetPushedBackImage(const String& pushedBackImage);
		virtual ContentAlignmentA GetTextAlign();
		virtual void SetTextAlign(ContentAlignmentA textAlign);
	public:
		virtual String GetControlType();
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		virtual void OnMouseDown(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseEnter(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseLeave(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseUp(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnPaintForeground(CPaint *paint, const RECT& clipRect);
		virtual void SetProperty(const String& name, const String& value);
	};
}

#endif