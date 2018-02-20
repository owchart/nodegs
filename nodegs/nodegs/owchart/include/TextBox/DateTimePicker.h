/********************************************************************************\
*                                                                                *
* DatePicker.h -  Date picker functions, types, and definitions                  *
*                                                                                *
*               Version 4.00 ¡ï¡ï¡ï¡ï¡ï                                          *
*                                                                                *
*               Copyright (c) 2016-2016, Lord's date picker. All rights reserved.*
*               Checked 2016/9/25 by Lord.                                       *
*                                                                                *
*********************************************************************************/

#ifndef __DATETIMEPICKER_H__
#define __DATETIMEPICKER_H__
#pragma once
#include "..\\..\\stdafx.h"
#include "TextBox.h"
#include "..\\Button\\Button.h"
#include "..\\Menu\\Menu.h"
#include "..\\Menu\\MenuItem.h"
#include "..\\Calendar\\Calendar.h"

namespace OwLib
{
	class DateTimePickerA : public TextBoxA
	{
	protected:
		CalendarA *m_calendar;
		String m_customFormat;
		ButtonA *m_dropDownButton;
		ControlMouseEvent m_dropDownButtonMouseDownEvent;
		ControlEvent m_selectedTimeChangedEvent;
		MenuA *m_dropDownMenu;
		static void DropDownButtonMouseDown(void *sender, const POINT& mp, MouseButtonsA button, int clicks, int delta, void *pInvoke);
		static void SelectedTimeChanged(void *sender, void *pInvoke);
	public:
		DateTimePickerA();
		virtual ~DateTimePickerA();
		virtual CalendarA* GetCalendar();
		virtual String GetCustomFormat();
		virtual void SetCustomFormat(const String& customFormat);
		virtual ButtonA* GetDropDownButton();
		virtual MenuA* GetDropDownMenu();
	public:
		virtual String GetControlType();
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		virtual void OnDropDownOpening();
		virtual void OnLoad();
		virtual void OnSelectedTimeChanged();
		virtual void SetProperty(const String& name, const String& value);
		virtual void Update();
	};
}

#endif