#pragma once
#include"EditControl_编辑框控件.h"
#include<windows.h>
#include"vector_向量.h"
#include"resource.h"
enum _ControlType
{
	CT_NAME,
	CT_POSITION
};
class DetaileControl_细节菜单控件组合
{
protected:
	HWND m_hWnd;
public:
	HWND GethWnd()const { return m_hWnd; }
	void Hide()const { ShowWindow(m_hWnd, SW_HIDE); }
	bool IsVisible() const { return IsWindowVisible(m_hWnd); }
	DetaileControl_细节菜单控件组合() :m_hWnd(nullptr) {}
	virtual ~DetaileControl_细节菜单控件组合();
	int MoveWind_移动窗口(int x, int y, int w);
	int GetHeight();
	static bool IsNumeric(const std::wstring& str);
};
class Position_位置控件 :public DetaileControl_细节菜单控件组合
{
public:
	Position_位置控件(HINSTANCE,HWND parent, int x, int y, int w);
	~Position_位置控件();
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	vec::Vector3 m_Position;
};
class Name_对象名称控件 : public DetaileControl_细节菜单控件组合
{
public:
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Name_对象名称控件(HINSTANCE hIns, HWND parent, int x, int y, int w);
};
class Rotation_旋转控件 :public DetaileControl_细节菜单控件组合
{
	Rotation_旋转控件(HINSTANCE, HWND parent, int x, int y, int w);
	static INT_PTR Dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};