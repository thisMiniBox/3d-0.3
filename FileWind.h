#pragma once
#include<Windows.h>
#include"resource.h"
#include"Tree_Ê÷¿Ø¼þ.h"
#include"Object.h"
#include"WndData.h"
#include<string>
class FileWind
{
	HINSTANCE m_hInst;
	HWND m_hWnd;
	std::wstring m_ClassName;
	TreeControl* m_FileTreeView;
	std::map<int, int>m_PngImglist;
	int GetPngID(int pngIndex);
	void ShowFolderRecursion(const Folder&, const Object* aim, HTREEITEM);
	void ShowModel(const Model*, HTREEITEM);
public:
	FileWind(HWND parent,HINSTANCE hIns);
	~FileWind();
	void ExploreFolder(HTREEITEM);
	void ExploreFolder(Object*);
	HTREEITEM AddItem(const Object&, HTREEITEM parent = TVI_ROOT);
	void DeleteItem(HTREEITEM);
	void DeleteItem(Object*);
	void MoveItem(HTREEITEM moved, HTREEITEM parent);
	HTREEITEM GetSelectedItem();
	Object* GetSelectedItemData();
	HTREEITEM GetMousePositionItem();
	Object* GetMousePositionItemData();
	Object* GetItemObject(HTREEITEM)const;
	void SetMappingBasedOnObjects(const Object& object, HTREEITEM hItem);
	void SetNodeImage(HTREEITEM hItem, int imageIndex, int imageIndexSelected = -1);
	void FixItemName(HTREEITEM, const std::wstring Name);
	void FixItemName(Object*, const std::wstring Name);
	void ShowFolder(const Folder&, const Object* aim, HTREEITEM parent = TVI_ROOT);
	void MoveTree(int x, int y, int w, int h);
	HWND GethWnd();
	static LRESULT CALLBACK    FileWndProc(HWND, UINT, WPARAM, LPARAM);
};