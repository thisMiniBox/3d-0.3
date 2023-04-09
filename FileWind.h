#pragma once
#include<Windows.h>
#include"Tree_树控件.h"
#include"Object.h"
#include<string>
class FileWind
{
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HWND m_Tree;
	std::wstring m_ClassName;
	Tree_树控件 m_FileTree_文件树;
public:
	FileWind(HINSTANCE hIns);
	~FileWind();
	HWND CreateWind(HWND parent);
	//节点的位置用【1，2】格式表示，此格式表示添加到第一个节点下的第二个节点中，
	//【1.0】则表示添加到第一个节点下，若0写在前面后面的信息将被忽略,
	//非正数都会被当作0处理
	HTREEITEM AddItem(const Object&, const std::string& address);
	HTREEITEM AddItem(const Object&, HTREEITEM parent = TVI_ROOT);
	void DeleteItem(HTREEITEM);
	HTREEITEM GetSelectedItem();
	Object* GetSelectedItemData();
	HTREEITEM GetMousePositionItem();
	Object* GetMousePositionItemData();
	void FixItemName(HTREEITEM, const std::wstring Name);
	void ShowFolder(const Folder&, HTREEITEM parent = TVI_ROOT);
	void MoveTree(int x, int y, int w, int h);
	HWND GethWnd();
	HWND GetTree();
	static LRESULT CALLBACK    FileWndProc(HWND, UINT, WPARAM, LPARAM);
};