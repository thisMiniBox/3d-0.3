#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include"resource.h"
enum class ChildWindSign
{
	MainWind = 1,
	GdiWind,
	OpenGLWind,
	D3D11Wind,

	LeftWind,
	FileWind,
	ProjectContentWind,

	BottomWind,
	MessageWind,
	ControlWind,
	KeyframeWind,

	RightWind,
	AttributeWind,

	TopWind,
	MenuWind,

	LeftSeekBar,
	BottomSeekBar,
	RightSeekBar,
	TopSeekBar,
};

enum MSGMode
{
	_ALL,
	_ERROR,
	_WARNING,
	_REMIND
};
enum MSGtype
{
	_Message,
	_Error,
	_Warning
};

typedef struct WndMsg
{
	HWND hWnd;
	std::vector<WndMsg>ChildWnd;
	std::wstring className;
	RECT m_rect;
	WndMsg() :hWnd(nullptr) { m_rect = { 0 }; }
}WndMsg;
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