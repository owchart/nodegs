/*********************************************************************************\
*                                                                                 *
* Tree.h -      Tree functions, types, and definitions                            *
*                                                                                 *
*               Version 4.00 ¡ï¡ï¡ï¡ï¡ï                                           *
*                                                                                 *
*               Copyright (c) 2016-2016, Lord's true. All rights reserved.        *
*               Check 2016/9/23 by Lord.                                          *
*                                                                                 *
***********************************************************************************/

#ifndef __TREE_H__
#define __TREE_H__
#pragma once
#include "..\\..\\stdafx.h"
#include "..\\Grid\\Grid.h"
#include "..\\Grid\\GridColumn.h"
#include "..\\Grid\\GridRow.h"
#include "..\\Grid\\GridCell.h"
#include "..\\Grid\\GridCellExtends.h"
#include "TreeNode.h"

namespace OwLib
{
	class TreeNodeA;

	class TreeA:public GridA
	{
	protected:
		bool m_checkBoxes;
		SIZE m_checkBoxSize;
		String m_checkedImage;
		String m_collapsedNodeImage;
		String m_expendedNodeImage;
		TreeNodeA *m_movingNode;
		SIZE m_nodeSize;
		String m_unCheckedImage;
	public:
		vector<TreeNodeA*> m_nodes;
		TreeA();
		virtual ~TreeA();
		virtual bool HasCheckBoxes();
		virtual void SetCheckBoxes(bool checkBoxes);
		virtual SIZE GetCheckBoxSize();
		virtual void SetCheckBoxSize(SIZE checkBoxSize);
		virtual String GetCheckedImage();
		virtual void SetCheckedImage(const String& checkedImage);
		virtual String GetCollapsedNodeImage();
		virtual void SetCollapsedNodeImage(const String& collapsedNodeImage);
		virtual String GetExpendedNodeImage();
		virtual void SetExpendedNodeImage(const String& expendedNodeImage);
		virtual SIZE GetNodeSize();
		virtual void SetNodeSize(SIZE nodeSize);
		virtual vector<TreeNodeA*> GetSelectedNodes();
		virtual void SetSelectedNodes(vector<TreeNodeA*> selectedNodes);
		virtual String GetUnCheckedImage();
		virtual void SetUnCheckedImage(const String& unCheckedImage);
	public:
		void AppendNode(TreeNodeA *node);
		void ClearNodes();
		void Collapse();
		void CollapseAll();
		void Expend();
		void ExpendAll();
		vector<TreeNodeA*> GetChildNodes();
		virtual String GetControlType();
		int GetNodeIndex(TreeNodeA *node);
		virtual void GetProperty(const String& name, String *value, String *type);
		virtual vector<String> GetPropertyNames();
		void InsertNode(int index, TreeNodeA *node);
		virtual void OnCellMouseDown(GridCell *cell, const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnCellMouseMove(GridCell *cell, const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnCellMouseUp(GridCell *cell, const POINT& mp, MouseButtonsA button, int clicks, int delta);
		virtual void OnPaintForeground(CPaint *paint, const RECT& clipRect);
		virtual void OnPaintEditTextBox(GridCell *cell, CPaint *paint, const RECT& rect, const RECT& clipRect);
		void RemoveNode(TreeNodeA *node);
		virtual void SetProperty(const String& name, const String& value);
	};
}

#endif