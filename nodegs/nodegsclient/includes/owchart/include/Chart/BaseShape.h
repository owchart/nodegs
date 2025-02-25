/*****************************************************************************\
*                                                                             *
* BaseShape.h - Base Shape functions, types, and definitions                  *
*                                                                             *
*               Version 4.00  ������                                      *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's owchart. All rights reserved. *
*               Modified by Wang Shaoxu.                                      *
*******************************************************************************/

#ifndef __BASESHAPE_H__
#define __BASESHAPE_H__
#pragma once

#include "..\\..\\stdafx.h"
#include "..\\Base\\CPaint.h"
#include "Enums.h"
#include "Chart.h"
#include "CDiv.h"
#include "CTable.h"

namespace OwLib
{
	class  CDiv;
	class  ChartA;
	class  BaseShape : public CProperty
	{
	protected:
		bool m_allowUserPaint;
		AttachVScale m_attachVScale;
	protected:
		bool m_selected;
		bool m_visible;
		int m_zOrder;
	public:
		BaseShape();
		virtual ~BaseShape();
		virtual bool AllowUserPaint();
		virtual void SetAllowUserPaint(bool allowUserPaint);
		virtual AttachVScale GetAttachVScale();
		virtual void SetAttachVScale(AttachVScale attachVScale);
		virtual bool IsSelected();
		virtual void SetSelected(bool selected);
		virtual bool IsVisible();
		virtual void SetVisible(bool visible);
		virtual int GetZOrder();
		virtual void SetZOrder(int zOrder);
	public:
		virtual int GetBaseField();
		virtual wstring GetFieldText(int fieldName);
		virtual int* GetFields(int *length);
		virtual void GetProperty(const wstring& name, wstring *value, wstring *type);
		virtual vector<wstring> GetPropertyNames();
		virtual _int64 GetSelectedColor();
		virtual void OnPaint(CPaint *paint, CDiv *div, const RECT& rect);
		virtual void SetProperty(const wstring& name, const wstring& value);
	};

	class  CandleShape : public BaseShape
	{
	protected:
		int m_closeField;
		wstring m_closeFieldText;
		int m_colorField;
		_int64 m_downColor;
		int m_highField;
		wstring m_highFieldText;
		int m_lowField;
		wstring m_lowFieldText;
		int m_openField;
		wstring m_openFieldText;
		bool m_showMaxMin;
		CandleStyle m_style;
		int m_styleField;
		_int64 m_tagColor;
		_int64 m_upColor;
	public:
		CandleShape();
		int GetCloseField();
		void SetCloseField(int closeField);
		wstring GetCloseFieldText();
		void SetCloseFieldText(const wstring& closeFieldText);
		int GetColorField();
		void SetColorField(int colorField);
		_int64 GetDownColor();
		void SetDownColor(_int64 downColor);
		int GetHighField();
		void SetHighField(int highField);
		wstring GetHighFieldText();
		void SetHighFieldText(const wstring& highFieldText);
		int GetLowField();
		void SetLowField(int lowField);
		wstring GetLowFieldText();
		void SetLowFieldText(const wstring& lowFieldText);
		int GetOpenField();
		void SetOpenField(int openField);
		wstring GetOpenFieldText();
		void SetOpenFieldText(const wstring& openFieldText);
		bool GetShowMaxMin();
		void SetShowMaxMin(bool showMaxMin);
		CandleStyle GetStyle();
		void SetStyle(CandleStyle style);
		int GetStyleField();
		void SetStyleField(int styleField);
		_int64 GetTagColor();
		void SetTagColor(_int64 tagColor);
		_int64 GetUpColor();
		void SetUpColor(_int64 upColor);
	public:
		virtual int GetBaseField();
		virtual wstring GetFieldText(int fieldName);
		virtual int* GetFields(int *length);
		virtual void GetProperty(const wstring& name, wstring *value, wstring *type);
		virtual vector<wstring> GetPropertyNames();
		virtual _int64 GetSelectedColor();
		virtual void SetProperty(const wstring& name, const wstring& value);
	};

	class  BarShape:public BaseShape
	{
	protected:
		int m_colorField;
		_int64 m_downColor;
		int m_fieldName;
		int m_fieldName2;
		wstring m_fieldText;
		wstring m_fieldText2;
		float m_lineWidth;
		BarStyle m_style;
		int m_styleField;
		_int64 m_upColor;
	public:
		BarShape();
		int GetColorField();
		void SetColorField(int colorField);
		_int64 GetDownColor();
		void SetDownColor(_int64 downColor);
		int GetFieldName();
		void SetFieldName(int fieldName);
		int GetFieldName2();
		void SetFieldName2(int fieldName2);
		wstring GetFieldText();
		void SetFieldText(const wstring& fieldText);
		wstring GetFieldText2();
		void SetFieldText2(const wstring& fieldText2);
		float GetLineWidth();
		void SetLineWidth(float lineWidth);
		BarStyle GetStyle();
		void SetStyle(BarStyle style);
		int GetStyleField();
		void SetStyleField(int styleField);
		_int64 GetUpColor();
		void SetUpColor(_int64 upColor);
	public:
		virtual int GetBaseField();
		virtual wstring GetFieldText(int fieldName);
		virtual int* GetFields(int *length);
		virtual void GetProperty(const wstring& name, wstring *value, wstring *type);
		virtual vector<wstring> GetPropertyNames();
		virtual _int64 GetSelectedColor();
		virtual void SetProperty(const wstring& name, const wstring& value);
	};

	class  PolylineShape:public BaseShape
	{
	protected:
		_int64 m_color;
		int m_colorField;
		int m_fieldName;
		wstring m_fieldText;
		_int64 m_fillColor;
		PolylineStyle m_style;
		float m_width;
	public:
		PolylineShape();
		_int64 GetColor();
		void SetColor(_int64 color);
		int GetColorField();
		void SetColorField(int colorField);
		int GetFieldName();
		void SetFieldName(int fieldName);
		wstring GetFieldText();
		void SetFieldText(const wstring& fieldText);
		_int64 GetFillColor();
		void SetFillColor(_int64 fillColor);
		PolylineStyle GetStyle();
		void SetStyle(PolylineStyle style);
		float GetWidth();
		void SetWidth(float width);
	public:
		virtual int GetBaseField();
		virtual wstring GetFieldText(int fieldName);
		virtual int* GetFields(int *length);
		virtual void GetProperty(const wstring& name, wstring *value, wstring *type);
		virtual vector<wstring> GetPropertyNames();
		virtual _int64 GetSelectedColor();
		virtual void SetProperty(const wstring& name, const wstring& value);
	};

	class  TextShape:public BaseShape
	{
	protected:
		int m_colorField;
		int m_fieldName;
		FONT *m_font;
		_int64 m_foreColor;
		int m_styleField;
		wstring m_text;
	public:
		TextShape();
		virtual ~TextShape();
		int GetColorField();
		void SetColorField(int colorField);
		int GetFieldName();
		void SetFieldName(int fieldName);
		FONT* GetFont();
		void SetFont(FONT *font);
		_int64 GetForeColor();
		void SetForeColor(_int64 foreColor);
		int GetStyleField();
		void SetStyleField(int styleField);
		wstring GetText();
		void SetText(const wstring& text);
	public:
		virtual void GetProperty(const wstring& name, wstring *value, wstring *type);
		virtual vector<wstring> GetPropertyNames();
		virtual void SetProperty(const wstring& name, const wstring& value);
	};
}
#endif