#pragma once
#include<Windows.h>
#include"resource.h"
#include"Tree_���ؼ�.h"
#include"Object.h"
#include"WndData.h"
#include<string>
class FileWind
{
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HWND m_Tree;
	std::wstring m_ClassName;
	Tree_���ؼ� m_FileTree_�ļ���;
	void ShowFolderRecursion(const Folder&, const Object* aim, HTREEITEM);
public:
	FileWind(HINSTANCE hIns);
	~FileWind();
	void ExploreFolder(HTREEITEM);
	HWND CreateWind(HWND parent);
	//�ڵ��λ���á�1��2����ʽ��ʾ���˸�ʽ��ʾ��ӵ���һ���ڵ��µĵڶ����ڵ��У�
	//��1.0�����ʾ��ӵ���һ���ڵ��£���0д��ǰ��������Ϣ��������,
	//���������ᱻ����0����
	HTREEITEM AddItem(const Object&, const std::string& address);
	HTREEITEM AddItem(const Object&, HTREEITEM parent = TVI_ROOT);
	HTREEITEM GetItemParent(HTREEITEM);
	void DeleteItem(HTREEITEM);
	HTREEITEM GetSelectedItem();
	Object* GetSelectedItemData();
	HTREEITEM GetMousePositionItem();
	Object* GetMousePositionItemData();
	void SetMappingBasedOnObjects(const Object& object, HTREEITEM hItem);
	void SetNodeImage(HTREEITEM hItem, int imageIndex, int imageIndexSelected = -1);
	void FixItemName(HTREEITEM, const std::wstring Name);
	void ShowFolder(const Folder&, const Object* aim, HTREEITEM parent = TVI_ROOT);
	void MoveTree(int x, int y, int w, int h);
	HWND GethWnd();
	HWND GetTree();
	static LRESULT CALLBACK    FileWndProc(HWND, UINT, WPARAM, LPARAM);
};