#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include"vector_œÚ¡ø.h"
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
typedef struct runMsg
{
	std::wstring Time;
	std::wstring Str;
	char Type;
	runMsg() :Type(0) {}
	runMsg(const std::wstring& time, const std::wstring& str, const char& type) :Time(time), Str(str), Type(type) {}
}runMsg;
class TextOutWind
{
	HWND m_hWnd;
	std::vector<HWND>m_ChildControl;
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
	size_t OutMessage(const std::string&, const char&);
	size_t OutMessage(const std::wstring&, const char&);
	void DrawWind(HDC hdc);
	HWND GethWnd();
	static LRESULT CALLBACK TextWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

