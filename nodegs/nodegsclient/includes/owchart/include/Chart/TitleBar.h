/*****************************************************************************\
*                                                                             *
* TitleBar.h -  Title Bar functions, types, and definitions                   *
*                                                                             *
*               Version 4.00 ¡ï¡ï¡ï¡ï¡ï                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's owchart. All rights reserved. *
*               Modified by Wang Shaoxu.                                      *
*******************************************************************************/

#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__
#pragma once
#include "..\\..\\stdafx.h"
#include "..\\Base\\CPaint.h"
#include "CTable.h"
#include "Enums.h"
#include "CList.h"
#include "CDiv.h"

namespace OwLib
{
	class CDiv;

	class CTitle : public CProperty
	{
	protected:
		int m_digit;
		int m_fieldName;
		String m_fieldText;
		TextMode m_fieldTextMode;
		String m_fieldTextSeparator;
		_int64 m_textColor;
		bool m_visible;
	public:
		CTitle(int fieldName, const String& fieldText, _int64 color, int digit, bool visible);
		virtual int GetDigit();
		virtual void SetDigit(int digit);
		virtual int GetFieldName();
		virtual void SetFieldName(int fieldName);
		virtual String GetFieldText();
		virtual void SetFieldText(const String& fieldText);
		virtual TextMode GetFieldTextMode();
		virtual void SetFieldTextMode(TextMode fieldTextMode);
		virtual String GetFieldTextSeparator();
		virtual void SetFieldTextSeparator(const String& fieldTextSeparator);
		virtual _int64 GetTextColor();
		virtual void SetTextColor(_int64 textColor);
		virtual bool IsVisible();
		virtual void SetVisible(bool visible);
	public:
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		virtual void SetProperty(const String& name, const String& value);
	};

	class TitleBar : public CProperty
	{
	protected:
		bool m_allowUserPaint;
		FONT *m_font;
		_int64 m_foreColor;
		int m_height;
		int m_maxLine;
		bool m_showUnderLine;
		String m_text;
		_int64 m_underLineColor;
		bool m_visible;
	public:
		TitleBar();
		virtual ~TitleBar();
		vector<CTitle*> Titles;
		virtual bool AllowUserPaint();
		virtual void SetAllowUserPaint(bool allowUserPaint);
		virtual FONT* GetFont();
		virtual void SetFont(FONT *font);
		virtual _int64 GetForeColor();
		virtual void SetForeColor(_int64 foreColor);
		virtual int GetHeight();
		virtual void SetHeight(int height);
		virtual int GetMaxLine();
		virtual void SetMaxLine(int maxLine);
		virtual bool ShowUnderLine();
		virtual void SetShowUnderLine(bool showUnderLine);
		virtual String GetText();
		virtual void SetText(const String& text);
		virtual _int64 GetUnderLineColor();
		virtual void SetUnderLineColor(_int64 underLineColor);
		virtual bool IsVisible();
		virtual void SetVisible(bool visible);
	public:
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		virtual void OnPaint(CPaint *paint, CDiv *div, const RECT& rect);
		virtual void SetProperty(const String& name, const String& value);
	};
}
#endif