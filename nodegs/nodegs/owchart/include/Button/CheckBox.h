/*****************************************************************************\
*                                                                             *
* CheckBox.h -  Check box functions, types, and definitions                   *
*                                                                             *
*               Version 4.00 ¡ï¡ï¡ï¡ï¡ï                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's button. All rights reserved.  *
*               Checked 2016/10/8 by Lord.                                    *
*                                                                             *
*******************************************************************************/

#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__
#pragma once
#include "..\\..\\stdafx.h"
#include "..\\Base\\CStr.h"
#include "Button.h"

namespace OwLib
{
	class CheckBoxA : public ButtonA
	{
	protected:
		HorizontalAlignA m_buttonAlign;
		_int64 m_buttonBackColor;
		_int64 m_buttonBorderColor;
		SIZE m_buttonSize;
		bool m_checked;
		String m_checkedBackImage;
		String m_checkHoveredBackImage;
		String m_checkPushedBackImage;
		String m_disableCheckedBackImage;
	protected:
		virtual _int64 GetPaintingBackColor();
		virtual _int64 GetPaintingButtonBackColor();
		virtual _int64 GetPaintingButtonBorderColor();
		virtual String GetPaintingBackImage();
	public:
		CheckBoxA();
		virtual ~CheckBoxA();
		virtual HorizontalAlignA GetButtonAlign();
		virtual void SetButtonAlign(HorizontalAlignA buttonAlign);
		virtual _int64 GetButtonBackColor();
		virtual void SetButtonBackColor(_int64 buttonBackColor);
		virtual _int64 GetButtonBorderColor();
		virtual void SetButtonBorderColor(_int64 buttonBorderColor);
		virtual SIZE GetButtonSize();
		virtual void SetButtonSize(SIZE buttonSize);
		virtual bool IsChecked();
		virtual void SetChecked(bool checked);
		virtual String GetCheckedBackImage();
		virtual void SetCheckedBackImage(const String& checkedBackImage);
		virtual String GetCheckHoveredBackImage();
		virtual void SetCheckHoveredBackImage(const String& checkHoveredBackImage);
		virtual String GetCheckPushedBackImage();
		virtual void SetCheckPushedBackImage(const String& checkPushedBackImage);
		virtual String GetDisableCheckedBackImage();
		virtual void SetDisableCheckedBackImage(const String& disableCheckedBackImage);
	public:
		virtual String GetControlType();
		virtual vector<String> GetEventNames();
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		virtual void OnCheckedChanged();
		virtual void OnClick(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnPaintBackground(CPaint *paint, const RECT& clipRect);
		virtual void OnPaintCheckButton(CPaint *paint, const RECT& clipRect);
		virtual void OnPaintForeground(CPaint *paint, const RECT& clipRect);
		virtual void SetProperty(const String& name, const String& value);
	};
}

#endif