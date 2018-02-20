/*****************************************************************************\
*                                                                             *
* SelectArea.h -Select Area functions, types, and definitions                 *
*                                                                             *
*               Version 4.00 ¡ï¡ï¡ï¡ï¡ï                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's owchart. All rights reserved. *
*               Modified by Wang Shaoxu.                                      *
*******************************************************************************/

#ifndef __SELECTAREA_H__
#define __SELECTAREA_H__
#pragma once
#include "..\\..\\stdafx.h"
#include "CDiv.h"

namespace OwLib
{
	class CDiv;

	class SelectArea : public CProperty
	{
	protected:
		bool m_allowUserPaint;
		_int64 m_backColor;
		RECT m_bounds;
		bool m_canResize;
		bool m_enabled;
		_int64 m_lineColor;
		bool m_visible;
	public:
		SelectArea();
		virtual ~SelectArea();
		virtual bool AllowUserPaint();
		virtual void SetAllowUserPaint(bool allowUserPaint);
		virtual _int64 GetBackColor();
		virtual void SetBackColor(_int64 backColor);
		virtual RECT GetBounds();
		virtual void SetBounds(RECT bounds);
		virtual bool CanResize();
		virtual void SetCanResize(bool canResize);
		virtual bool IsEnabled();
		virtual void SetEnabled(bool enabled);
		virtual _int64 GetLineColor();
		virtual void SetLineColor(_int64 lineColor);
		virtual bool IsVisible();
		virtual void SetVisible(bool visible);
	public:
		void Close();
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		virtual void OnPaint(CPaint *paint, CDiv *div, const RECT& rect);
		virtual void SetProperty(const String& name, const String& value);

	};
}
#endif