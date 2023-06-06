#pragma once
#include <windows.h>
#include <string>
#include<CommCtrl.h>
#include<unordered_map>
#include"WndData.h"
#include"vector_ÏòÁ¿.h"
#include"Object.h"
#include"×Ö·û×ª»».h"
#define buttonH 20
class BottomWindow
{
public:
	BottomWindow(HINSTANCE,HWND);
	~BottomWindow();
	HWND CreateWind(HWND parent);
	HWND GethWnd()const;
	void AddWind(HWND hWnd, const std::wstring& text);
	void DeleteWind(HWND hWnd);
	void Size(int w, int h);
	HWND Select(HWND hWnd);
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
private:
	int m_w;
	int m_h;
	HWND m_hWnd;
	HWND m_Foucs;
	UINT m_ChildWindNumber;
	std::unordered_map<HWND, HWND>m_Index;
	HINSTANCE m_hInst;
};

typedef struct runMsg
{
	std::wstring Time;
	std::wstring Str;
	MSGtype Type;
	runMsg(const std::wstring& time, const std::wstring& str, MSGtype type) :Time(time), Str(str), Type(type) {}
}runMsg;
class TextOutWind
{
	HWND m_hWnd;
	HWND m_ChildControl[4];
	HINSTANCE m_hInst;
	LOGFONT m_TextTypeFace;
	unsigned int m_MessageSizeMaximum;
	unsigned int m_DisplayMode;
	vec::Vector2 m_DisplayPosition;
	std::vector<runMsg>m_MessageData;
public:
	COLORREF ErrorOutColor = RGB(255, 0, 0);
	COLORREF WarningOutColor = RGB(150, 150, 0);
	COLORREF RemindOutColor = RGB(0, 0, 255);

	TextOutWind(HINSTANCE);
	~TextOutWind();
	HWND CreateWind(HWND Parent);
	void MoveText(const vec::Vector2&);
	void SetTextPosition(const vec::Vector2&);
	void SetTextMaximum(unsigned int max);
	void SetOutMode(unsigned int mode);
	void SetFont(LOGFONT);
	void UpdateWindowSize(int w, int h);
	size_t size();
	size_t OutMessage(const std::string&, MSGtype);
	size_t OutMessage(const std::wstring&, MSGtype);
	void DrawWind(HDC hdc);
	HWND GethWnd();
	static LRESULT CALLBACK TextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
class InputOutput
{
	HWND m_hWnd;
	HWND m_Input;
	HWND m_Output;
	HWND m_Enter;
	HINSTANCE m_hInst;
	UINT m_MaxText;
	UINT m_CurrentText;
public:
	InputOutput(HINSTANCE,HWND parent);
	~InputOutput();
	HWND GethWnd()const;
	void Size();
	static LRESULT CALLBACK IOWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	std::wstring InputString();
	void DeleteLineText(int line);
	void OutputString(const std::wstring&);
	void Clear();
};
class KeyframeEdit
{
	HWND m_hWnd;
	HWND m_hTime;
	HWND m_hCanvas;
	HWND m_hButten;
	HWND m_hFile;

	std::wstring m_ClassName;
	std::wstring m_TimeClassName;
	std::wstring m_FileClassName;
	std::wstring m_CanvasClassName;
	std::wstring m_ButtenClassName;
	HINSTANCE m_hInst;

	ULONG64 m_LeftTime;
	ULONG64 m_RightTime;
	int m_Y;
public:
	KeyframeEdit(HINSTANCE hInst, HWND parent);
	~KeyframeEdit();
	void UpdateView()const;
	HWND GethWnd()const;
	void MoveSize(int w, int h);
	int GetY()const;
	void MoveY(int y);
	void MoveTime(int x);
	void GetTime(ULONG64* left, ULONG64* right)const;
	static LRESULT CALLBACK KeyframeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK KeyframeTimeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK KeyframeFileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK KeyframeButtenProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK KeyframeCanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};