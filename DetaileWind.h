#pragma once
#include<Windows.h>
#include"Object.h"
#include"EditControl_编辑框控件.h"
#include"DetaileControl_细节菜单控件组合.h"
#include"resource.h"
#include<string>
#include<unordered_map>
#include<commctrl.h>
#define WM_UPDATE WM_USER+1
class DetaileWind
{
	HINSTANCE m_hInstance;
	const int HI = 100;
	HWND m_hWnd;
	std::unordered_map<int,DetaileControl_细节菜单控件组合*> m_ChildControl;
	Object* m_target;
	RECT m_rect;
	std::wstring WindClassName;
	HTREEITEM m_TreeTarget;
	int CreateContrle(int type,int x, int y, int w);
public:
	DetaileWind();
	~DetaileWind();
	const HWND GethWnd()const { return m_hWnd; }
	const RECT GetRect()const { return m_rect; }
	HTREEITEM GetTree()const { return m_TreeTarget; }
	void SetRect(RECT Rect);
	void UpDate(int type = -1);
	HWND CreateWind(HWND parent);
	void SetView(Object*);
	void SetTree(HTREEITEM);
	Object* GetTarget() { return m_target; }
	HWND CreateDialogToWind(LRESULT(CALLBACK* aWNDPROC)(HWND, UINT, WPARAM, LPARAM), LPCWSTR lpTemplate);
	static LRESULT CALLBACK DetaileWndProc(HWND, UINT, WPARAM, LPARAM);
};
