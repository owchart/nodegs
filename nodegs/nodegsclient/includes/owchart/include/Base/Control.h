/*****************************************************************************\
*                                                                             *
* Control.h - Base control functions, types, and definitions              *
*                                                                             *
*               Version 4.00 ������                                       *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's owchart. All rights reserved. *
*               Check 2016/9/15 by Lord.                                      *
*                                                                             *
*******************************************************************************/

#ifndef __BASECONTROL_H__
#define __BASECONTROL_H__
#pragma once

#include "..\\..\\stdafx.h"
#include "NativeBase.h"
#include "CPaint.h"
#include "CStr.h"
#include "CProperty.h"
#include "ControlHost.h"

namespace OwLib
{
	class  NativeBase;
	class  ControlA;

	typedef  void (*ControlEvent)(void*, void*);
	typedef  void (*ControlInvokeEvent)(void*, void*, void*);
	typedef  void (*ControlKeyEvent)(void*, char, void*);
	typedef  void (*ControlMouseEvent)(void*, const POINT&, MouseButtonsA, int, int, void*);
	typedef  void (*ControlPaintEvent)(void*, CPaint*, const RECT&, void*);
	typedef  void (*ControlTimerEvent)(void*, int, void*);
	typedef  void (*ControlTouchEvent)(void*, vector<CTouch>*, void*);

	class  EVENTID
	{
	public:
        static const int ADD = 0;
        static const int AUTOSIZECHANGED = 1;
        static const int BACKCOLORCHANGED = 2;
        static const int BACKIMAGECHANGED = 3;
        static const int CHAR = 4;
        static const int CLICK = 5;
        static const int COPY = 6;
        static const int CUT = 7;
        static const int DOCKCHANGED = 8;
        static const int DOUBLECLICK = 9;
        static const int DRAGBEGIN = 10;
        static const int DRAGEND = 11;
        static const int DRAGGING = 12;
        static const int ENABLECHANGED = 13;
        static const int FONTCHANGED = 14;
        static const int FORECOLORCHANGED = 15;
        static const int GOTFOCUS = 16;
        static const int INVOKE = 17;
        static const int KEYDOWN = 18;
        static const int KEYUP = 19;
        static const int LOAD = 20;
        static const int LOCATIONCHANGED = 21;
        static const int LOSTFOCUS = 22;
        static const int MARGINCHANGED = 23;
        static const int MOUSEDOWN = 24;
        static const int MOUSEENTER = 25;
        static const int MOUSELEAVE = 26;
        static const int MOUSEMOVE = 27;
        static const int MOUSEUP = 28;
        static const int MOUSEWHEEL = 29;
        static const int PADDINGCHANGED = 30;
        static const int PARENTCHANGED = 31;
        static const int PAINT = 32;
        static const int PAINTBORDER = 33;
        static const int PASTE = 34;
        static const int REGIONCHANGED = 36;
        static const int REMOVE = 37;
        static const int SIZECHANGED = 38;
        static const int TABINDEXCHANGED = 39;
		static const int TABSTOP = 40;
        static const int TABSTOPCHANGED = 41;
        static const int TEXTCHANGED = 42;
        static const int TIMER = 43;
        static const int VISIBLECHANGED = 44;
        static const int CHECKEDCHANGED = 45;
        static const int SELECTEDTIMECHANGED = 46;
        static const int GRIDCELLCLICK = 47;
        static const int GRIDCELLEDITBEGIN = 48;
        static const int GRIDCELLEDITEND = 49;
        static const int GRIDCELLMOUSEDOWN = 50;
        static const int GRIDCELLMOUSEENTER = 51;
        static const int GRIDCELLMOUSELEAVE = 52;
        static const int GRIDCELLMOUSEMOVE = 53;
        static const int GRIDCELLMOUSEUP = 54;
        static const int GRIDSELECTEDCELLSCHANGED = 55;
        static const int GRIDSELECTEDCOLUMNSSCHANGED = 56;
        static const int GRIDSELECTEDROWSCHANGED = 57;
        static const int MENUITEMCLICK = 58;
        static const int SELECTEDINDEXCHANGED = 59;
        static const int SELECTEDTABPAGECHANGED = 60;
        static const int SCROLLED = 61;
        static const int VALUECHANGED = 62;
        static const int WINDOWCLOSED = 63;
        static const int WINDOWCLOSING = 64;
        static const int TOUCHBEGIN = 10000;
        static const int TOUCHMOVE = 10001;
        static const int TOUCHEND = 10002;
        static const int TOUCHCANCEL = 10003;
        static const int USER = 100000;
	};

	class  ControlA : public CProperty
	{
	protected:
		HorizontalAlignA m_align;
		bool m_allowDrag;
		bool m_allowPreviewsEvent;
		ANCHOR m_anchor;
		bool m_autoEllipsis;
		bool m_autoShowToolTip;
		bool m_autoSize;
		_int64 m_backColor;
		wstring m_backImage;
		_int64 m_borderColor;
		bool m_canFocus;
		bool m_canRaiseEvents;
		CursorsA m_cursor;
		bool m_displayOffset;
		DockStyleA m_dock;
		bool m_enabled;
		bool m_focused;
		FONT *m_font;
		_int64 m_foreColor;
		bool m_hasPopupMenu;
		bool m_isDragging;
		bool m_isWindow;
		POINT m_location;
		PADDING m_margin;
		SIZE m_maximumSize;
		SIZE m_minimumSize;
		wstring m_name;
		NativeBase *m_native;
		POINT m_oldLocation;
		SIZE m_oldSize;
		float m_opacity;
		PADDING m_padding;
		ControlA *m_parent;
		POINTF *m_percentLocation;
		SIZEF *m_percentSize;
		RECT m_region;
		wstring m_resourcePath;
		SIZE m_size;
		int m_tabIndex;
		bool m_tabStop;
		void *m_tag;
		wstring m_text;
		bool m_topMost;
		bool m_useRegion;
		VerticalAlignA m_verticalAlign;
		bool m_visible;
	protected:
		map<int, vector<void*>*> m_events;
		map<int, vector<void*>*> m_invokes;
		void CallEvents(int eventID);
		void CallInvokeEvents(int eventID, void *args);
		void CallKeyEvents(int eventID, char key);
		void CallMouseEvents(int eventID, const POINT& mp, MouseButtonsA button, int clicks, int delta);
		void CallPaintEvents(int eventID, CPaint *paint, const RECT& clipRect);
		void CallTimerEvents(int eventID, int timerID);
		void CallTouchEvents(int eventID, vector<CTouch> *touches);
        virtual _int64 GetPaintingBackColor();
        virtual wstring GetPaintingBackImage();
        virtual _int64 GetPaintingBorderColor();
        virtual _int64 GetPaintingForeColor();
	public:
		vector<ControlA*> m_controls;
		ControlA();
		virtual ~ControlA();
		virtual HorizontalAlignA GetAlign();
		virtual void SetAlign(HorizontalAlignA align);
		virtual bool AllowDrag();
		virtual void SetAllowDrag(bool allowDrag);
		virtual bool AllowPreviewsEvent();
		virtual void SetAllowPreviewsEvent(bool allowPreviewsEvent);
		virtual ANCHOR GetAnchor();
		virtual void SetAnchor(const ANCHOR& anchor);
		virtual bool AutoEllipsis();
		virtual void SetAutoEllipsis(bool autoEllipsis);
		virtual bool AutoShowToolTip();
		virtual void SetAutoShowToolTip(bool autoShowToolTip);
		virtual bool AutoSize();
		virtual void SetAutoSize(bool autoSize);
		virtual _int64 GetBackColor();
		virtual void SetBackColor(_int64 backColor);
		virtual wstring GetBackImage();
		virtual void SetBackImage(const wstring& backImage);
		virtual _int64 GetBorderColor();
		virtual void SetBorderColor(_int64 borderColor);
		virtual int GetBottom();
		virtual RECT GetBounds();
		virtual void SetBounds(const RECT& rect);
		virtual bool CanFocus();
		virtual void SetCanFocus(bool canFocus);
		virtual bool CanRaiseEvents();
		virtual void SetCanRaiseEvents(bool canRaiseEvents);
		virtual bool IsCapture();
		virtual CursorsA GetCursor();
		virtual void SetCursor(CursorsA cursor);
		virtual bool DisplayOffset();
		virtual void SetDisplayOffset(bool displayOffset);
		virtual RECT GetDisplayRect();
		virtual DockStyleA GetDock();
		virtual void SetDock(DockStyleA dock);
		virtual bool IsEnabled();
		virtual void SetEnabled(bool enabled);
		virtual bool IsFocused();
		virtual void SetFocused(bool focused);
		virtual FONT* GetFont();
		virtual void SetFont(FONT *font);
		virtual _int64 GetForeColor();
		virtual void SetForeColor(_int64 foreColor);
		virtual bool HasPopupMenu();
		virtual void SetHasPopupMenu(bool hasPopupMenu);
		virtual int GetHeight();
		virtual void SetHeight(int height);
		virtual bool IsDragging();
		virtual bool IsWindow();
		virtual void SetWindow(bool isWindow);
		virtual int GetLeft();
		virtual void SetLeft(int left);
		virtual POINT GetLocation();
		virtual void SetLocation(const POINT& location);
		virtual PADDING GetMargin();
		virtual void SetMargin(const PADDING& margin);
		virtual SIZE GetMaximumSize();
		virtual void SetMaximumSize(SIZE maxinumSize);
		virtual SIZE GetMinimumSize();
		virtual void SetMinimumSize(SIZE minimumSize);
		virtual POINT GetMousePoint();
		virtual wstring GetName();
		virtual void SetName(const wstring& name);
		virtual NativeBase* GetNative();
		void SetNative(NativeBase *native);
		virtual float GetOpacity();
		virtual void SetOpacity(float opacity);
		virtual PADDING GetPadding();
		virtual void SetPadding(const PADDING& padding);
		virtual ControlA* GetParent();
		virtual void SetParent(ControlA *control);
		virtual RECT GetRegion();
		virtual void SetRegion(const RECT& region);
		virtual wstring GetResourcePath();
		virtual void SetResourcePath(const wstring& resourcePath);
		virtual int GetRight();
		virtual SIZE GetSize();
		virtual void SetSize(const SIZE& size);
		virtual int GetTabIndex();
		virtual void SetTabIndex(int tabIndex);
		virtual bool IsTabStop();
		virtual void SetTabStop(bool tabStop);
		virtual void* GetTag();
		virtual void SetTag(void *tag);
		virtual wstring GetText();
		virtual void SetText(const wstring& text);
		virtual int GetTop();
		virtual void SetTop(int top);
		virtual bool IsTopMost();
		virtual void SetTopMost(bool topMost);
		virtual bool UseRegion();
		virtual VerticalAlignA GetVerticalAlign();
		virtual void SetVerticalAlign(VerticalAlignA verticalAlign);
		virtual bool IsVisible();
		virtual void SetVisible(bool visible);
		virtual int GetWidth();
		virtual void SetWidth(int width);
	public:
		virtual void AddControl(ControlA *control);
		virtual void BeginInvoke(void *args);
		virtual void BringChildToFront(ControlA *childControl);
		virtual void BringToFront();
		virtual void ClearControls();
		virtual bool ContainsControl(ControlA *control);
		virtual bool ContainsPoint(const POINT& mp);
		virtual void Focus();
		virtual vector<ControlA*> GetControls();
		virtual wstring GetControlType();
		virtual POINT GetDisplayOffset();
		virtual vector<wstring> GetEventNames();
		static int GetNewTimerID();
		virtual ControlA* GetPopupMenuContext(ControlA *control);
		virtual void GetProperty(const wstring& name, wstring *value, wstring *type);
		virtual vector<wstring> GetPropertyNames();
		virtual bool HasChildren();
		virtual void Hide();
		virtual void InsertControl(int index, ControlA *control);
		virtual void Invalidate();
		virtual void Invoke(void *args);
		bool IsPaintEnabled(ControlA *control);
		bool IsPaintVisible(ControlA *control);
		virtual void OnAdd();
		virtual void OnAutoSizeChanged();
		virtual void OnBackColorChanged();
		virtual void OnBackImageChanged();
		virtual void OnChar(wchar_t ch);
		virtual void OnClick(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnCopy();
		virtual void OnCut();
		virtual void OnDockChanged();
		virtual void OnDoubleClick(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual bool OnDragBegin();
		virtual void OnDragEnd();
		virtual void OnDragging();
		virtual void OnDragReady(POINT *startOffset);
		virtual void OnEnableChanged();
		virtual void OnFontChanged();
		virtual void OnForeColorChanged();
		virtual void OnGotFocus();
		virtual void OnInvoke(void *args);
		virtual void OnLoad();
		virtual void OnLocationChanged();
		virtual void OnLostFocus();
		virtual void OnKeyDown(char key);
		virtual void OnKeyUp(char key);
		virtual void OnMarginChanged();
		virtual void OnMouseDown(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseEnter(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseLeave(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseMove(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseUp(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnMouseWheel(const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnPaddingChanged();
		virtual void OnPaint(CPaint *paint, const RECT& clipRect);
		virtual void OnPaintBackground(CPaint *paint, const RECT& clipRect);
		virtual void OnPaintBorder(CPaint *paint, const RECT& clipRect);
		virtual void OnPaintForeground(CPaint *paint, const RECT& clipRect);
		virtual void OnParentChanged();
		virtual void OnPaste();
		virtual void OnPrePaint(CPaint *paint, const RECT& clipRect);
		virtual bool OnPreviewsKeyEvent(int eventID, char key);
		virtual bool OnPreviewsMouseEvent(int eventID, const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnRegionChanged();
		virtual void OnRemove();
		virtual void OnSizeChanged();
		virtual void OnTabIndexChanged();
		virtual void OnTabStop();
		virtual void OnTabStopChanged();
		virtual void OnTextChanged();
		virtual void OnTimer(int timerID);
		virtual void OnTouchBegin(vector<CTouch> *touches);
		virtual void OnTouchCancel(vector<CTouch> *touches);
		virtual void OnTouchEnd(vector<CTouch> *touches);
		virtual void OnTouchMove(vector<CTouch> *touches);
		virtual void OnVisibleChanged();
		virtual POINT PointToControl(const POINT& mp);
		virtual POINT PointToNative(const POINT& mp);
		virtual void RegisterEvent(void *func, int eventID, void *pInvoke);
		virtual void RemoveControl(ControlA *control);
		virtual void SetProperty(const wstring& name, const wstring& value);
		virtual void Show();
		virtual void StartTimer(int timerID, int interval);
		virtual void StopTimer(int timerID);
		virtual void SendChildToBack(ControlA *childControl);
		virtual void SendToBack();
		virtual void UnRegisterEvent(void *func, int eventID);
		virtual void Update();
	};
}
#endif