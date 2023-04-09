#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include<vector>
#include<string>
typedef struct _T树列表节点
{
	HTREEITEM own;
	std::vector<_T树列表节点>child;
	_T树列表节点() :own(nullptr) {}
}T_树节点;
typedef struct WndMsg
{
	HWND hWnd;
	std::vector<WndMsg>ChildWnd;
	std::wstring className;
	RECT m_rect;
	WndMsg() :hWnd(nullptr) { m_rect = { 0 }; }
}WndMsg;
typedef struct runMsg
{
	std::wstring Time;
	std::wstring Str;
	char Type;
	runMsg() :Type(0) {}
	runMsg(const std::wstring& time, const std::wstring& str, const char& type) :Time(time), Str(str), Type(type) {}
}runMsg;
class ModelTriData
{
public:
	float d;
	POINT point[3];
	ModelTriData()
	{
		point[0] = { 0 };
		point[1] = { 0 };
		point[2] = { 0 };
		d = 0;
	}
	bool operator>(ModelTriData b)
	{
		return d > b.d ? true : false;
	}
	bool operator<(ModelTriData b)
	{
		return d < b.d ? true : false;
	}
};
ATOM MyRegisterClass(HINSTANCE   hInstance,
	LPCWSTR     lpszClassName,
	LRESULT(*wnd_proc)(HWND, UINT, WPARAM, LPARAM),
	LPCWSTR     lpszMenuName = NULL,
	UINT        style = CS_HREDRAW | CS_VREDRAW,
	HCURSOR     hCursor = NULL,
	HBRUSH      hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
	HICON       hIconSm = NULL,
	HICON       hIcon = NULL,
	int         cbClsExtra = 0,
	int         cbWndExtra = 0);